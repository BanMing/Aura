// Copyright BanMing

#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		if (LevelUpInfos.Num() - 1 <= Level)
		{
			return Level;
		}

		if (XP >= LevelUpInfos[Level].LevelUpRequirement)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}

	return Level;
}

#if WITH_EDITORONLY_DATA
void ULevelUpInfo::Serialize(FStructuredArchiveRecord Record)
{
	//if (HasAnyFlags(RF_LoadCompleted))
	//{
	//	const int32 MaxLevel = 60;
	//	LevelUpInfos.Empty();
	//	LevelUpInfos.Add(FAuraLevelUpInfo());
	//	for (int32 i = 1; i < MaxLevel; i++)
	//	{
	//		FAuraLevelUpInfo Info;
	//		Info.LevelUpRequirement = pow(3, i) * 100;
	//		LevelUpInfos.Add(Info);
	//	}
	//}
	Super::Serialize(Record);
}
#endif