// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MagicCircle.generated.h"

class UDecalComponent;

UCLASS() class AURA_API AMagicCircle : public AActor
{
	GENERATED_BODY()

public:
	AMagicCircle();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> MagicCirleDecal;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RotationSpeed = 8;
};
