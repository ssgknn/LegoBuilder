// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"
#include "LegoBuilderGameModeBase.h"

ALegoBuilderGameModeBase::ALegoBuilderGameModeBase()
{
    // Set default pawn class to your custom character class
    DefaultPawnClass = APlayerCharacter::StaticClass(); // Set this to your custom character class

    // Customize other properties as needed
    bStartPlayersAsSpectators = false; // Start players as spectators by default
    bUseSeamlessTravel = true; // Enable seamless travel between levels
    //PlayerControllerClass = // Set custom player controller class
    //HUDClass =  // Set custom HUD class
    //GameStateClass =  // Set custom GameState class
    //PlayerStateClass =  // Set custom PlayerState class
    //DefaultPlayerName =  // Set default player name
}
