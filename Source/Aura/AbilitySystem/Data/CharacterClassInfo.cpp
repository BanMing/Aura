// Copyright BanMing

#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetCharacterCalssInfo(ECharacterClass CharacterClass) const
{
	return CharacterClassInfoMap[CharacterClass];
}
