#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/HealthComponent.h"
#include "Characters/Enemies/Turrets.h"

// === Constructor ===
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// === Mesh ===
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (MeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshAsset.Object);
	}

	// === Collision Settings ===
	MeshComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->SetNotifyRigidBodyCollision(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	// === Visibility ===
	MeshComponent->SetVisibility(true);
	MeshComponent->SetHiddenInGame(false);

	// === Movement ===
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	MovementComponent->InitialSpeed = 1000.f;
	MovementComponent->MaxSpeed = 1000.f;
	MovementComponent->ProjectileGravityScale = 0.0f;
}

// === BeginPlay ===
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Ignore owner collision
	if (AActor* MyOwner = GetOwner())
	{
		MeshComponent->IgnoreActorWhenMoving(MyOwner, true);
	}

	// Bind collision events
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);
	MeshComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// === Tick ===
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// === Overlap Event ===
void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner())
	{
		return;
	}

	// Avoid triggering on detection spheres
	if (OtherComp && OtherComp->GetName().Contains("DetectionSphere"))
	{
		return;
	}

	// Flash turret if hit on mesh
	if (ATurrets* Turret = Cast<ATurrets>(OtherActor))
	{
		if (OtherComp && OtherComp->GetName() == "TurretMesh")
		{
			Turret->FlashOnHit();
		}
	}

	// Apply damage if health component exists
	if (UHealthComponent* HealthComp = OtherActor->FindComponentByClass<UHealthComponent>())
	{
		HealthComp->TakeDamage(DamageAmount);
	}

	Destroy();
}

// === Hit Event ===
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == GetOwner())
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Projectile hit actor: %s"), *OtherActor->GetName());

	Destroy();
}

// === Toggle Gravity ===
void AProjectile::SetGravityEnabled(bool bEnableGravity)
{
	if (MovementComponent)
	{
		MovementComponent->ProjectileGravityScale = bEnableGravity ? 0.5f : 0.0f;
	}
}