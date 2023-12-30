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
public:
	AAuraEnemy();

public:
	// Inherited via IEnemyInterface
	void HighlightActor() override;
	void UnHighlightActor() override;
};
