// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turrets.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UProjectileMovementComponent;
class AProjectile;

UCLASS()
class DRONEGAME_API ATurrets : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurrets();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ShootAtTarget();

	FTimerHandle ShootingTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* DetectionSphere;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 1.5f;

	AActor* TargetActor;
};
