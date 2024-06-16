// Copyright BanMing

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AuraPlayerState.generated.h"

/**
 *
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FPlayerStatChanged, float);

class UAbilitySystemComponent;
class UAttributeSet;
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Inherited via IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;

	FORCEINLINE int32 GetPLayerLevel() const
	{
		return Level;
	}
	
	FORCEINLINE void SetLevel(int32 InLevel)
	{
		Level = InLevel;
		OnPlayerLevelChanged.Broadcast(XP);
	}
	
	FORCEINLINE void AddToLevel(int32 InLevel)
	{
		Level += InLevel;
		OnPlayerLevelChanged.Broadcast(Level);
	}

	FORCEINLINE int32 GetPLayerXP() const
	{
		return XP;
	}

	FORCEINLINE void SetXP(int32 InXP)
	{
		XP = InXP;
		OnPlayerXPChanged.Broadcast(XP);
	}

	FORCEINLINE void AddToXP(int32 InXP)
	{
		XP += InXP;
		OnPlayerXPChanged.Broadcast(XP);
	}

private:
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

public:
	FPlayerStatChanged OnPlayerXPChanged;
	FPlayerStatChanged OnPlayerLevelChanged;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	int32 XP = 0;
};
