// Copyright BanMing

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AuraPlayerState.generated.h"

/**
 *
 */
class UAbilitySystemComponent;
class UAttributeSet;
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();

	// Inherited via IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
