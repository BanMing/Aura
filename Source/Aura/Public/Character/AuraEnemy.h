// Copyright BanMing

#pragma once

#include "Character/AuraCharacterBase.h"
#include "CoreMinimal.h"
#include "Interaction/EnemyInterface.h"

#include "AuraEnemy.generated.h"

/**
 *
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

private:
	virtual void InitAbilityActorInfo() override;

public:
	AAuraEnemy();
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
};
