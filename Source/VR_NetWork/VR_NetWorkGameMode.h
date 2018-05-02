// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VR_NetWorkGameMode.generated.h"

UCLASS(minimalapi)
class AVR_NetWorkGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVR_NetWorkGameMode();

	UFUNCTION(BlueprintNativeEvent)
	AActor* ChoosePlayerStart(APlayerController* player);
	virtual AActor* ChoosePlayerStart_Implementation(APlayerController* player);
};



