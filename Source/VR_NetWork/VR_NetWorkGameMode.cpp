// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "VR_NetWorkGameMode.h"
#include "VR_NetWorkCharacter.h"
#include "Mode/VRGameState.h"
#include "Mode/VRPawn.h"
#include "Mode/VRPlayerController.h"
#include "Mode/VRPlayerState.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerStart.h"

AVR_NetWorkGameMode::AVR_NetWorkGameMode()
{
	// set default pawn class to our Blueprinted character
	// /Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter
	// Blueprint'/Game/VR/Figure/BP_VRPawn.BP_VRPawn'
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/VR/Figure/BP_VRPawn"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AGameStateBase> GameStateCClass(TEXT("Class'/Script/VR_NetWork.VRGameState'"));
	static ConstructorHelpers::FClassFinder<APlayerState> PlayerStateCClass(TEXT("Class'/Script/VR_NetWork.VRPlayerState'"));
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerCClass(TEXT("Class'/Script/VR_NetWork.VRPlayerController'"));


	if (GameStateCClass.Class != NULL)
	{
		GameStateClass = GameStateCClass.Class;
	}

	if (PlayerStateCClass.Class != NULL)
	{
		PlayerStateClass = PlayerStateCClass.Class;
	}

	if (PlayerControllerCClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerCClass.Class;
	}
}

AActor * AVR_NetWorkGameMode::ChoosePlayerStart_Implementation(APlayerController * player)
{

	TSubclassOf<APlayerStart> PlayerStars;
	PlayerStars = APlayerStart::StaticClass();
	TArray<AActor*> FindStarts;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStars, FindStarts);

	for (auto start = FindStarts.CreateIterator(); start; ++start)
	{

		APlayerStart* ss = Cast<APlayerStart>(*start);
		if (ss)
		{
			AGameStateBase* gameStateBase = UGameplayStatics::GetGameState(GetWorld());
			if (gameStateBase)
			{
				if (ss->PlayerStartTag == FName("play1") && player->PlayerState->PlayerId == gameStateBase->PlayerArray[0]->PlayerId)
				{					
					return ss;					
				}
				if (ss->PlayerStartTag != FName("play1") && player->PlayerState->PlayerId != gameStateBase->PlayerArray[0]->PlayerId)
				{
					return ss;
				}
			}
		}
	}
	

	return nullptr;
}
