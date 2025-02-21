#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BBaseWeapon.h"  // 기본 무기 클래스 포함
#include "BCharacter.generated.h"
struct FInputActionValue;
UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	Primary,    // 주무기 (예: 소총)
	Secondary,  // 보조무기 (예: 권총)
	Melee,      // 근접무기 (예: 칼, 도끼)
	Throwable,   // 투척무기 (예: 수류탄)
	Max
};

class UCameraComponent;
class USpringArmComponent; // 스프링 암 클래스 선언
UCLASS()
class SHOOTERGAMEPROJECT_API ABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void Fire(const FInputActionValue& value);
	// 입력 바인딩을 처리할 함수
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	// 🔹 현재 장착된 무기 (각 슬롯에 해당하는 무기 저장)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	// 배열 초기화 예시
	TArray<ABBaseWeapon*> EquippedWeapons;  // 슬롯에 대응하는 무기 배열
	// 📌 카메라 컴포넌트 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;
	// 📌 스프링 암 (카메라를 부드럽게 따라가도록 설정)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComponent;
	// 🔹 현재 사용 중인 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponSlot ActiveWeaponSlot;

	// 캐릭터 클래스에 있는 변수 (예제)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ABBaseWeapon> WeaponClass;
	// 🔹 무기 장착 함수
	void EquipWeapon(ABBaseWeapon* NewWeapon);
	/** 캐릭터의 카메라가 바라보는 방향을 가져오는 함수 */
	FVector GetCameraForwardVector() const;
	// 🔹 무기 변경 함수 (예: 1번 키: 주무기, 2번 키: 보조무기)
	void SwitchWeapon(EWeaponSlot NewSlot);
	
};
