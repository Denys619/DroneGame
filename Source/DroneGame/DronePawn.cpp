#include "DronePawn.h"
#include "Projectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// === Constructor ===
ADronePawn::ADronePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// --- Collision root ---
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    CollisionComponent->InitSphereRadius(45.f);
    CollisionComponent->SetCollisionProfileName(TEXT("Pawn"));
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECC_Pawn);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    RootComponent = CollisionComponent;

	// Components
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	// Auto Possess
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Rotation
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = false;
}

// === BeginPlay ===
void ADronePawn::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ADronePawn::HandleDeath);
	}
}

// === Tick ===
void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// === Input Bindings ===
void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADronePawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADronePawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ADronePawn::MoveUp);

	PlayerInputComponent->BindAxis("Turn", this, &ADronePawn::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ADronePawn::LookUp);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ADronePawn::Shoot);
}

// === Movement ===
void ADronePawn::MoveForward(float Value)
{
	if (Controller && Value != 0.f)
	{
		FRotator ControlRotation = Controller->GetControlRotation();
		ControlRotation.Pitch = 0.f;
		FVector Direction = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ADronePawn::MoveRight(float Value)
{
	if (Controller && Value != 0.f)
	{
		FRotator ControlRotation = Controller->GetControlRotation();
		ControlRotation.Pitch = 0.f;
		FVector Direction = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ADronePawn::MoveUp(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(FVector::UpVector, Value);
	}
}

void ADronePawn::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ADronePawn::LookUp(float Value)
{
	if (Controller)
	{
		FRotator Rotation = Controller->GetControlRotation();
		float CurrentPitch = FRotator::NormalizeAxis(Rotation.Pitch);
		float NewPitch = FMath::Clamp(CurrentPitch + Value, -80.f, -30.f);
		Rotation.Pitch = NewPitch;
		Controller->SetControlRotation(Rotation);
	}
}

// === Shooting ===
void ADronePawn::Shoot()
{
	if (!ProjectileClass) return;

	FVector SpawnLocation = CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * 100.0f;
	FRotator SpawnRotation = CameraComponent->GetComponentRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
}

// === Death Handling ===
void ADronePawn::HandleDeath()
{
	// TODO: додати візуальний ефект, звук, анімацію (опціонально)
	Destroy();
}