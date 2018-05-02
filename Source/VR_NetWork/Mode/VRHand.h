// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/GlovesData.h"
#include "UnrealNetwork.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "VRHand.generated.h"

class AGlovesData;
class USceneComponent;
class USkeletalMeshComponent;

UCLASS()
class VR_NETWORK_API AVRHand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVRHand();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hand)
	USceneComponent* rootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hand)
	USkeletalMeshComponent* HandMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hand)
	EHandType HandType;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



private:


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
