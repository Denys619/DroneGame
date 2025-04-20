#include "PickupBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../DronePawn.h"
#include "../Components/HealthComponent.h"

APickupBase::APickupBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Box Collision
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	CollisionComponent->InitBoxExtent(FVector(40.f));
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->SetGenerateOverlapEvents(true);
	RootComponent = CollisionComponent;

	// Mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);

	// Overlap binding
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnOverlap);
}

void APickupBase::BeginPlay()
{
	Super::BeginPlay();
}

void APickupBase::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                            bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (UHealthComponent* Health = OtherActor->FindComponentByClass<UHealthComponent>())
		{
			if (!Health->IsDead())
			{
				Health->Heal(HealAmount);
				Destroy();
			}
		}
	}
}