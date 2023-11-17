// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABlock::ABlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BlockMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	BlockMeshComponent->SetupAttachment(GetRootComponent());

	ScaleTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));

}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();

	CachedMaterial = BlockMeshComponent->GetMaterial(0); //temp
}

void ABlock::CalculateSnapPoint()
{
	for (int i = 0; i < SnapPoints.Num() - 1; ++i)
	{
		tempIndex = i;
		int32 tempMinValue = 0;
		UKismetMathLibrary::MinOfIntArray(SnapPriority, tempMinIndex, tempMinValue);
		tempSnapPoints[tempIndex] = SnapPoints[tempMinIndex];
		tempSnapDirections[tempIndex] = SnapDirections[tempMinIndex];
		tempSnapPriority[tempIndex] = SnapPriority[tempMinIndex];
		SnapPriority[tempMinIndex] = 999999;
	}

	SnapPoints = tempSnapPoints;
	SnapDirections = tempSnapDirections;
	SnapPriority = tempSnapPriority;
}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TArray<AActor*> OverlappingActors;

	if (bIsPickedUp)
	{
		if (!bIsRotating)
		{
			GetOverlappingActors(OverlappingActors);
			if (sizeof(OverlappingActors) > 0)
			{
				BlockMeshComponent->SetMaterial(0, CachedMaterial);
			}
			else
			{
				BlockMeshComponent->SetMaterial(0, MAT_Hologram_Red);
			}
		}
		else
		{
			BlockMeshComponent->SetMaterial(0, CachedMaterial);
		}
	}
	else
	{
		BlockMeshComponent->SetMaterial(0, CachedMaterial);
	}
}

void ABlock::ChildCollisionUpdate(ECollisionEnabled::Type collisionType)
{
	BlockMeshComponent->SetCollisionEnabled(collisionType);
	
	switch (collisionType)
	{
	case ECollisionEnabled::Type::NoCollision:
		BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
		BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		this->bIsPickedUp = true;
		for (AActor* ChildActor : ChildActors)
		{
			Cast<ABlock>(ChildActor)->ChildCollisionUpdate(collisionType);
		}
		break;

	case ECollisionEnabled::Type::QueryOnly:
		BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
		BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
		this->bIsPickedUp = true;
		for (AActor* ChildActor : ChildActors)
		{
			Cast<ABlock>(ChildActor)->ChildCollisionUpdate(collisionType);
		}
		break;

	case ECollisionEnabled::Type::PhysicsOnly:
		BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
		BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
		this->bIsPickedUp = false;
		for (AActor* ChildActor : ChildActors)
		{
			Cast<ABlock>(ChildActor)->ChildCollisionUpdate(collisionType);
		}
		break;

	case ECollisionEnabled::Type::QueryAndPhysics:
		BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
		BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		this->bIsPickedUp = false;
		for (AActor* ChildActor : ChildActors)
		{
			Cast<ABlock>(ChildActor)->ChildCollisionUpdate(collisionType);
		}
		break;

	case ECollisionEnabled::Type::ProbeOnly:
		BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
		BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
		this->bIsPickedUp = false;
		for (AActor* ChildActor : ChildActors)
		{
			Cast<ABlock>(ChildActor)->ChildCollisionUpdate(collisionType);
		}
		break;

	case ECollisionEnabled::Type::QueryAndProbe:
		BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
		BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
		this->bIsPickedUp = false;
		for (AActor* ChildActor : ChildActors)
		{
			Cast<ABlock>(ChildActor)->ChildCollisionUpdate(collisionType);
		}
		break;
	}

}

void ABlock::ChangeScale(uint8 bIsPlaced)
{
	if (bIsPlaced)
	{

	}
	else
	{

	}
}

void ABlock::AddSnapPoint(FVector3d location, FRotator3d rotation, int priotity)
{
	SnapPoints.Add(location);
	SnapDirections.Add(rotation);
	SnapPriority.Add(priotity);
	CalculateSnapPoint();
}

void ABlock::AddChild(AActor* ChildToAtt)
{
	ChildActors.Add(ChildToAtt);
	if (Parent)
	{
		//if (IBuildingInterface* ParentInterface = Cast<IBuildingInterface>(Parent))
		if (ABlock* ParentBlock_cast = Cast<ABlock>(Parent))
		{
			ParentBlock_cast->AddChild(ChildToAtt);
		}
		//Parent->AddChild(ChildToAtt);
	}
}

