// Copyright Epic Games, Inc. All Rights Reserved.

#include "GunSmith_NlundyGameMode.h"
#include "GunSmith_NlundyCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGunSmith_NlundyGameMode::AGunSmith_NlundyGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
