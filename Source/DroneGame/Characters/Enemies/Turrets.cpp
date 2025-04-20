#include "Turrets.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "../../Components/HealthComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../Projectile.h"
#include "TimerManager.h"

// === Constructor ===
ATurrets::ATurrets()
{
	PrimaryActorTick.bCanEverTick = true;

	// === Components Setup ===
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(RootComponent);
	DetectionSphere->SetSphereRadius(2000.f);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// === BeginPlay ===
void ATurrets::BeginPlay()
{
	Super::BeginPlay();

	// === Death Binding ===
	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ATurrets::HandleDeath);
	}

	// === Dynamic Material Setup ===
	if (UStaticMeshComponent* FoundMesh = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("TurretMesh"))))
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(FoundMesh->GetMaterial(0), this);
		FoundMesh->SetMaterial(0, DynamicMaterial);
		OriginalColor = DynamicMaterial->K2_GetVectorParameterValue(FName("BaseColor"));
	}

	// === Get Target Player ===
	TargetActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// === Setup Shooting Timer ===
	GetWorldTimerManager().SetTimer(ShootingTimer, this, &ATurrets::ShootAtTarget, FireRate, true);
}

// === Tick ===
void ATurrets::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// === Rotate Toward Target ===
	if (TargetActor && DetectionSphere->IsOverlappingActor(TargetActor))
	{
		FVector Direction = TargetActor->GetActorLocation() - GetActorLocation();
		FRotator TargetRotation = Direction.Rotation();

		FRotator SmoothRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 2.0f);
		SmoothRotation.Pitch = 0.f;
		SmoothRotation.Roll = 0.f;

		SetActorRotation(SmoothRotation);
	}
}

// === Shooting Logic ===
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

// === Flash Feedback on Hit ===
void ATurrets::FlashOnHit()
{
	if (!DynamicMaterial) return;

	DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor::Red);

	GetWorldTimerManager().SetTimer(FlashTimer, [this]()
	{
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), OriginalColor);
		}
	}, 0.15f, false);
}

// === Handle Death ===
void ATurrets::HandleDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Turret has been destroyed!"));
	Destroy();
}