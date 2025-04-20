#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/HealthComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;

	// Assign a basic sphere mesh from engine content
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (MeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshAsset.Object);
	}

	MeshComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	MeshComponent->SetNotifyRigidBodyCollision(true); // Just in case

	// === Visuals ===
	MeshComponent->SetVisibility(true);
	MeshComponent->SetHiddenInGame(false);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	MovementComponent->InitialSpeed = 1000.f;
	MovementComponent->MaxSpeed = 1000.f;
	MovementComponent->ProjectileGravityScale = 0.0f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind overlap
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                            bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == GetOwner()) return;

    if (OtherComp && OtherComp->GetName().Contains("DetectionSphere"))
    {
        return;
    }

    if (UHealthComponent* HealthComp = OtherActor->FindComponentByClass<UHealthComponent>())
    {
        HealthComp->TakeDamage(DamageAmount);
    }
  

    Destroy();
}
