// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"

#include "PassiveNiagaraComponent.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	UPassiveNiagaraComponent();
	virtual void BeginPlay() override;

protected:
	void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveTag;
};
