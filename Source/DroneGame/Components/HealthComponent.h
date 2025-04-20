// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRONEGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	void TakeDamage(float DamageAmount);
    void Heal(float HealAmount);

    UFUNCTION(BlueprintCallable)
    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable)
    bool IsDead() const { return Health <= 0.f; }

    UPROPERTY(BlueprintAssignable)
    FOnDeathDelegate OnDeath;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category="Health")
    float MaxHealth = 100.f;

    float Health;
		
};
