#pragma once

#include "CoreMinimal.h"
#include "BEnemyBase.h"
#include "BAssassinEnemy.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API ABAssassinEnemy : public ABEnemyBase
{
	GENERATED_BODY()
	
public:
	ABAssassinEnemy();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Properties|Assassin")
	bool bIsStealthed;

	void BeginPlay() override;
	void UseSkill() override;
	void EndSkill();
	virtual FName GetMonsterType() const override;
};
