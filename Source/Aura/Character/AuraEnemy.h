// Copyright BanMing

#pragma once

#include "Aura/Character/AuraCharacterBase.h"
#include "Aura/Interaction/EnemyInterface.h"
#include "CoreMinimal.h"

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

	/** Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** end Enemy Interface */

	/** Combat Interface*/
	virtual int32 GetPlayerLevel() const override;
	/** end Combat Interface*/

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
};
