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
	virtual int32 FindLevelForXP_Implementation(int32 InXP) override;
	virtual int32 GetXP_Implementation() override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetAttributePointReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointReward_Implementation(int32 Level) const override;
	virtual void AddPlayerLevel_Implementation(int32 InLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual int32 GetAttributePoints_Implementation() const;
	virtual int32 GetSpellPoints_Implementation() const;
	/** end Player Interface*/

	/** Combat Interface*/
	virtual int32 GetPlayerLevel_Implementation() const override;
	/** end Combat Interface*/
protected:
	virtual void OnRep_Stunned() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UNiagaraComponent> LevelUpNiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USpringArmComponent> CameraBoom;

private:
	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticsatLevelUpParticles() const;
};
