// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddXP(int32 InXP);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetXP();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 FindLevelForXP(int32 InXP);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetAttributePointReward(int32 Level) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetSpellPointReward(int32 Level) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetAttributePoints() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetSpellPoints() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddPlayerLevel(int32 InLevel);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddToAttributePoints(int32 InAttributePoints);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddToSpellPoints(int32 InSpellPoints);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LevelUp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideMagicCircle();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMatrial();
};
