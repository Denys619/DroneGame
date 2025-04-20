// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	
}

void UHealthComponent::TakeDamage(float DamageAmount)
{
    if (DamageAmount <= 0.f || Health <= 0.f) return;

    Health -= DamageAmount;

    if (Health <= 0.f)
    {
        Health = 0.f;
        OnDeath.Broadcast();
    }
}

void UHealthComponent::Heal(float HealAmount)
{
    if (HealAmount <= 0.f || Health <= 0.f) return;

    Health = FMath::Clamp(Health + HealAmount, 0.f, MaxHealth);
}