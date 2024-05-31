// Copyright BanMing

#include "AbilitySystem/Abilities/GameplayAbility_Summon.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UGameplayAbility_Summon::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	const float DeltaSpread = SpawnSpread / NumMinions;
	TArray<FVector> SpawnLocations;
	for (int i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		const FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		// Check the location if is the floor
		FHitResult Hit;
		const FVector ZOffsetVec = FVector(0.f, 0.f, 400.f);
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + ZOffsetVec, ChosenSpawnLocation - ZOffsetVec, ECollisionChannel::ECC_WorldStatic);
		if (Hit.bBlockingHit)
		{
			SpawnLocations.Add(Hit.ImpactPoint);
		}
		// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + Direction * MaxSpawnDistance, 4.f, FColor::Green, 3);
		// DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 15.f, 12, FColor::Red, false, 3.f);
		// DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 5.f, 12, FColor::Blue, false, 3.f);
		// DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 5.f, 12, FColor::Blue, false, 3.f);
	}

	// const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);

	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + LeftOfSpread * MaxSpawnDistance, 4.f, FColor::Green, 3);
	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + RightOfSpread * MaxSpawnDistance, 4.f, FColor::Red, 3);

	// DrawDebugSphere(GetWorld(), Location + RightOfSpread * MinSpawnDistance, 15.f, 12, FColor::Blue, false, 3.f);
	// DrawDebugSphere(GetWorld(), Location + RightOfSpread * MaxSpawnDistance, 15.f, 12, FColor::Blue, false, 3.f);
	//
	// DrawDebugSphere(GetWorld(), Location + LeftOfSpread * MinSpawnDistance, 15.f, 12, FColor::Blue, false, 3.f);
	// DrawDebugSphere(GetWorld(), Location + LeftOfSpread * MaxSpawnDistance, 15.f, 12, FColor::Blue, false, 3.f);

	return SpawnLocations;
}

TSubclassOf<APawn> UGameplayAbility_Summon::GetRandomMinionClass()
{
	const int index = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[index];
}
