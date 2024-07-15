// Copyright BanMing

#pragma once

#include "AbilitySystem/Abilities/GameplayAbility_DamageBase.h"
#include "CoreMinimal.h"

#include "GameplayAbility_BeamSpell.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UGameplayAbility_BeamSpell : public UGameplayAbility_DamageBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();
	
	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	FVector MouseHitLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> MouseHitActor;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;
	
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<ACharacter> OwerCharacter;
};
