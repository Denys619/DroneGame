#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

// === Forward Declarations ===
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class DRONEGAME_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// === Constructor ===
	AProjectile();

	// === Tick ===
	virtual void Tick(float DeltaTime) override;

	// === Gravity Control ===
	void SetGravityEnabled(bool bEnableGravity);

	// === Accessors ===
	FORCEINLINE UProjectileMovementComponent* GetMovementComponent() const { return MovementComponent; }

protected:
	// === Lifecycle ===
	virtual void BeginPlay() override;

	// === Collision Events ===
	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

protected:
	// === Components ===
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* MovementComponent;

	// === Combat ===
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DamageAmount = 25.f;
};