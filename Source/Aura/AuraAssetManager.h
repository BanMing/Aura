// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"

#include "AuraAssetManager.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	UAuraAssetManager& Get() const;

protected:
	virtual void StartInitialLoading() override;
};