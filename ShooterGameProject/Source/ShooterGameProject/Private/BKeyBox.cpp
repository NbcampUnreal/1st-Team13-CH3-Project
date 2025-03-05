#include "BKeyBox.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BGameState.h"
#include "BGameInstance.h"
#include "BUIManager.h"
#include "BCharacter.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"

ABKeyBox::ABKeyBox()
{
    PrimaryActorTick.bCanEverTick = false;
    OpenDuration = 3.0f;
    bIsOpening = false;
    KeyCount = 1;

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
}

void ABKeyBox::StartOpenKeyBox()
{
    if (bIsOpening)
    {
        return;
    }
    bIsOpening = true;

    UE_LOG(LogTemp, Log, TEXT("Start Open Box"));
    GetWorldTimerManager().SetTimer(OpenTimerHandle, this, &ABKeyBox::CompleteOpenKeyBox, OpenDuration, false);
}

void ABKeyBox::CompleteOpenKeyBox()
{
    bIsOpening = false;
    UE_LOG(LogTemp, Log, TEXT("Complete Open Box"));

    EBoxItemType ItemType = RandomReward();
    GrantItemToPlayer(ItemType);

    if (ABGameState* GameState = Cast<ABGameState>(UGameplayStatics::GetGameState(this)))
    {
        GameState->ItemCollected();
    }
    Destroy();
}

void ABKeyBox::CancelOpenKeyBox()
{
    if (bIsOpening)
    {
        bIsOpening = false;
        GetWorldTimerManager().ClearTimer(OpenTimerHandle);
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

    if (RandomValue < 48.0f)
    {
        return EBoxItemType::Ammo;
    }
    else if (RandomValue < 48.0f + 30.0f) // 78%
    {
        return EBoxItemType::HealthKit;
    }
    else if (RandomValue < 48.0f + 30.0f + 5.0f) // 83%
    {
        return EBoxItemType::Weapon;
    }
    else if (RandomValue < 48.0f + 30.0f + 5.0f + 16.0f) // 99%
    {
        return EBoxItemType::Key;
    }
    else
    {
        return EBoxItemType::None; // 2%
    }
}

void ABKeyBox::GrantItemToPlayer(EBoxItemType ItemType)
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return;

    ABPlayerState* PlayerState = PlayerController->GetPlayerState<ABPlayerState>();
    if (!PlayerState) return;

    switch (ItemType)
    {
    case EBoxItemType::Ammo:
        UE_LOG(LogTemp, Log, TEXT("탄창 획득!"));
        // PlayerState 탄창추가 구현 후 추가
        break;

    case EBoxItemType::HealthKit:
        UE_LOG(LogTemp, Log, TEXT("회복약 획득!"));
        // PlayerState 회복약추가 구현 후 추가
        break;

    case EBoxItemType::Weapon:
        UE_LOG(LogTemp, Log, TEXT("무기 획득!"));
        // 무기추가?
        break;

    case EBoxItemType::Key:
        UE_LOG(LogTemp, Log, TEXT("열쇠 획득!"));
        if (ABGameState* GameState = Cast<ABGameState>(UGameplayStatics::GetGameState(this)))
        {
            GameState->CollectedKeys++;
        }
        break;

    case EBoxItemType::None:
        UE_LOG(LogTemp, Log, TEXT("꽝! 상자가 비어있습니다"));
        break;
    }
}