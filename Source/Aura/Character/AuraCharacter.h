// Copyright BanMing

#pragma once

#include "Aura/Character/AuraCharacterBase.h"
#include "CoreMinimal.h"
#include "Interaction/PlayerInterface.h"

#include "AuraCharacter.generated.h"

/**
 *
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Player Interface*/
	virtual void AddXP_Implementation(int32 InXP) override;
	/** end Player Interface*/

	/** Combat Interface*/
	virtual int32 GetPlayerLevel() const override;
	/** end Combat Interface*/
private:
	virtual void InitAbilityActorInfo() override;
};
