// Fill out your copyright notice in the Description page of Project Settings.


#include "Turrets.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "../../Components/HealthComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../Projectile.h"
#include "TimerManager.h"

// Sets default values
ATurrets::ATurrets()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(RootComponent);
	DetectionSphere->SetSphereRadius(2000.f);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

}

// Called when the game starts or when spawned
void ATurrets::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ATurrets::HandleDeath);
	}
	
	TargetActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	GetWorldTimerManager().SetTimer(ShootingTimer, this, &ATurrets::ShootAtTarget, FireRate, true);
}

// Called every frame
void ATurrets::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetActor && DetectionSphere->IsOverlappingActor(TargetActor))
	{

		FVector Direction = TargetActor->GetActorLocation() - GetActorLocation();
		FRotator TargetRotation = Direction.Rotation();

		FRotator CurrentRotation = GetActorRotation();

		float RotationSpeed = 2.0f;
		FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);

		SmoothRotation.Pitch = 0.f;
		SmoothRotation.Roll = 0.f;

		SetActorRotation(SmoothRotation);
	}
}

void ATurrets::ShootAtTarget()
{
	if (!TargetActor || !ProjectileClass) return;

	if (DetectionSphere->IsOverlappingActor(TargetActor))
	{
		FVector SpawnLocation = BaseMesh->GetComponentLocation() + BaseMesh->GetForwardVector() * 100.f + FVector(0.f, 0.f, 200.f);
		FVector Direction = (TargetActor->GetActorLocation() - SpawnLocation).GetSafeNormal();
		FRotator SpawnRotation = Direction.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

		if (Projectile && Projectile->GetMovementComponent())
		{
			Projectile->GetMovementComponent()->Velocity = Direction * 1000.f;
		}
	}
}

void ATurrets::HandleDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Turret has been destroyed!"));
	Destroy();
}