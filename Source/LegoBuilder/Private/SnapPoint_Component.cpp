// Fill out your copyright notice in the Description page of Project Settings.


#include "SnapPoint_Component.h"

void USnapPoint_Component::BeginPlay()
{
	Super::BeginPlay();

	if (IBuildingInterface* BuildingInterface = Cast<IBuildingInterface>(this))
	{
		BuildingInterface->AddSnapPoint(this->GetRelativeLocation(), this->GetRelativeRotation(), Priority);
	}

}

void USnapPoint_Component::DataSyncCompleted()
{
	this->DestroyComponent();
}
