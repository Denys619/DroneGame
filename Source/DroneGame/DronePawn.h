#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/HealthComponent.h"
#include "Blueprint/UserWidget.h"
#include "DronePawn.generated.h"

class UCameraComponent;
class UFloatingPawnMovement;
class USphereComponent;
class UHealthComponent;
class AProjectile;
class UUserWidget;

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

	// === Reload UI ===
	UFUNCTION(BlueprintCallable)
	float GetReloadProgress() const;

	// === Combat ===
	UFUNCTION(BlueprintCallable)
	void AddAmmo(int32 Amount);

protected:
	// === Lifecycle ===
	virtual void BeginPlay() override;

	// === Movement ===
	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveUp(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	// === Combat ===
	UFUNCTION()
	void Shoot();

	void ResetShoot();

	// === Death ===
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

	// === Combat ===
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 MaxAmmo = 15;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	int32 CurrentAmmo = 0;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireCooldown = 3.f;

	bool bCanShoot = true;
	FTimerHandle FireCooldownTimer;

	// === UI ===
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HealthWidgetClass;

	UPROPERTY()
	UUserWidget* HealthWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ReloadBarClass;

	UPROPERTY()
	UUserWidget* ReloadBarWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> CrosshairClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> VignetteClass;
};