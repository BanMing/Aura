// Copyright BanMing
#include "Actor/MagicCircle.h"

#include "Components/DecalComponent.h"

AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;
	MagicCirleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCirleDecal");
	MagicCirleDecal->SetupAttachment(GetRootComponent());
}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
}

void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MagicCirleDecal->AddLocalRotation(FRotator(0, 0, RotationSpeed * DeltaTime));
}
