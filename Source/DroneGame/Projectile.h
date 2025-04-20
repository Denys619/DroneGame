#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class DRONEGAME_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UProjectileMovementComponent* GetMovementComponent() const { return MovementComponent; }

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DamageAmount = 25.f;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	               bool bFromSweep, const FHitResult& SweepResult);
};
