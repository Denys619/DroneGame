#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Turrets.generated.h"

// === Forward Declarations ===
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
	// === Constructor ===
	ATurrets();

	// === Visual Feedback ===
	void FlashOnHit();

protected:
	// === Lifecycle ===
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// === Combat ===
	void ShootAtTarget();

	// === Damage Response ===
	UFUNCTION()
	void HandleDeath();

private:
	// === Components ===
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* DetectionSphere;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UHealthComponent* HealthComponent;

	// === Combat Settings ===
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 1.5f;

	// === Target ===
	AActor* TargetActor;

	// === Timers ===
	FTimerHandle ShootingTimer;
	FTimerHandle FlashTimer;

	// === Material Feedback ===
	UMaterialInstanceDynamic* DynamicMaterial = nullptr;

	UPROPERTY()
	FLinearColor OriginalColor = FLinearColor::White;
};