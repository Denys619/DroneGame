#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

// === Delegates ===
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DRONEGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// === Constructor ===
	UHealthComponent();

	// === Health Interface ===
	void TakeDamage(float DamageAmount);
	void Heal(float HealAmount);

	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return Health <= 0.f; }

	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	// === Events ===
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeathDelegate OnDeath;

protected:
	// === Lifecycle ===
	virtual void BeginPlay() override;

protected:
	// === Properties ===
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	float Health = 0.f;
};