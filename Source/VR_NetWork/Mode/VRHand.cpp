// Fill out your copyright notice in the Description page of Project Settings.

#include "VRHand.h"


// Sets default values
AVRHand::AVRHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	rootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = rootScene;

	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hand"));
	HandMesh->SetupAttachment(rootScene);


}

// Called when the game starts or when spawned
void AVRHand::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void AVRHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

