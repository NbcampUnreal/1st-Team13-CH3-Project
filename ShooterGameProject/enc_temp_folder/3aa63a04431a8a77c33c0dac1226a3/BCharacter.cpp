#include "BCharacter.h"
#include "BPlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"


ABCharacter::ABCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    ActiveWeaponSlot = EWeaponSlot::Primary;  // 기본적으로 주무기를 활성화
    // 배열의 크기를 ActiveWeaponSlot에 맞게 확장
    EquippedWeapons.SetNumZeroed(4);  // ActiveWeaponSlot에 맞게 배열 크기 설정
    // 📌 스프링 암 추가 (카메라가 캐릭터를 따라가도록 설정)
    USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 300.0f; // 카메라와 캐릭터 사이 거리 설정
    SpringArm->bUsePawnControlRotation = true; // 카메라가 마우스를 따라 회전하도록 설정

    // 📌 카메라 추가
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArm);
    CameraComponent->bUsePawnControlRotation = false; // 카메라는 스프링 암을 따라가도록 설정
}

void ABCharacter::BeginPlay()
{
    Super::BeginPlay();

    // WeaponClass가 유효한지 확인
    if (WeaponClass)
    {
        // 무기 스폰
        ABBaseWeapon* NewWeapon = GetWorld()->SpawnActor<ABBaseWeapon>(WeaponClass);

        if (NewWeapon)
        {
            // 캐릭터의 손 위치에 있는 소켓에 부착
            USkeletalMeshComponent* CharacterMesh = GetMesh();
            if (CharacterMesh)
            {
                FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
                // 무기가 부착된 후 로그 추가
                // 무기 장착 후 EquippedWeapons에 추가
                EquipWeapon(NewWeapon);
                UE_LOG(LogTemp, Log, TEXT("Weapon successfully attached to WeaponSocket!"));
                // 렌더링이 켜져 있는지 확인
                NewWeapon->SetActorEnableCollision(true);
                NewWeapon->SetActorHiddenInGame(false); // 총기 메시 보이게 하기
            }

            UE_LOG(LogTemp, Log, TEXT("무기 장착 완료!"));
        }
    }
}



void ABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent로 캐스팅
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA를 가져오기 위해 현재 소유 중인 Controller를 ASpartaPlayerController로 캐스팅
        if (ABPlayerController* PlayerController = Cast<ABPlayerController>(GetController()))
        {
            if (PlayerController->LookAction)
            {
                // IA_Look 액션 마우스가 "움직일 때" Look() 호출
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ABCharacter::Look
                );
            }
            if (PlayerController->FireAction)
            {
                // IA_Look 액션 마우스가 "움직일 때" Look() 호출
                EnhancedInput->BindAction(
                    PlayerController->FireAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ABCharacter::Fire
                );
                EnhancedInput->BindAction(
                    PlayerController->FireAction, 
                    ETriggerEvent::Completed, 
                    this, 
                    &ABCharacter::StopFire
                );

            }
        }
    }
}
void ABCharacter::Look(const FInputActionValue& value)
{
    FVector2D LookInput = value.Get<FVector2D>();

    // X는 좌우 회전 (Yaw), Y는 상하 회전 (Pitch)
    AddControllerYawInput(LookInput.X);  // 좌우 회전
    AddControllerPitchInput(LookInput.Y);  // 상하 회전
}

void ABCharacter::Fire(const FInputActionValue& value)
{
    ABBaseWeapon* CurrentWeapon = EquippedWeapons[(int32)ActiveWeaponSlot];
    if (!CurrentWeapon) return;

    if (CurrentWeapon->WeaponType == "Pistol")
    {
        // 권총: 한 번 클릭하면 한 발 발사
        CurrentWeapon->Attack();
    }
    else if (CurrentWeapon->WeaponType == "Rifle")
    {
        // 라이플: FireRate 간격으로 자동 발사
        GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ABCharacter::FireOnce, CurrentWeapon->FireRate, true);
    }
}

void ABCharacter::FireOnce()
{
    ABBaseWeapon* CurrentWeapon = EquippedWeapons[(int32)ActiveWeaponSlot];
    if (CurrentWeapon)
    {
        CurrentWeapon->Attack();
    }
}

void ABCharacter::StopFire()
{
    UE_LOG(LogTemp, Log, TEXT("StopFire"));
    // 발사 멈추기
    GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}
// 무기 부착 함수
void ABCharacter::EquipWeapon(ABBaseWeapon* NewWeapon)
{
    if (NewWeapon)
    {
        // 1. 캐릭터의 손 위치에 있는 소켓에 무기 부착
        USkeletalMeshComponent* CharacterMesh = GetMesh();
        if (CharacterMesh)
        {
            // 🔹 무기 부착 (손 소켓에 장착)
            FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
            NewWeapon->AttachToComponent(GetMesh(), AttachRules, TEXT("WeaponSocket"));
            UE_LOG(LogTemp, Log, TEXT("WeaponType : %s"),*NewWeapon->WeaponType);  // ✅ 정상 동작
            if (NewWeapon->WeaponType == "Rifle") {
                // 🔹 상대 회전을 조정하여 총구가 앞쪽을 향하도록 설정
                FRotator AdjustedRotation(0.0f, -180.0f, 0.0f);  // Yaw 값(90도) 조정
                NewWeapon->SetActorRelativeRotation(AdjustedRotation);
            }
            if (NewWeapon->WeaponType == "Pistol") {
                // 🔹 상대 회전을 조정하여 총구가 앞쪽을 향하도록 설정
                FRotator AdjustedRotation(0.0f, 90.0f, 90.0f); // 예: Y축으로 90도 회전
                NewWeapon->SetActorRelativeRotation(AdjustedRotation);
            }
            // 🔹 무기에 캐릭터 정보 설정
            NewWeapon->SetOwnerCharacter(this);
            // 2. 무기 장착 후, 현재 활성화된 무기 슬롯에 무기 추가
            EquippedWeapons[(int32)EWeaponSlot::Primary] = NewWeapon;  // EWeaponSlot을 int32로 캐스팅하여 사용

            // 3. 무기 활성화 및 기타 설정 (옵션)
            NewWeapon->SetActorEnableCollision(true);  // 충돌 활성화
            NewWeapon->SetActorHiddenInGame(false);    // 게임에서 보이게 설정

            UE_LOG(LogTemp, Log, TEXT("Weapon successfully equipped and attached to WeaponSocket!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to equip weapon. NewWeapon is null."));
    }
}
FVector ABCharacter::GetCameraForwardVector() const
{
    if (CameraComponent)
    {
        return CameraComponent->GetForwardVector();
    }

    return GetActorForwardVector();  // 카메라가 없으면 기본 방향 반환
}
void ABCharacter::SwitchWeapon(EWeaponSlot NewSlot)
{
    int32 SlotIndex = (int32)NewSlot;

    if (EquippedWeapons.IsValidIndex(SlotIndex) && EquippedWeapons[SlotIndex] != nullptr)
    {
        ActiveWeaponSlot = NewSlot;
        UE_LOG(LogTemp, Log, TEXT("무기 변경: %d"), SlotIndex);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("해당 슬롯에 무기가 없습니다!"));
    }
}
