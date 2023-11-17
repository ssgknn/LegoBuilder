// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"
#include "SnapPoint_Component.h"

void USnapPoint_Component::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		ABlock* Block_cast = Cast<ABlock>(this->GetOwner());
		if (Block_cast)
		{
			Block_cast->AddSnapPoint(this->GetRelativeLocation(), this->GetRelativeRotation(), Priority);
		}
	}
}

void USnapPoint_Component::DataSyncCompleted()
{
	this->DestroyComponent();
}
