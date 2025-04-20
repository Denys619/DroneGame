#include "PickupBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../DronePawn.h"
#include "../Components/HealthComponent.h"

// === Constructor ===
APickupBase::APickupBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// === Collision ===
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	CollisionComponent->InitBoxExtent(FVector(40.f));
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->SetGenerateOverlapEvents(true);
	RootComponent = CollisionComponent;

	// === Mesh ===
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);

	// === Bind Overlap Event ===
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnOverlap);
}

// === BeginPlay ===
void APickupBase::BeginPlay()
{
	Super::BeginPlay();
}

// === Overlap Handler ===
void APickupBase::OnOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	switch (PickupType)
	{
		case EPickupType::Health:
		{
			if (UHealthComponent* Health = OtherActor->FindComponentByClass<UHealthComponent>())
			{
				if (!Health->IsDead())
				{
					Health->Heal(HealAmount);
					Destroy();
				}
			}
			break;
		}

		case EPickupType::Ammo:
		{
			if (ADronePawn* Drone = Cast<ADronePawn>(OtherActor))
			{
				Drone->AddAmmo(AmmoAmount);
				Destroy();
			}
			break;
		}

		default:
			break;
	}
}