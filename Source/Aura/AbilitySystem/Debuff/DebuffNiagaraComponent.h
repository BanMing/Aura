// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"

#include "DebuffNiagaraComponent.generated.h"

/**
 *
 */
UCLASS(ClassGroup = (Rendering, Common), Blueprintable, hidecategories = Object, hidecategories = Physics, hidecategories = Collision, showcategories = Trigger, editinlinenew,
	meta = (BlueprintSpawnableComponent, DisplayName = "Debuff Niagara System Component"))
class AURA_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	UDebuffNiagaraComponent();
	virtual void BeginPlay() override;

protected:
	void OnDebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DebuffTag;
};
