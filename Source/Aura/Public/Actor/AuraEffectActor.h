// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AuraEffectActor.generated.h"
class UGameplayEffect;
#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
enum class EEfectApplicationPolicy
{

	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEfectRemovalPolicy
{

	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Gameplay Effect Class")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Gameplay Effect Class")
	EEfectApplicationPolicy InstantEfectApplicationPolicy = EEfectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Gameplay Effect Class")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Gameplay Effect Class")
	EEfectApplicationPolicy DurationEfectApplicationPolicy = EEfectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Gameplay Effect Class")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Gameplay Effect Class")
	EEfectApplicationPolicy InfiniteEfectApplicationPolicy = EEfectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Gameplay Effect Class")
	EEfectRemovalPolicy InfiniteEfectRemovalPolicy = EEfectRemovalPolicy::RemoveOnEndOverlap;
};
