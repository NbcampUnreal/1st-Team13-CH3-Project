#include "BShopManagerPawn.h"
#include "BShopWidget.h"
#include "BCharacter.h"
#include "BGameInstance.h"
#include "BUIManager.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"

ABShopManagerPawn::ABShopManagerPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

	// Create and set up meshes
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	
	// Create and set up collision box
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(StaticMesh);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Changed to QueryOnly when shop is enabled
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABShopManagerPawn::OnCollisionBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ABShopManagerPawn::OnCollisionEndOverlap);

	// Create and set up point light / intensity = 0 when initialized
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(StaticMesh);
	PointLight->SetIntensity(0.f);
	LightIntensity = 500.f; // To be modified in the editor

	// Create and set up Spring Arm and Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(StaticMesh);

	// Get WBP class
	//ConstructorHelpers::FClassFinder<UUserWidget> ShopWidgetClassFinder(TEXT("/Game/Shop/WBP/WBP_BShopWidget"));
	//if (ShopWidgetClassFinder.Succeeded())
	//{
	//	ShopWidgetClass = ShopWidgetClassFinder.Class;
	//}

	// Initialize widget instance
	ShopWidgetInstance = nullptr;

	// Hide the shop owner (skeletal mesh) upon initialization
	SkeletalMesh->SetVisibility(false);

	bIsShopEnabled = false;
	bIsShopWidgetOpen = false;
}



// Turn the lights on and display skeletal mesh (TODO: To be called by BGameState in OpenDoor())
void ABShopManagerPawn::EnableShop()
{
	bIsShopEnabled = true;
	// TODO: do something with material so that the skeletal mesh appears ~gracefully~
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SkeletalMesh->SetVisibility(true);
	PointLight->SetIntensity(LightIntensity);
}

void ABShopManagerPawn::OpenShopWidget()
{
	if (ShopWidgetClass && ShopWidgetInstance == nullptr && GetWorld())
	{
		// Create shop widget and add to viewport
		ShopWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), ShopWidgetClass);
		if (ShopWidgetInstance)
		{
			ShopWidgetInstance->AddToViewport();
			if (UBShopWidget* ShopWidget = Cast<UBShopWidget>(ShopWidgetInstance))
			{
				ShopWidget->AddItemsToScrollBox();
				ShopWidget->DisplayPlayerCoin();
			}

			bIsShopWidgetOpen = true;

			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
			{
				// Show mouse cursor and set input mode to Game and UI
				PlayerController->bShowMouseCursor = true;
				PlayerController->SetInputMode(FInputModeGameAndUI());

				// Set view target to the shop pawn's camera component
				PlayerController->SetViewTargetWithBlend(this, 0.5f);
				
				// Hide character in case it's in the way
				PlayerController->GetCharacter()->SetActorHiddenInGame(true);
			}

			PlayAnim("Dance");
		}
	}
}

void ABShopManagerPawn::CloseShopWidget()
{
	// Remove widget
	if (ShopWidgetInstance && bIsShopWidgetOpen)
	{
		// Return camera to character camera?

		ShopWidgetInstance->RemoveFromParent();
		ShopWidgetInstance = nullptr;
		bIsShopWidgetOpen = false;

		PlayAnim("Wave");

		// Hide mouse cursor and set input mode to Game Only
		if (GetWorld())
		{
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
			{
				PlayerController->bShowMouseCursor = false;
				PlayerController->SetInputMode(FInputModeGameOnly());

				// Set view target to the character's camera component
				PlayerController->SetViewTargetWithBlend(PlayerController->GetCharacter(), 0.5f);

				// Display character
				PlayerController->GetCharacter()->SetActorHiddenInGame(false);
			}
		}
	}
}

void ABShopManagerPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayAnim("Idle");
}

void ABShopManagerPawn::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ABCharacter>())
	{		
		// Display notification
		if (UBGameInstance* GameInstance = GetGameInstance<UBGameInstance>())
		{
			if (UBUIManager* UIManager = GameInstance->GetUIManagerInstance())
			{
				if (!bIsShopEnabled)
				{
					UIManager->DisplayNotification("Shop is closed!", "Come back after all missions are completed.");
					return;
				}
				else
				{
					UIManager->DisplayNotification("Shop is open!", "Press E to interact.");
				}
			}
		}

		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (UInputComponent* PlayerInputComponent = PlayerController->InputComponent)
			{
				PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABShopManagerPawn::OnInteractKeyPressed);
			}
		}
	}
}

void ABShopManagerPawn::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{	
	if (OtherActor->IsA<ABCharacter>())
	{
		// Close shop widget in case the player somehow moves out of CollisionBox while the shop is open
		CloseShopWidget();

		// Remove action binding
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (UInputComponent* PlayerInputComponent = PlayerController->InputComponent)
			{
				PlayerInputComponent->RemoveActionBinding("Interact", IE_Pressed);
			}
		}
	}
}

void ABShopManagerPawn::OnInteractKeyPressed()
{
	if (bIsShopWidgetOpen)
	{
		CloseShopWidget();
	}
	else
	{
		OpenShopWidget();
	}
}

void ABShopManagerPawn::PlayAnim(const FName& SectionName)
{
	if (Montage == nullptr || SectionName == NAME_None) return;

	if (UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance())
	{
		if (AnimInstance->Montage_IsPlaying(Montage))
		{
			AnimInstance->Montage_Stop(0.5f, Montage);
		}

		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}
