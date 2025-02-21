#pragma once

#include "CoreMinimal.h"
#include "BEnemyBase.h"
#include "BBasicEnemy.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API ABBasicEnemy : public ABEnemyBase
{
	GENERATED_BODY()
	
public:
	ABBasicEnemy();
	float GetAttackRange() const override;
};
