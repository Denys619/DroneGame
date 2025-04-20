#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class ADronePawn;

/** Type of pickup: either Health or Ammo */
UENUM(BlueprintType)
enum class EPickupType : uint8
{
	Health	UMETA(DisplayName = "Health"),
	Ammo	UMETA(DisplayName = "Ammo")
};

UCLASS()
class DRONEGAME_API APickupBase : public AActor
{
	GENERATED_BODY()
	
public:
	// === Constructor ===
	APickupBase();

protected:
	// === Lifecycle ===
	virtual void BeginPlay() override;

	// === Overlap Logic ===
	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

protected:
	// === Components ===
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	// === Pickup Settings ===
	UPROPERTY(EditAnywhere, Category = "Pickup")
	EPickupType PickupType = EPickupType::Health;

	UPROPERTY(EditAnywhere, Category = "Pickup", meta = (EditCondition = "PickupType == EPickupType::Health", ClampMin = "0.0"))
	float HealAmount = 50.f;

	UPROPERTY(EditAnywhere, Category = "Pickup", meta = (EditCondition = "PickupType == EPickupType::Ammo", ClampMin = "1"))
	int32 AmmoAmount = 3;
};