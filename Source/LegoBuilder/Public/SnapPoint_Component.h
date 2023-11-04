// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "BuildingInterface.h"
#include "SnapPoint_Component.generated.h"

/**
 * 
 */
UCLASS()
class LEGOBUILDER_API USnapPoint_Component : public UStaticMeshComponent, public IBuildingInterface
{
	GENERATED_BODY()
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	int32 Priority;

public:

	UFUNCTION()
	void DataSyncCompleted();
};
