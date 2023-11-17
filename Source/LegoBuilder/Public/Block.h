// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "Components/TimelineComponent.h"
#include "BuildingInterface.h"
#include "Block.generated.h"

class UCurveFloat;

UCLASS()
class LEGOBUILDER_API ABlock : public AActor, public IBuildingInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BlockMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UTimelineComponent* ScaleTimelineComp;

	UPROPERTY()
	TArray<FVector> SnapPoints;

	UPROPERTY()
	TArray<FRotator> SnapDirections;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlacementData")
	TArray<int> SnapPriority;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlacementData")
	int ConnSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlockVariable")
	TArray<AActor*> ChildActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlockVariable")
	AActor* Parent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlockVariable")
	bool bIsPickedUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlockVariable")
	bool bIsRotating;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlockVariable")
	UMaterialInterface* CachedMaterial;

	

private:
	FString MaterialPath = TEXT("/Game/Materials/MI_ProcGrid_Hologram_Red.uasset");
	UMaterialInterface* MAT_Hologram_Red = LoadObject<UMaterialInterface>(nullptr, *MaterialPath);

	UPROPERTY()
	int32 tempIndex;

	UPROPERTY()
	int32 tempMinIndex;

	UPROPERTY()
	TArray<FVector> tempSnapPoints;

	UPROPERTY()
	TArray<FRotator> tempSnapDirections;

	UPROPERTY()
	TArray<int> tempSnapPriority;


	UFUNCTION()
	void CalculateSnapPoint();

public:	

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Block Function")
	void ChildCollisionUpdate(ECollisionEnabled::Type collisionType);

	UFUNCTION(Category = "Block Function")
	void ChangeScale(uint8 bIsPlaced);

	UFUNCTION(Category = "Interface Override")
	virtual void AddSnapPoint(FVector3d location, FRotator3d rotation, int priotity) override;

	UFUNCTION(BlueprintCallable, Category = "Interface Override")
	virtual void AddChild(AActor* ChildToAtt) override;

	UFUNCTION(BlueprintCallable, Category = "Interface Override")
	virtual void TransferChildren(TArray<AActor*> ChildrenActorsToTransfer) override;

	UFUNCTION(BlueprintCallable, Category = "Interface Override")
	virtual void RemoveChild(AActor* ChildToRemove) override;

	UFUNCTION(BlueprintCallable, Category = "Interface Override")
	virtual void RemoveChildren(TArray<AActor*> ChildrenActorsToRemove) override;

	UFUNCTION(BlueprintCallable, Category = "Interface Override")
	virtual void PickUp(UPhysicsHandleComponent* PhysicHandle) override;

	UFUNCTION(BlueprintCallable, Category = "Interface Override")
	virtual void Remove() override;

	UFUNCTION(BlueprintCallable, Category = "Interface Override")
	virtual void Drop(UPhysicsHandleComponent* PhysicHandle) override;

	UFUNCTION(BlueprintCallable, Category = "Interface Override")
	virtual void Place(AActor* ActorToPlace, FTransform transform) override;

	//getters
	TArray<AActor*> GetChildActors();

	TArray<FVector3d> GetSnapPoints();

	TArray<FRotator> GetSnapDirections();

	int GetConnSize();


};
