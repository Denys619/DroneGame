#include "HealthComponent.h"

// === Constructor ===
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // Disabled unless needed
}

// === BeginPlay ===
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

// === Damage Handling ===
void UHealthComponent::TakeDamage(float DamageAmount)
{
	if (DamageAmount <= 0.f || Health <= 0.f)
	{
		return;
	}

	Health -= DamageAmount;

	if (Health <= 0.f)
	{
		Health = 0.f;
		OnDeath.Broadcast();
	}
}

// === Healing ===
void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.f || Health <= 0.f)
	{
		return;
	}

	Health = FMath::Clamp(Health + HealAmount, 0.f, MaxHealth);
}