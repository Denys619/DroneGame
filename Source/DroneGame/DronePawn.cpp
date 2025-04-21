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

	// === Collision ===
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComponent->InitSphereRadius(45.f);
	CollisionComponent->SetCollisionProfileName(TEXT("Pawn"));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECC_Pawn);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	RootComponent = CollisionComponent;

	// === Components ===
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	// === Movement Component ===
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->MaxSpeed = 1200.f;
	MovementComponent->Acceleration = 2200.f;
	MovementComponent->Deceleration = 2500.f;

	// === Possession ===
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// === Rotation Settings ===
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = false;
}

// === BeginPlay ===
void ADronePawn::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ADronePawn::HandleDeath);
	}

	// === UI Setup ===
	if (HealthWidgetClass)
	{
		HealthWidget = CreateWidget<UUserWidget>(GetWorld(), HealthWidgetClass);
		if (HealthWidget) HealthWidget->AddToViewport();
	}

	if (ReloadBarClass)
	{
		ReloadBarWidget = CreateWidget<UUserWidget>(GetWorld(), ReloadBarClass);
		if (ReloadBarWidget) ReloadBarWidget->AddToViewport();
	}

	if (CrosshairClass)
	{
		UUserWidget* CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairClass);
		if (CrosshairWidget) CrosshairWidget->AddToViewport();
	}

	if (VignetteClass)
	{
		UUserWidget* VignetteWidget = CreateWidget<UUserWidget>(GetWorld(), VignetteClass);
		if (VignetteWidget) VignetteWidget->AddToViewport();
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

// === Combat ===
void ADronePawn::Shoot()
{
	if (!ProjectileClass || !bCanShoot || CurrentAmmo <= 0) return;

	FVector SpawnLocation = CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * 100.f;
	FRotator SpawnRotation = CameraComponent->GetComponentRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (Projectile)
	{
		Projectile->SetGravityEnabled(true);
	}

	--CurrentAmmo;
	bCanShoot = false;

	GetWorldTimerManager().SetTimer(FireCooldownTimer, this, &ADronePawn::ResetShoot, FireCooldown, false);
}

void ADronePawn::ResetShoot()
{
	bCanShoot = true;
}

void ADronePawn::AddAmmo(int32 Amount)
{
	if (Amount <= 0 || CurrentAmmo >= MaxAmmo) return;

	CurrentAmmo = FMath::Clamp(CurrentAmmo + Amount, 0, MaxAmmo);
}

float ADronePawn::GetReloadProgress() const
{
	if (!GetWorldTimerManager().IsTimerActive(FireCooldownTimer))
	{
		return 1.0f;
	}

	float Remaining = GetWorldTimerManager().GetTimerRemaining(FireCooldownTimer);
	return 1.0f - (Remaining / FireCooldown);
}

// === Death Handling ===
void ADronePawn::HandleDeath()
{
	// TODO: Add VFX, sound, or animation
	Destroy();
}