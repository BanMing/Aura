// Copyright BanMing

#include "Aura/Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNoFound /*= false*/) const
{
	for (const FAuraInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNoFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilitInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetName());
	}
	return nullptr;
}
