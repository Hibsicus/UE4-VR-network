// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealNetwork.h"
#include "Runtime/Engine/Classes/Components/ChildActorComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Mode/VRHand.h"
#include "Data/GlovesData.h"
#include "UDPSocketActor.h"
#include "GameFramework/Pawn.h"
#include "VRPawn.generated.h"

class USceneComponent;
class UCameraComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class AUDPSocketActor;
class AGlovesData;
class AVRHand;
class UChildActorComponent;

UCLASS()
class VR_NETWORK_API AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR)
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR)
	USceneComponent* CameraScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR)
	UCameraComponent* VRCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR)
	UStaticMeshComponent* CameraMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR)
	USceneComponent* LeftScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR)
	UChildActorComponent* LeftActorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR)
	USceneComponent* RightScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR)
	UChildActorComponent* RightActorComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	AUDPSocketActor* UDPSocket;

public:

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

	UPROPERTY(ReplicatedUsing = OnRep_HeadTrans)
	FTransform HeadTransform;

	UPROPERTY(ReplicatedUsing = OnRep_LeftHandTrans)
	FTransform LeftHandTransform;

	UPROPERTY(ReplicatedUsing = OnRep_RightHandTrans)
	FTransform RightHandTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HAND)
	TSubclassOf<AVRHand> LeftHandSelect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HAND)
	TSubclassOf<AVRHand> RightHandSelect;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly , Category = HAND)
	AVRHand* LeftHand;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HAND)
	AVRHand* RightHand;
private:
	// NetWork

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_UpdateVRTransform(FTransform Head, FTransform Left, FTransform Right);
	virtual bool Server_UpdateVRTransform_Validate(FTransform Head, FTransform Left, FTransform Right);
	virtual void Server_UpdateVRTransform_Implementation(FTransform Head, FTransform Left, FTransform Right);

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_SpawnVRHand();
	virtual bool Server_SpawnVRHand_Validate();
	virtual void Server_SpawnVRHand_Implementation();

	UFUNCTION()
	void OnRep_HeadTrans();

	UFUNCTION()
	void OnRep_LeftHandTrans();

	UFUNCTION()
	void OnRep_RightHandTrans();

private:
	UFUNCTION()
	void UpdateVRTransformLocally();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Data)
	FSingleGlovesData RightData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Data)
	FSingleGlovesData LeftData;
	
};
