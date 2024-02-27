// Copyright BanMing

#pragma once

#include "Aura/Character/AuraCharacterBase.h"
#include "CoreMinimal.h"

#include "AuraCharacter.generated.h"

/**
 *
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Combat Interface*/
	virtual int32 GetPlayerLevel() const override;
	/** end Combat Interface*/
private:
	virtual void InitAbilityActorInfo() override;
};
