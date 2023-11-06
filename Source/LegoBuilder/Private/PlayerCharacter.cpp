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

	// Initialize the RotationalHelperComponent
	RotationalHelperComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RotationalHelperComponent"));
	RootComponent = RotationalHelperComponent;

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
			ActorsToIgnore = Block_cast->GetChildActors();
			ActorsToIgnore.Add(PhysicsHandleComponent->GetGrabbedComponent()->GetOwner());
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

void APlayerCharacter::HandleBlock(FHitResult HitResult, uint8 bIsHit, FVector EndLocation)
{
	if (PhysicsHandleComponent->GetGrabbedComponent())
	{
		ABlock* HeldActor_local = Cast<ABlock>(PhysicsHandleComponent->GetGrabbedComponent()->GetOwner());
		if (HeldActor_local)
		{
			SnapPointIndexLength = HeldActor_local->GetSnapPoints().Num();
		}

		if (uint8 bHit_local = bIsHit)
		{
			FVector Location_local = HitResult.Location;
			FVector Normal_local = HitResult.Normal;
			AActor* HitActor_local = HitResult.GetActor();
			FTransform HitActorTransform_local = HitActor_local->GetTransform();

			if (!HitActor_local->Implements<UBuildingInterface>())
			{
				FVector NewLocation;
				if (true) //STATEMENT later
				{

				}
				else
				{

				}
				HeldActor_local->GetSnapPoints()[SnapPointIndex].TransformPosition(HeldActor_local->GetActorTransform())
					RotationalHelperComponent->SetWorldLocation(NewLocation);
				HeldActor_local->GetActorLocation();

			}
		}

	}

}

void APlayerCharacter::ClosestPointCalculate(TArray<FVector> Points, FVector TestLocation, FTransform HitActorTransform, FVector& ClosestPointResult, int& ClosesPointIdxResult, float& DistanceResult)
{
	FVector TestLocation_local = HitActorTransform.InverseTransformPosition(TestLocation);
	FVector ClosestPoint_local = Points[0];
	float Distance_local = FVector::Distance(TestLocation_local, ClosestPoint_local);
	int ClosestPointIndex_local = 0;

	for (int32 Index = 0; Index < Points.Num(); Index++)
	{
		if (FVector::Distance(TestLocation_local, Points[Index]) < Distance_local)
		{
			ClosestPoint_local = Points[Index];
			Distance_local = FVector::Distance(TestLocation_local, Points[Index]);
			ClosestPointIndex_local = Index;
		}
	}
	DistanceResult = Distance_local;
	ClosestPointResult = ClosestPoint_local;
	ClosesPointIdxResult = ClosestPointIndex_local;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeltaSeconds = DeltaTime;

	TArray<FVector> PreTrace = PreTraceCheck();
	FVector TraceStart = PreTrace[0];
	FVector TraceEnd = PreTrace[1];

	FHitResult TraceHit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);

	if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		//debug
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TraceHit.GetActor()->GetFName().ToString());
		DrawDebugLine(GetWorld(), TraceStart, TraceHit.Location, FColor(255, 0, 0), false, 0.3f, 0, 1);

	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

