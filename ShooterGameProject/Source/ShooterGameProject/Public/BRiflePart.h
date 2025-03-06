// BRiflePart.h

#pragma once

#include "CoreMinimal.h"
#include "BBaseItem.h"
#include "BRiflePart.generated.h"

/**
 * 라이플 파츠 클래스
 * 이 클래스는 특정 라이플에 장착할 수 있는 파츠를 나타냅니다.
 * 각 파츠는 라이플의 능력치를 개선하는 기능을 가집니다.
 */
UCLASS()
class SHOOTERGAMEPROJECT_API ABRiflePart : public ABBaseItem
{
	GENERATED_BODY()

public:
	ABRiflePart();
	void UseItem(AActor* Activator) override;
	// 파츠의 매쉬 (각 파츠가 하나의 매쉬를 가짐)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RiflePart")
	UStaticMeshComponent* Mesh;
	// 파츠 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RiflePart")
	FString PartName;

	// 파츠의 효과 - 공격력 증대
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RiflePart")
	float DamageBoost;

	// 파츠의 효과 - 발사 속도 감소
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RiflePart")
	float FireRateBoost;

	// 파츠의 효과 - 정확도 증가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RiflePart")
	int32 MaxAmmoBoost;

};
