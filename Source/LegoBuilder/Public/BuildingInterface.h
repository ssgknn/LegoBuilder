// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "BuildingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBuildingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEGOBUILDER_API IBuildingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void PickUp(UPhysicsHandleComponent* PhysicHandle) {};
	virtual void Drop(UPhysicsHandleComponent* PhysicHandle) {};
	virtual void Place(AActor* ActorToAttach, FTransform transform) {};
	virtual void Remove() {};
	virtual void AddChild(AActor* ChildToAtt) {};
	virtual void RemoveChild(AActor* ChildToRemove) {};
	virtual uint8 OverlapCheck() { return false; };
	virtual void AddSnapPoint(FVector3d location, FRotator3d rotation, int priotity) {};
	virtual void TransferChildren(TArray<AActor*> ChildrenActorsToTransfer) {};
	virtual uint8 ParentCheck() { return false; };
	virtual void RemoveChildren(TArray<AActor*> ChildrenActorsToRemove) {};

};
