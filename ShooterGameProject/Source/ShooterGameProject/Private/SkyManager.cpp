#include "SkyManager.h"
#include "Kismet/GameplayStatics.h"
#include "BGameInstance.h"

ASkyManager::ASkyManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASkyManager::BeginPlay()
{
	Super::BeginPlay();

    AActor* SkyActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASkyAtmosphere::StaticClass());
    SkyComponent = Cast<USkyAtmosphereComponent>(SkyActor->GetComponentByClass(USkyAtmosphereComponent::StaticClass()));

}

void ASkyManager::UpdateSkyColor(int CurrentStage)
{
    if (!SkyComponent) return;

    FLinearColor RayleighColor;

    switch (CurrentStage)
    {//»öÁ¶ÇÕ³ªÁß¿¡¼öÁ¤
    case 1: RayleighColor = FLinearColor(0.2f, 0.4f, 1.0f, 1.0f); break; // Çª¸¥
    case 2: RayleighColor = FLinearColor(0.15f, 0.12f, 0.6f, 1.0f); break; // º¸¶ó¼¯
    case 3: RayleighColor = FLinearColor(0.16f, 0.02f, 0.07f, 1.0f); break; // ºÓÀº¼¯
    case 4: RayleighColor = FLinearColor(0.15f, 0.0f, 0.01f, 1.0f); break; // ºÓ°íÁøÇÔ +Èå¸´¹Ý´Þ
    case 5: RayleighColor = FLinearColor(0.1f, 0.0f, 0.0f, 1.0f); break; // ¾îµÓ°í°ËºÓ+ºÓÀº´Þ
    default: RayleighColor = FLinearColor::Black; break;
    }
    SkyComponent->SetRayleighScattering(RayleighColor);
}
