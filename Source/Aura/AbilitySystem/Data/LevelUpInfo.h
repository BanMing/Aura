// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward = 1;
};

/**
 *
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	int32 FindLevelForXP(int32 XP);
#if WITH_EDITORONLY_DATA
	virtual void Serialize(FStructuredArchiveRecord Record) override;
#endif
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInfos;
};
