#include "BKeyBox.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BGameState.h"
#include "BPlayerState.h"
#include "BGameInstance.h"
#include "BUIManager.h"
#include "BCharacter.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "BFirstAidKit.h"
#include "BGrenadeWeapon.h"
#include "BBattery.h"
#include "Components/ProgressBar.h"

ABKeyBox::ABKeyBox()
{
    PrimaryActorTick.bCanEverTick = false;
    OpenDuration = 3.0f;
    bIsOpening = false;

    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    SetRootComponent(RootSceneComponent);

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootSceneComponent);

    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
    MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
    MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootSceneComponent);
    CollisionBox->SetBoxExtent(FVector(60.f, 60.f, 2.f));

    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
    CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}
void ABKeyBox::BeginPlay()
{
    Super::BeginPlay();
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABKeyBox::OnCollisionBeginOverlap);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ABKeyBox::OnCollisionEndOverlap);

    if (OpenBoxWidgetClass)
    {
        OpenBoxWidget = CreateWidget<UUserWidget>(GetWorld(), OpenBoxWidgetClass);
        if (OpenBoxWidget)
        {
            OpenBoxWidget->AddToViewport();
            OpenBoxWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void ABKeyBox::StartOpenKeyBox()
{
    if (bIsOpening) return;
    bIsOpening = true;

    UE_LOG(LogTemp, Log, TEXT("Start Open Box"));
    if (OpenBoxWidget)
    {
        OpenBoxWidget->SetVisibility(ESlateVisibility::Visible);
    }

    GetWorldTimerManager().SetTimer(OpenTimerHandle, this, &ABKeyBox::CompleteOpenKeyBox, OpenDuration, false);
    GetWorldTimerManager().SetTimer(ProgressUpdateTimer, this, &ABKeyBox::UpdateProgressBar, 0.1f, true);
}

void ABKeyBox::CompleteOpenKeyBox()
{
    bIsOpening = false;
    UE_LOG(LogTemp, Log, TEXT("Complete Open Box"));
    if (OpenBoxWidget)
    {
        OpenBoxWidget->SetVisibility(ESlateVisibility::Hidden);
    }    
    GetWorldTimerManager().ClearTimer(ProgressUpdateTimer);

    EBoxItemType ItemType = RandomReward();
    GrantItemToPlayer(ItemType);
    Destroy();
}

void ABKeyBox::CancelOpenKeyBox()
{
    if (bIsOpening)
    {
        bIsOpening = false;
        GetWorldTimerManager().ClearTimer(OpenTimerHandle);
        GetWorldTimerManager().ClearTimer(ProgressUpdateTimer);
        if (OpenBoxWidget)
        {
            OpenBoxWidget->SetVisibility(ESlateVisibility::Hidden);
        }
        UE_LOG(LogTemp, Log, TEXT("* Cancel *"));
    }
}

void ABKeyBox::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->IsA<ABCharacter>())
    {
        if (UBGameInstance* GameInstance = GetGameInstance<UBGameInstance>())
        {
            if (UBUIManager* UIManager = GameInstance->GetUIManagerInstance())
            {
                UIManager->DisplayNotification("* Item Box *", "Press E to interact");
            }
        }
        if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
        {
            if (UInputComponent* PlayerInputComponent = PlayerController->InputComponent)
            {
                PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABKeyBox::StartOpenKeyBox);
            }
        }
    }
}

void ABKeyBox::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor->IsA<ABCharacter>())
    {
        CancelOpenKeyBox();
    }
}

EBoxItemType ABKeyBox::RandomReward()
{
    float RandomValue = FMath::RandRange(0.0f, 100.0f);
    if (RandomValue < 33.0f)
    {
        return EBoxItemType::HealthKit;
    }
    else if (RandomValue < 66.0f)
    {
        return EBoxItemType::Grenade;
    }
    else
    {
        return EBoxItemType::Key;
    }
}

void ABKeyBox::SpawnAndAddItemToInventory(APlayerController* PlayerController, const FString& ItemPath)
{
    if (!GetWorld() || !PlayerController) return;

    ABPlayerState* PlayerState = PlayerController->GetPlayerState<ABPlayerState>();
    if (!PlayerState) return;

    TSubclassOf<ABBaseItem> ItemClass = LoadClass<ABBaseItem>(nullptr, *ItemPath);
    if (!ItemClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ %s 클래스를 찾을 수 없음!"), *ItemPath);
        return;
    }

    FVector InvisibleLocation = FVector(0.f, 0.f, -1000.f);
    ABBaseItem* SpawnedItem = GetWorld()->SpawnActor<ABBaseItem>(ItemClass, InvisibleLocation, FRotator::ZeroRotator);
    if (!SpawnedItem)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ %s 스폰 실패!"), *ItemPath);
        return;
    }

    FItemData ItemData = SpawnedItem->GetItemData();
    PlayerState->InventoryAddItem(ItemData);
    UE_LOG(LogTemp, Log, TEXT("🎉 %s 인벤토리에 추가됨!"), *ItemPath);
    SpawnedItem->Destroy();
}

void ABKeyBox::GrantItemToPlayer(EBoxItemType ItemType)
{
    if (!GetWorld()) return;
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return;

    switch (ItemType)
    {
    case EBoxItemType::HealthKit:
        SpawnAndAddItemToInventory(PlayerController, TEXT("/Game/KSH/BluePrint/BP_FirstAidKit.BP_FirstAidKit_C"));
        break;

    case EBoxItemType::Grenade:
        SpawnAndAddItemToInventory(PlayerController, TEXT("/Game/KSH/BluePrint/BP_BGrenadeWeapon.BP_BGrenadeWeapon_C"));
        break;

    case EBoxItemType::Key:
        SpawnAndAddItemToInventory(PlayerController, TEXT("/Game/KSH/BluePrint/BP_BBattery.BP_BBattery_C"));
        if (ABGameState* GameState = Cast<ABGameState>(UGameplayStatics::GetGameState(this)))
        {
            GameState->ItemCollected();
        }
        break;
    }
}

void ABKeyBox::UpdateProgressBar()
{
    if (OpenBoxWidget)
    {
        UProgressBar* ProgressBar = Cast<UProgressBar>(OpenBoxWidget->GetWidgetFromName(TEXT("ProgressBar_105")));
        if (ProgressBar)
        {
            float TimeElapsed = OpenDuration - GetWorldTimerManager().GetTimerRemaining(OpenTimerHandle);
            float Progress = FMath::Clamp(TimeElapsed / OpenDuration, 0.0f, 1.0f);
            ProgressBar->SetPercent(Progress);
        }
    }
}