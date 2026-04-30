// Copyright Epic Games, Inc. All Rights Reserved.

#include "MasterClassGameMode.h"
#include "MasterClassCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMasterClassGameMode::AMasterClassGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
