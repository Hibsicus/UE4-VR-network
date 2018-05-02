// Fill out your copyright notice in the Description page of Project Settings.

#include "VRPawn.h"
#include "SteamVRFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	CameraScene = CreateDefaultSubobject<USceneComponent>(TEXT("CameraScene"));
	CameraScene->SetupAttachment(RootScene);

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(CameraScene);

	CameraMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraMesh"));
	CameraMesh->SetupAttachment(VRCamera);

	LeftScene = CreateDefaultSubobject<USceneComponent>(TEXT("LeftScene"));
	LeftScene->SetupAttachment(RootScene);

	LeftActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("LeftHandActor"));
	LeftActorComponent->SetupAttachment(LeftScene);

	RightScene = CreateDefaultSubobject<USceneComponent>(TEXT("RightScene"));
	RightScene->SetupAttachment(RootScene);

	RightActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("RightHandActor"));
	RightActorComponent->SetupAttachment(RightScene);
}

void AVRPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVRPawn, HeadTransform);
	DOREPLIFETIME(AVRPawn, LeftHandTransform);
	DOREPLIFETIME(AVRPawn, RightHandTransform);
}


bool AVRPawn::Server_UpdateVRTransform_Validate(FTransform Head, FTransform Left, FTransform Right)
{
	return true;
}

void AVRPawn::Server_UpdateVRTransform_Implementation(FTransform Head, FTransform Left, FTransform Right)
{
	HeadTransform = Head;
	LeftHandTransform = Left;
	RightHandTransform = Right;
}

bool AVRPawn::Server_SpawnVRHand_Validate()
{
	return true;
}

void AVRPawn::Server_SpawnVRHand_Implementation()
{
	FVector loc = FVector(.0f, .0f, .0f);
	FRotator rot = FRotator(.0f, .0f, .0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (LeftHandSelect.Get())
	{
		LeftHand = GetWorld()->SpawnActor<AVRHand>(LeftHandSelect.Get(), loc, rot, SpawnInfo);
		LeftHand->AttachToComponent(LeftScene, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	if (RightHandSelect.Get())
	{
		RightHand = GetWorld()->SpawnActor<AVRHand>(RightHandSelect.Get(), loc, rot, SpawnInfo);
		RightHand->AttachToComponent(RightScene, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

void AVRPawn::OnRep_HeadTrans()
{
	CameraMesh->SetRelativeTransform(HeadTransform);
}

void AVRPawn::OnRep_LeftHandTrans()
{
	//if(LeftHand)
	//	LeftHand->HandMesh->SetRelativeTransform(LeftHandTransform);
	AVRHand* hand = Cast<AVRHand>(LeftActorComponent->GetChildActor());
	if (hand)
		hand->SetActorRelativeTransform(LeftHandTransform);
}

void AVRPawn::OnRep_RightHandTrans()
{
	/*if(RightHand)
		RightHand->HandMesh->SetRelativeTransform(RightHandTransform);*/
	AVRHand* hand = Cast<AVRHand>(RightActorComponent->GetChildActor());
	if (hand)
		hand->SetActorRelativeTransform(RightHandTransform);
}

void AVRPawn::UpdateVRTransformLocally()
{
	if (IsLocallyControlled())
	{	
		
		HeadTransform = VRCamera->GetRelativeTransform();

		TArray<int32> foundIDs;
		USteamVRFunctionLibrary::GetValidTrackedDeviceIds(ESteamVRTrackedDeviceType::Other, foundIDs);

		FVector t_left_location;
		FRotator t_left_rotation;

		FVector t_right_location;
		FRotator t_right_rotation;

		UE_LOG(LogActor, Warning, TEXT("%d"), foundIDs.Num());
		if (foundIDs.Num() == 1)
		{
			
			USteamVRFunctionLibrary::GetTrackedDevicePositionAndOrientation(foundIDs[0], t_left_location, t_left_rotation);
			LeftHandTransform = FTransform(t_left_rotation, t_left_location);
		}
		if (foundIDs.Num() == 2)
		{
			USteamVRFunctionLibrary::GetTrackedDevicePositionAndOrientation(foundIDs[1], t_right_location, t_right_rotation);
			RightHandTransform = FTransform(t_right_rotation, t_right_location);
		}
	}

}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();
	
	
	/*Server_SpawnVRHand();*/

	
		TSubclassOf<AUDPSocketActor> udp = AUDPSocketActor::StaticClass();
		TArray<AActor*> findclass;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), udp, findclass);
		for (auto socket = findclass.CreateIterator(); socket; ++socket)
		{
			AUDPSocketActor* ss = Cast<AUDPSocketActor>(*socket);
			if (ss)
			{
				UDPSocket = ss;
			}
		}
	
}

// Called every frame
void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UDPSocket)
	{
		if (UDPSocket->isLeftSuccess)
		{
			LeftData = UDPSocket->LeftHandData;
		}

		if (UDPSocket->isRightSuccess)
		{
			RightData = UDPSocket->RightHandData;
		}
	}

	UpdateVRTransformLocally();

	OnRep_HeadTrans();
	OnRep_LeftHandTrans();
	OnRep_RightHandTrans();

	Server_UpdateVRTransform(HeadTransform, LeftHandTransform, RightHandTransform);

}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

