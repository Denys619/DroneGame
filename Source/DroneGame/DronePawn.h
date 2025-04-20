
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/HealthComponent.h"
#include "DronePawn.generated.h"

class UCameraComponent;
class UFloatingPawnMovement;
class AProjectile;

UCLASS()
class DRONEGAME_API ADronePawn : public APawn
{
	GENERATED_BODY()

public:
	// === Constructor ===
	ADronePawn();

	// === Tick ===
	virtual void Tick(float DeltaTime) override;

	// === Input ===
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// === BeginPlay ===
	virtual void BeginPlay() override;

	// === Movement Input ===
	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveUp(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	// === Combat ===
	UFUNCTION()
	void Shoot();

	// === Health ===
	UFUNCTION()
	void HandleDeath();

protected:
	// === Components ===
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UFloatingPawnMovement* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	// === Shooting ===
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AProjectile> ProjectileClass;
};