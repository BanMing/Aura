// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get()
	{
		return GameplayTags;
	}

	static void InitNativeGameplayTags();

public:
	FGameplayTag Attributes_Secondary_Armor;

private:
	static FAuraGameplayTags GameplayTags;
};