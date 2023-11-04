// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Block.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the PhysicsHandle component
	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	// Create and set up the camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create and set up the camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);

	//Set reach distance to 3000
	ReachDistance = 3000.f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

TArray<FVector> APlayerCharacter::PreTraceCheck()
{
	if (PhysicsHandleComponent->GetGrabbedComponent())
	{
		ABlock* Block_cast = Cast<ABlock>(PhysicsHandleComponent->GetGrabbedComponent()->GetOwner());
		if (Block_cast)
		{
			ChildsToIgnore = Block_cast->GetChildActors();
			ChildsToIgnore.Add(PhysicsHandleComponent->GetGrabbedComponent()->GetOwner());
		}
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			FVector2D MouseLocation;
			FVector WorldLocation, WorldDirection;

			// Get the current mouse location in screen coordinates
			PlayerController->GetMousePosition(MouseLocation.X, MouseLocation.Y);

			// Convert the screen coordinates to a ray in the world
			PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

			TArray<FVector> VectorsToReturn;
			VectorsToReturn.Add(CameraComponent->GetComponentLocation());
			VectorsToReturn.Add((WorldDirection * ReachDistance) + CameraComponent->GetComponentLocation());

			return VectorsToReturn;
		}
	}
	return TArray<FVector>();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeltaSeconds = DeltaTime;

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

