// Copyright BanMing

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AuraPlayerState.generated.h"

#define PLAYER_STAT_ACCESSORS(PropertyName)                            \
	FORCEINLINE int32 GetPlayer##PropertyName() const                  \
	{                                                                  \
		return this->PropertyName;                                     \
	}                                                                  \
                                                                       \
	FORCEINLINE void Set##PropertyName(int32 In##PropertyName)         \
	{                                                                  \
		this->PropertyName = In##PropertyName;                         \
		OnPlayer##PropertyName##Changed.Broadcast(this->PropertyName); \
	}                                                                  \
                                                                       \
	FORCEINLINE void AddTo##PropertyName(int32 In##PropertyName)       \
	{                                                                  \
		this->PropertyName += In##PropertyName;                        \
		OnPlayer##PropertyName##Changed.Broadcast(this->PropertyName); \
	}

/**
 *
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FPlayerStatChanged, int32);

class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;
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

	PLAYER_STAT_ACCESSORS(Level)

	PLAYER_STAT_ACCESSORS(XP)
	
	PLAYER_STAT_ACCESSORS(AttributePoints)
	
	PLAYER_STAT_ACCESSORS(SpellPoints)

	FORCEINLINE ULevelUpInfo* GetLevelUpInfo() const
	{
		return LevelUpInfo;
	}

private:
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);

public:
	FPlayerStatChanged OnPlayerXPChanged;
	FPlayerStatChanged OnPlayerLevelChanged;
	FPlayerStatChanged OnPlayerAttributePointsChanged;
	FPlayerStatChanged OnPlayerSpellPointsChanged;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
	int32 SpellPoints = 1;
};
