// Copyright BanMing

#pragma once

#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Aura/Character/AuraCharacterBase.h"
#include "Aura/Interaction/EnemyInterface.h"
#include "CoreMinimal.h"
#include "UI/WidgetController/OverlayAuraWidgetController.h"

#include "AuraEnemy.generated.h"

class UBehaviorTree;
class AAuraAIController;
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
	virtual AActor* GetCombatTarget_Implementation() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual ECharacterClass GetCharacterClass_Implementation() const;
	/** end Enemy Interface */

	/** Combat Interface*/
	virtual int32 GetPlayerLevel_Implementation() const override;
	/** end Combat Interface*/

	virtual void PossessedBy(AController* NewController) override;

	virtual void Die(const FVector& DeathImpulse) override;

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

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float Lifespan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	TObjectPtr<AAuraAIController> AIController;
};
