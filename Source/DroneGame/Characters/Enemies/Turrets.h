// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turrets.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UProjectileMovementComponent;
class AProjectile;
class UHealthComponent;


UCLASS()
class DRONEGAME_API ATurrets : public AActor
{
	GENERATED_BODY()
	
public:	
	ATurrets();

	// === Flash On Hit ===
	void FlashOnHit();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void ShootAtTarget();

	UFUNCTION()
	void HandleDeath();

	FTimerHandle ShootingTimer;

	private:
	UMaterialInstanceDynamic* DynamicMaterial = nullptr;
	FTimerHandle FlashTimer;

	FLinearColor OriginalColor = FLinearColor::White;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* DetectionSphere;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 1.5f;

	AActor* TargetActor;
};
