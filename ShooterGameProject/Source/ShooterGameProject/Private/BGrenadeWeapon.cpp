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
    GrenadeBody->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f)); // 50% 크기 설정

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
            SetOwnerCharacter(ActiveCharacter);
            UE_LOG(LogTemp, Log, TEXT("Picked up %s and attached to %s"), *GetName(), *ActiveCharacter->GetName());
        }
    }
}
void ABGrenadeWeapon::Attack()
{
    Super::Attack();
    UE_LOG(LogTemp, Warning, TEXT("🔹 Attack() 호출됨"));

    if (!GrenadeClass || !OwnerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GrenadeClass is NULL or OwnerCharacter is NULL!"));
        return;
    }

    // ✅ 수류탄 개수 확인
    if (OwnerCharacter->GrenadeCount <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ 더 이상 수류탄이 없습니다!"));
        return;
    }

    // ✅ 연속 투척 방지
    if (!bCanThrowGrenade)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ 현재 수류탄을 던지는 중입니다!"));
        return;
    }
    bCanThrowGrenade = false; // 던지는 중으로 설정

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

    // ✅ 손 위치에서 던지기
    FVector SpawnLocation;
    FRotator SpawnRotation;

    if (OwnerCharacter->GetMesh())
    {
        SpawnLocation = OwnerCharacter->GetMesh()->GetSocketLocation(TEXT("WeaponSocket")) + CrosshairDirection * 700.0f;
        SpawnRotation = OwnerCharacter->GetMesh()->GetSocketRotation(TEXT("WeaponSocket"));
    }
    else
    {
        SpawnLocation = OwnerCharacter->GetActorLocation() + CrosshairDirection * 700.0f;
        SpawnRotation = OwnerCharacter->GetActorRotation();
    }

    UE_LOG(LogTemp, Warning, TEXT("📌 Spawning Grenade at: %s"), *SpawnLocation.ToString());

    AABGrenadeProjectile* Grenade = GetWorld()->SpawnActor<AABGrenadeProjectile>(GrenadeClass, SpawnLocation, SpawnRotation);
    if (!Grenade)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Failed to spawn Grenade!"));
        bCanThrowGrenade = true; // 실패 시 다시 던질 수 있도록 플래그 복구
        return;
    }

    // ✅ 수류탄 개수 차감
    OwnerCharacter->GrenadeCount--;

    // ✅ 물리 설정
    if (Grenade->MeshComponent)
    {
        Grenade->MeshComponent->SetSimulatePhysics(true);
        Grenade->MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        Grenade->MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
    }

    // ✅ 포물선 발사 속도 설정
    FVector StartLocation = SpawnLocation;
    FVector EndLocation = StartLocation + CrosshairDirection * 3500.f + FVector(0, 0, 150.f);

    FVector OutVelocity = FVector::ZeroVector;
    if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, OutVelocity, StartLocation, EndLocation, GetWorld()->GetGravityZ(), 1.0f))
    {
        if (Grenade->ProjectileMovement)
        {
            Grenade->ProjectileMovement->bShouldBounce = true;
            Grenade->ProjectileMovement->bSimulationEnabled = true;
            Grenade->ProjectileMovement->bSweepCollision = true;
            Grenade->ProjectileMovement->Velocity = OutVelocity;
            Grenade->ProjectileMovement->ProjectileGravityScale = 0.2f;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("📌 Grenade Spawned at: %s, Rotation: %s"), *Grenade->GetActorLocation().ToString(), *Grenade->GetActorRotation().ToString());
    UE_LOG(LogTemp, Warning, TEXT("✅ Grenade thrown with Velocity: %s"), *OutVelocity.ToString());

    // ✅ 일정 시간 후 다시 던질 수 있도록 설정
    GetWorldTimerManager().SetTimer(GrenadeThrowTimerHandle, this, &ABGrenadeWeapon::ResetGrenadeThrow, 4.5f, false);
}

// ✅ 일정 시간 후 다시 던질 수 있도록 초기화하는 함수
void ABGrenadeWeapon::ResetGrenadeThrow()
{
    bCanThrowGrenade = true;
}
