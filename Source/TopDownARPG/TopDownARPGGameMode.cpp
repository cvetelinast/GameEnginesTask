// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TopDownARPGGameMode.h"
#include "TopDownARPGPlayerController.h"
#include "TopDownARPGCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "TopDownARPG.h"
#include "Engine/World.h"

ATopDownARPGGameMode::ATopDownARPGGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATopDownARPGPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ATopDownARPGGameMode::EndGame(bool IsWin)
{
	if (IsWin)
	{
		UE_LOG(LogTopDownARPG, Display, TEXT("Win"));
	}
	else
	{
		UE_LOG(LogTopDownARPG, Display, TEXT("Lose"));
	}

	GetWorld()->GetTimerManager().ClearTimer(LoseTimerHandle);
}

void ATopDownARPGGameMode::BeginPlay()
{
	LoseTimerDelegate.BindUObject(this, &ATopDownARPGGameMode::EndGame, false);

	GetWorld()->GetTimerManager().SetTimer(LoseTimerHandle, LoseTimerDelegate, GameTimeInSeconds, false);
}