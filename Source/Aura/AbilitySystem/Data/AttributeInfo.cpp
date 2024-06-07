// Copyright BanMing

#include "Aura/AbilitySystem/Data/AttributeInfo.h"

#include "AuraLogChannels.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& Tag, bool bLogNotFound /*= false*/) const
{
	for (const FAuraAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(Tag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Does not find %s tag in asset table"), *Tag.ToString());
	}

	return FAuraAttributeInfo();
}
