// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
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

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		//FInputModeGameAndUI InputMode;
			// --mouse lock later to +doNotLock" InMouseLockMode
		//PlayerController->SetInputMode(InputMode);
		//PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
	}
	
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
			VectorsToReturn.Empty();
			VectorsToReturn.Add(CameraComponent->GetComponentLocation());
			VectorsToReturn.Add((WorldDirection * ReachDistance) + CameraComponent->GetComponentLocation());

			return VectorsToReturn;
		}
	}
	return TArray<FVector>();
}

void APlayerCharacter::HandleBlock(FHitResult HitResult, uint8 bIsHit, FVector EndLocation)
{
	//check if GrabetComponent valid object
	if (PhysicsHandleComponent->GetGrabbedComponent())
	{
		//init local variables used later by ref.
		FVector ClosestPoint_local;
		int ClosestSnapPointIndex_local;
		float ClosestDistance_local;

		ABlock* HeldActor_local = Cast<ABlock>(PhysicsHandleComponent->GetGrabbedComponent()->GetOwner());
		if (HeldActor_local)
		{
			SnapPointIndexLength = HeldActor_local->GetSnapPoints().Num();
		}

		uint8 bHit_local = bIsHit;
		if (bHit_local)
		{
			FVector Location_local = HitResult.Location;
			FVector Normal_local = HitResult.Normal;
			ABlock* HitActor_local = dynamic_cast<ABlock*>(HitResult.GetActor());
			if (HitActor_local)
			{			
				
				FTransform HitActorTransform_local = HitActor_local->GetTransform();
				HeldActor_local = Cast<ABlock>(PhysicsHandleComponent->GetGrabbedComponent()->GetOwner());
				APlayerCharacter::ClosestPointCalculate(HitActor_local->GetSnapPoints(), Location_local, HitActorTransform_local, ClosestPoint_local, ClosestSnapPointIndex_local, ClosestDistance_local);
				TArray<FRotator> SnapDirections_local = HitActor_local->GetSnapDirections();

				//test with GetActor() if its not behave well --> HitActor_local dynamic casted class object instead 
				if (HitResult.GetActor()->Implements<UBuildingInterface>())
				{
					//Call Interface 
					HitActor_local->GetSnapPoints();
					//Find closest point
					this->ClosestPointCalculate(HitActor_local->GetSnapPoints(), Location_local, HitActorTransform_local, ClosestPoint_local, ClosestSnapPointIndex_local, ClosestDistance_local);
					//Set local snap direction 
					SnapDirections_local = HitActor_local->GetSnapDirections();

					//Get the owner of the held actor and cast to ABlock to call Interface implementation. 
					ABlock* Block_cast = Cast<ABlock>(PhysicsHandleComponent->GetGrabbedComponent()->GetOwner());
					Block_cast->OverlapCheck();
				}
				/*else
				{
		
				}*/


				FVector NewLocation;
				FRotator NewRotation;
				if (!((ClosestDistance_local <= SnapDistance) && (HitActor_local->GetSnapPoints().Num() >= 0) && (bIsHit))) //STATEMENT later
				{
					NewLocation = HitActorTransform_local.TransformPosition(ClosestPoint_local) + (HeldActor_local->GetActorLocation() - HeldActor_local->GetActorTransform().TransformPosition(HeldActor_local->GetSnapPoints()[SnapPointIndex]));
					NewRotation = FRotationMatrix::MakeFromZ(Normal_local).Rotator();
				}
				else
				{
					NewLocation = Location_local + (HeldActor_local->GetActorLocation() - HeldActor_local->GetActorTransform().TransformPosition(HeldActor_local->GetSnapPoints()[SnapPointIndex]));
					HitActorTransform_local.SetRotation(SnapDirections_local[ClosestSnapPointIndex_local].Quaternion());
					NewRotation = HitActorTransform_local.GetRotation().Rotator();
				}

				RotationalHelperComponent->SetWorldLocation(NewLocation);
				RotationalHelperComponent->SetWorldRotation(NewRotation);
				RotationalHelperComponent->AddLocalRotation(WorldRotationOffset());

				FMatrix RotationMatrix = FRotationMatrix(NewRotation);

				//(FVector Axis, float Angle)
				// calculations from KismetMathLibrary - rotator from axis and angle:
				FVector SafeAxisY = RotationMatrix.GetScaledAxis(EAxis::Y).GetSafeNormal(); // Make sure axis is unit length
				FVector SafeAxisZ = RotationMatrix.GetScaledAxis(EAxis::Z).GetSafeNormal();

				FRotator WorldRotationToAdd = FRotator(FQuat(SafeAxisY, FMath::DegreesToRadians(RotationOffset.Pitch)) * FQuat(SafeAxisZ, FMath::DegreesToRadians(RotationOffset.Yaw)));
				RotationalHelperComponent->AddWorldRotation(WorldRotationToAdd);

				PhysicsHandleComponent->SetTargetLocationAndRotation(RotationalHelperComponent->GetComponentLocation(), RotationalHelperComponent->GetComponentRotation());
			}
		}
		else
		{
			PhysicsHandleComponent->SetTargetLocationAndRotation(EndLocation, RotationOffset);
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

FRotator APlayerCharacter::WorldRotationOffset()
{
	ABlock* Block = Cast<ABlock>(PhysicsHandleComponent->GetGrabbedComponent()->GetOwner());
	if (Block)
	{
		FRotator Rotator1(0.0, 180.0, 180.0);
		return Block->GetSnapDirections()[SnapPointIndex].GetInverse() + Rotator1;
	}
	return FRotator();
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::RotateLeftRight(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "RoteteLeftRight");
}

void APlayerCharacter::RotateUpDown(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "RoteteUpDown");
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeltaSeconds = DeltaTime;

	TArray<FVector> PreTrace = PreTraceCheck();
	FVector TraceStart;
	FVector TraceEnd;
	if (PreTrace.Num() > 0)
	{
		TraceStart = PreTrace[0];
		TraceEnd = PreTrace[1];
	}
	
	FHitResult TraceHit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);
	bool bHit_local = GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
	if (bHit_local)
	{
		//debug
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TraceHit.GetActor()->GetFName().ToString());
		DrawDebugLine(GetWorld(), TraceStart, TraceHit.Location, FColor(255, 0, 0), false, 0.3f, 0, 1);
		this->HandleBlock(TraceHit, bHit_local, TraceEnd);
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//add Input mapping context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		//Get local player subsystem
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

	}
}

