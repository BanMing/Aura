// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"

#include "AuraPlayerController.generated.h"

class USplineComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraAbilitySystemComponent;
/**
 *
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	void PlayerTick(float DeltaTime) override;
	const FHitResult& GetCursorHit() const;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamgaAmount, ACharacter* TargetCharacter, bool bIsBlockedHit, bool bIsCriticalHit);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void ShiftPressed()
	{
		bShiftKeyDown = true;
	}
	void ShiftReleased()
	{
		bShiftKeyDown = false;
	}

	void CursorTrace();

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	void Move(const FInputActionValue& InputActionValue);
	void AutoRun();
	void MoveClickedMove();
	void MouseHeldMove();

	UAuraAbilitySystemComponent* GetASC();

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UDamageTextComponent> DamageTextComponentClass;

private:
	FHitResult CursorHit;
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;

private:
	// Click to move
	FVector CacheDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	bool bShiftKeyDown = false;
};
