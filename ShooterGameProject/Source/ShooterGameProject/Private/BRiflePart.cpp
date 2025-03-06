// BRiflePart.cpp
#include "BRiflePart.h"
#include "BPlayerState.h"
#include "BRifle.h"


ABRiflePart::ABRiflePart()
{
    // 파츠의 매쉬 설정 (에디터에서 설정할 수도 있고, 코드로 설정할 수도 있음)
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PartMesh"));
    Mesh->SetupAttachment(RootComponent); // 부모 컴포넌트에 첨부
	// 기본 파츠 효과 초기화
	PartName = "Rifle Part"; // 기본 파츠 이름
	DamageBoost = 3.0f;        // 기본 공격력 증가
	FireRateBoost = 0.1f;      // 기본 발사 속도 감소
	MaxAmmoBoost = 5;     // 최대탄창 증가
	ItemPrice = 1000;
	// FItemData 초기화
	FItemData NewItemData;
	NewItemData.ItemName = TEXT("RiflePart");
	NewItemData.ItemClass = ABRiflePart::StaticClass();
	NewItemData.ItemCount = 1;  // 기본적으로 1개씩 추가
	NewItemData.ItemIcon = nullptr;  // 필요 시 아이콘 설정
	NewItemData.ItemRef = this;

	// SetItemData를 통해 ItemData 설정
	SetItemData(NewItemData);
}

// 라이플에 파츠를 적용하는 함수
void ABRiflePart::UseItem(AActor* Activator)
{
    if (!Activator)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ Activator가 없습니다"));
        return;
    }

    ABCharacter* Character = Cast<ABCharacter>(Activator);
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ Activator가 ABCharacter가 아닙니다!"));
        return;
    }

    // 플레이어 상태 가져오기
    ABPlayerState* PlayerState = Character->GetPlayerState<ABPlayerState>();
    if (!PlayerState)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ PlayerState를 찾을 수 없습니다!"));
        return;
    }

    // 라이플을 장착하고 있는지 확인
    ABRifle* EquippedRifle = Cast<ABRifle>(Character->EquippedWeapon);
    if (!EquippedRifle)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ 라이플이 장착되어 있지 않습니다!"));
        return;
    }

    // 라이플에 이미 파츠가 장착되어 있는지 확인
    if (EquippedRifle->IsPartMeshEquipped(this))
    {
        // 파츠의 효과만 라이플에 적용
        EquippedRifle->Damage += DamageBoost;
        EquippedRifle->FireRate -= FireRateBoost;  // 발사 속도는 감소하는 방향으로 적용
        EquippedRifle->MaxAmmo += MaxAmmoBoost;
        // 파츠가 라이플에 장착되었음을 알림
        UE_LOG(LogTemp, Log, TEXT("%s 파츠가 추가로 장착되었습니다! 공격력: %.1f, 발사 속도: %.1f, 최대 탄약: %d"),
            *PartName, EquippedRifle->Damage, EquippedRifle->FireRate, EquippedRifle->MaxAmmo);
    }
    else
    {
        // 파츠의 매쉬를 라이플에 장착
        if (Mesh)
        {
            // 라이플에 이미 매쉬가 장착되어 있는지 확인
            if (EquippedRifle->EquippedPartMesh == nullptr)
            {
                // 새로운 StaticMeshComponent를 라이플에 부착
                EquippedRifle->EquippedPartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquippedPartMesh"));
                EquippedRifle->EquippedPartMesh->SetupAttachment(EquippedRifle->RifleBody);  // RifleBody에 부착
                EquippedRifle->EquippedPartMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));  // 위치 조정

                // 파츠 매쉬 설정 (Mesh는 UStaticMeshComponent* 타입이므로, Mesh->GetStaticMesh()로 UStaticMesh를 얻음)
                EquippedRifle->EquippedPartMesh->SetStaticMesh(Mesh->GetStaticMesh());  // Mesh에서 StaticMesh를 얻어서 설정
                EquippedRifle->EquippedPartMesh->RegisterComponent();  // 컴포넌트 등록

                // 파츠가 장착되었다는 로그
                UE_LOG(LogTemp, Log, TEXT("%s 파츠의 매쉬가 라이플에 처음 장착되었습니다."), *PartName);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("%s 파츠의 매쉬는 이미 장착되어 있습니다."), *PartName);
            }

            // 파츠의 효과를 라이플에 적용 (중복 적용)
            EquippedRifle->Damage += DamageBoost;
            EquippedRifle->FireRate -= FireRateBoost; // 발사 속도 감소
            EquippedRifle->MaxAmmo += MaxAmmoBoost;

            // 파츠가 라이플에 장착되었음을 알림
            UE_LOG(LogTemp, Log, TEXT("%s 파츠가 장착되었습니다! 공격력: %.1f, 발사 속도: %.1f, 최대 탄약: %d"),
                *PartName, EquippedRifle->Damage, EquippedRifle->FireRate, EquippedRifle->MaxAmmo);
        }


        // 인벤토리에서 파츠 아이템 제거
        PlayerState->InventoryRemoveItem(ItemData);

        // 남은 파츠 개수 출력
        TArray<FItemData> PartItems = PlayerState->GetInventoryTypeItem(ItemData.ItemName);
        int32 RemainingPartCount = 0;

        for (const FItemData& Item : PartItems)
        {
            RemainingPartCount += Item.ItemCount;
        }

        UE_LOG(LogTemp, Log, TEXT("%s 파츠, 남은 개수: %d개"), *ItemData.ItemName.ToString(), RemainingPartCount);
    }
}