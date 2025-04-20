
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/HealthComponent.h"
#include "Blueprint/UserWidget.h"
#include "DronePawn.generated.h"

class UCameraComponent;
class UFloatingPawnMovement;
class AProjectile;
class USphereComponent; 

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

	UFUNCTION(BlueprintCallable)
    float GetReloadProgress() const;

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

	void ResetShoot();

	// === Health ===
	UFUNCTION()
	void HandleDeath();

protected:
	// === Components ===
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UFloatingPawnMovement* MovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HealthWidgetClass;

	UPROPERTY()
	UUserWidget* HealthWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ReloadBarClass;

	UUserWidget* ReloadBarWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> CrosshairClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> VignetteClass;

	// === Shooting ===
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 MaxAmmo = 15;
	int32 CurrentAmmo = 0;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireCooldown = 3.f;

	bool bCanShoot = true;
	FTimerHandle FireCooldownTimer;
};