void ABlock::TransferChildren(TArray<AActor*> ChildrenActorsToTransfer)
{
	for (int i = 0; i < ChildActors.Num(); ++i)
	{
		this->AddChild(ChildActors[i]);
	}
}

void ABlock::RemoveChild(AActor* ChildToRemove)
{
	ChildActors.Remove(ChildToRemove);
	if (Parent)
	{
		//if (IBuildingInterface* ParentInterface = Cast<IBuildingInterface>(Parent))
		if (ABlock* ParentBlock_cast = Cast<ABlock>(Parent))
		{
			ParentBlock_cast->RemoveChild(ChildToRemove);
		}
		//Parent->RemoveChild(ChildToRemove);
	}
}

void ABlock::RemoveChildren(TArray<AActor*> ChildrenActorsToRemove)
{
	for (int i = 0; i < ChildActors.Num(); ++i)
	{
		this->RemoveChild(ChildActors[i]);
	}
}

void ABlock::PickUp(UPhysicsHandleComponent* PhysicHandle)
{
	if (PhysicHandle)
	{
		this->Remove();
		FVector GrabLocation = this->GetActorTransform().GetLocation();
		FRotator GrabRotation = this->GetActorRotation();
		GrabRotation = FRotator(0, 0, 0);
		PhysicHandle->GrabComponentAtLocationWithRotation(BlockMeshComponent, NAME_None, GrabLocation, GrabRotation);
		bIsPickedUp = true;
		this->ChangeScale(bIsPickedUp);

		// play sound Pick ---------------------

		BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
		BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);

		for (int i = 0; i < ChildActors.Num(); ++i)
		{
			if (ABlock* BlockActor = Cast<ABlock>(ChildActors[i]))
			{
				BlockActor->ChildCollisionUpdate(ECollisionEnabled::Type::QueryOnly);
			}
		}
	}
}

void ABlock::Remove()
{
	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	BlockMeshComponent->SetSimulatePhysics(true);

	//if (IBuildingInterface* ParentInterface = Cast<IBuildingInterface>(Parent))
	ABlock* ParentBlock_cast = Cast<ABlock>(Parent);
	if(ParentBlock_cast)
	{
		ParentBlock_cast->RemoveChild(this);
		ParentBlock_cast->RemoveChildren(ChildActors);
		Parent = nullptr;
	}

	//Parent->RemoveChild(this);
	//Parent->RemoveChildren(ChildActors);
	Parent = nullptr;
}

void ABlock::Drop(UPhysicsHandleComponent* PhysicHandle)
{
	if (PhysicHandle)
	{
		PhysicHandle->ReleaseComponent();
		bIsPickedUp = false;
		this->ChangeScale(false);

		BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
		BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

		BlockMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);

		for (int i = 0; i < ChildActors.Num(); ++i)
		{
			if (ABlock* BlockActor = Cast<ABlock>(ChildActors[i]))
			{
				BlockActor->ChildCollisionUpdate(ECollisionEnabled::Type::QueryAndPhysics);
			}
		}
	}
}

void ABlock::Place(AActor* ActorToAttach, FTransform transform)
{
	IBuildingInterface* Interface = Cast<IBuildingInterface>(ActorToAttach);
	if (Interface)
	{
		Interface->AddChild(this);
	}
	//ActorToAttach->AddChild(this);

	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::KeepWorld,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true  // Set bWeldSimulatedBodies to true
	);
	this->AttachToActor(ActorToAttach, AttachmentRules, NAME_None);

	BlockMeshComponent->SetSimulatePhysics(false);
	Parent = ActorToAttach;
	this->SetActorRelativeLocation(transform.GetLocation(), false, false);
	this->SetActorRelativeRotation(transform.GetRotation(), false);

	if (Interface)
	{
		Interface->TransferChildren(ChildActors);
	}
	//ActorToAttach->TransferChildren(ChildActors);

	BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	BlockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	BlockMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);

	for (int i = 0; i < ChildActors.Num(); ++i)
	{
		if (ABlock* BlockActor = Cast<ABlock>(ChildActors[i]))
		{
			BlockActor->ChildCollisionUpdate(ECollisionEnabled::Type::QueryAndPhysics);
		}
	}

	//Play sound place --------------------------


}

TArray<AActor*> ABlock::GetChildActors()
{
	return ChildActors;
}

TArray<FVector> ABlock::GetSnapPoints()
{
	return SnapPoints;
}

TArray<FRotator> ABlock::GetSnapDirections()
{
	return SnapDirections;
}

int ABlock::GetConnSize()
{
	return ConnSize;
}


