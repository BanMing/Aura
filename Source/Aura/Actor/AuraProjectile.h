// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AuraProjectile.generated.h"

class UPrimitiveComponent;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAuraProjectile();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(
		UPrimitiveComponent* OnComponentBeginOverlap, UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;
};
