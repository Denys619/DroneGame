// Fill out your copyright notice in the Description page of Project Settings.


#include "DronePawn.h"
#include "Projectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADronePawn::ADronePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ADronePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
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

void ADronePawn::Shoot()
{
	if (!ProjectileClass) return;

	FVector SpawnLocation = CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * 100.0f;
	FRotator SpawnRotation = CameraComponent->GetComponentRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
}

void ADronePawn::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void ADronePawn::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ADronePawn::MoveUp(float Value)
{
	AddMovementInput(GetActorUpVector(), Value);
}

void ADronePawn::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ADronePawn::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}