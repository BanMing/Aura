// Copyright BanMing

#pragma once

#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Aura/Character/AuraCharacterBase.h"
#include "Aura/Interaction/EnemyInterface.h"
#include "CoreMinimal.h"
#include "UI/WidgetController/OverlayAuraWidgetController.h"

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

	/** Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** end Enemy Interface */

	/** Combat Interface*/
	virtual int32 GetPlayerLevel() const override;
	/** end Combat Interface*/
protected:
	virtual void BeginPlay() override;

private:
	virtual void InitAbilityActorInfo() override;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnMaxHealthChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Elementalist;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> HealthBar;

	UPROPERTY(BlueprintReadOnly, Category = "Cambat")
	bool bHitReacting;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cambat")
	float BaseWalkSpeed = 250.f;
};
