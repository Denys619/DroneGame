// Fill out your copyright notice in the Description page of Project Settings.


#include "Turrets.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
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
	DetectionSphere->SetSphereRadius(1200.f);

}

// Called when the game starts or when spawned
void ATurrets::BeginPlay()
{
	Super::BeginPlay();
	
	TargetActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	GetWorldTimerManager().SetTimer(ShootingTimer, this, &ATurrets::ShootAtTarget, FireRate, true);
}

// Called every frame
void ATurrets::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetActor && DetectionSphere->IsOverlappingActor(TargetActor))
	{
		FRotator LookAtRotation = (TargetActor->GetActorLocation() - GetActorLocation()).Rotation();
		SetActorRotation(FRotator(0.f, LookAtRotation.Yaw, 0.f));
	}
}

void ATurrets::ShootAtTarget()
{
	if (!TargetActor || !ProjectileClass) return;

	if (DetectionSphere->IsOverlappingActor(TargetActor))
	{
		FVector SpawnLocation = BaseMesh->GetComponentLocation() + BaseMesh->GetForwardVector() * 10.f;
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