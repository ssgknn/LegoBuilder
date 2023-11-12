// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"
#include "LegoBuilderGameModeBase.h"

ALegoBuilderGameModeBase::ALegoBuilderGameModeBase()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Character/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
