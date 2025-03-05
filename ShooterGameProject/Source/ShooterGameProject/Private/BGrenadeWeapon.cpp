// Fill out your copyright notice in the Description page of Project Settings.


#include "BGrenadeWeapon.h"
#include "GameFramework/Actor.h"
#include "BGameInstance.h"
#include "BCharacter.h"
#include "ABGrenadeProjectile.h"
#include "Kismet/GameplayStatics.h"

ABGrenadeWeapon::ABGrenadeWeapon()
{
    WeaponType = "Grenade";
    WeaponDamage = 200.0f;
    ItemPrice = 200;

    // 🔹 본체 (RootComponent로 설정)
    GrenadeBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeBody"));
    SetRootComponent(GrenadeBody); // ✅ RootComponent를 GrenadeBody로 설정

    // 🔹 초기 회전 설정
    GrenadeBody->SetRelativeRotation(FRotator::ZeroRotator);

    // 🔹 물리 비활성화
    GrenadeBody->SetSimulatePhysics(false); // ✅ 물리 적용 방지
    GrenadeBody->SetCollisionEnabled(ECollisionEnabled::NoCollision); // ✅ 충돌 비활성화

    // 🔹 부모 클래스에서 생성된 Collision이 WeaponMesh에 부착되었는지 확인
    if (Collision)
    {
        Collision->SetupAttachment(GrenadeBody); // ✅ GrenadeBody에 부착
        Collision->SetSimulatePhysics(false);   // ✅ 물리 시뮬레이션 해제
        Collision->SetRelativeLocation(FVector::ZeroVector); // ✅ 위치 초기화
    }
}


void ABGrenadeWeapon::ActivateItem(AActor* Activator)
{
    if (Activator && Activator->ActorHasTag("Player"))
    {
        ABCharacter* ActiveCharacter = Cast<ABCharacter>(Activator);
        if (ActiveCharacter)
        {
            ActiveCharacter->PickupWeapon(this);
            ActiveCharacter->GrenadeCount++;
            SetOwnerCharacter(ActiveCharacter);
            UE_LOG(LogTemp, Log, TEXT("Picked up %s and attached to %s"), *GetName(), *ActiveCharacter->GetName());
        }
    }
}

void ABGrenadeWeapon::Attack()
{
    UE_LOG(LogTemp, Warning, TEXT("🔹 Attack() 호출됨"));

    if (!GrenadeClass || !OwnerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GrenadeClass is NULL or OwnerCharacter is NULL!"));
        return;
    }
    if (OwnerCharacter->GrenadeCount <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GrenadeCount is 0"));
        return;
    }

    // 🔹 수류탄 개수 감소
    OwnerCharacter->GrenadeCount--;

    // ✅ 크로스헤어 방향 가져오기
    UBGameInstance* GameInstance = Cast<UBGameInstance>(GetWorld()->GetGameInstance());
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ GameInstance를 찾을 수 없습니다!"));
        return;
    }

    UBUIManager* UIManager = GameInstance->GetUIManagerInstance();
    if (!UIManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ UBUIManager를 찾을 수 없습니다!"));
        return;
    }

    // 크로스헤어의 위치와 방향을 가져옵니다
    TTuple<FVector, FVector> CrosshairData = UIManager->GetCrosshairLocationAndDirection();
    FVector CrosshairLocation = CrosshairData.Get<0>();
    FVector CrosshairDirection = CrosshairData.Get<1>();

    UE_LOG(LogTemp, Warning, TEXT("📌 CrosshairLocation: %s"), *CrosshairLocation.ToString());
    UE_LOG(LogTemp, Warning, TEXT("📌 CrosshairDirection: %s"), *CrosshairDirection.ToString());

    // ✅ 손 위치에서 바로 던지기
    FVector SpawnLocation;
    FRotator SpawnRotation;

    // 손 위치에서 던지는 위치를 크로스헤어 방향에 맞게 설정
    if (OwnerCharacter->GetMesh())
    {
        // 손 위치에서 크로스헤어 방향으로 일정 거리만큼 이동
        SpawnLocation = OwnerCharacter->GetMesh()->GetSocketLocation(TEXT("GrenadeSocket")) + CrosshairDirection * 700.0f;  // 크로스헤어 방향으로 이동
        SpawnRotation = OwnerCharacter->GetMesh()->GetSocketRotation(TEXT("GrenadeSocket"));
    }
    else
    {
        // 기본 위치에서 크로스헤어 방향으로 이동
        SpawnLocation = OwnerCharacter->GetActorLocation() + CrosshairDirection * 700.0f;
        SpawnRotation = OwnerCharacter->GetActorRotation();
    }


    UE_LOG(LogTemp, Warning, TEXT("📌 Spawning Grenade at: %s"), *SpawnLocation.ToString());

    AABGrenadeProjectile* Grenade = GetWorld()->SpawnActor<AABGrenadeProjectile>(GrenadeClass, SpawnLocation, SpawnRotation);

    if (!Grenade)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Failed to spawn Grenade!"));
        return;
    }
    if (!Grenade->MeshComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ MeshComponent is NULL!"));
        return;
    }

    // 수류탄의 MeshComponent가 충돌을 감지하도록 설정
    if (Grenade->MeshComponent)
    {
        // 물리 시뮬레이션이 활성화되어야 충돌이 발생함
        Grenade->MeshComponent->SetSimulatePhysics(true);

        // 충돌 처리 활성화
        Grenade->MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        Grenade->MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);  // 모든 채널에 대해 충돌 응답을 블록으로 설정

    }

    // 크로스헤어 방향을 던지는 방향으로 사용
    FVector StartLocation = SpawnLocation;
    FVector EndLocation = StartLocation + CrosshairDirection * 800.f + FVector(0, 0, 150.f); // 크로스헤어 방향으로 던지기

    // 포물선 발사를 위한 속도 예측
    FVector OutVelocity = FVector::ZeroVector;
    if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, OutVelocity, StartLocation, EndLocation, GetWorld()->GetGravityZ(), 1.0f))
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ Predicted outVelocity: %s"), *OutVelocity.ToString());

         // 수류탄의 프로젝타일 무브먼트에 속도 값을 설정
        if (Grenade->ProjectileMovement)
        {
            Grenade->ProjectileMovement->bShouldBounce = true; // 바운스 활성화
            Grenade->ProjectileMovement->bSimulationEnabled = true;
            Grenade->ProjectileMovement->bSweepCollision = true;  // ✅ 충돌 감지 활성화

            // 공기 저항 설정 (Drag 값 조정)
            Grenade->ProjectileMovement->Velocity = Grenade->ProjectileMovement->Velocity * 0.9f;  // 속도 감소
            // 중력 강도를 더 강하게 설정 (1보다 크게 설정하면 더 빨리 떨어짐)
            Grenade->ProjectileMovement->ProjectileGravityScale = 1.7f;  // 중력의 영향을 두 배로 설정
        }
    }

    // 수류탄이 던져진 후 물리 상태 추적
    UE_LOG(LogTemp, Warning, TEXT("📌 Grenade Spawned at: %s, Rotation: %s"), *Grenade->GetActorLocation().ToString(), *Grenade->GetActorRotation().ToString());
    UE_LOG(LogTemp, Warning, TEXT("✅ Grenade thrown with Velocity: %s"), *OutVelocity.ToString());
}