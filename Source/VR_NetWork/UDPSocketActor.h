// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/GlovesData.h"
#include "Networking.h"
#include "GameFramework/Actor.h"
#include "UDPSocketActor.generated.h"

class AGlovesData;

UCLASS()
class VR_NETWORK_API AUDPSocketActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUDPSocketActor();

	UFUNCTION(BlueprintNativeEvent)
	void Data_Received(const FSingleGlovesData& ReceiveData);
	virtual void Data_Received_Implementation(const FSingleGlovesData& ReceiveData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReceivedArduino();
	virtual void ReceivedArduino_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void tData_Received(const FTempData& ReceiveData);
	virtual void tData_Received_Implementation(const FTempData& ReceiveData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DDataReceived(FString& ReceiveData, bool& sucess);
	virtual void DDataReceived_Implementation(FString& ReceiveData, bool& sucess);

	FSocket* ListenSocket;

	FUdpSocketReceiver* UDPReceiver = nullptr;
	void Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);

	bool StartUDPReceiver(
		const FString& ChosenSocketName,
		const FString& TheIP,
		const int32 ThePort
	);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Data)
	FSingleGlovesData RightHandData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Data)
	FSingleGlovesData LeftHandData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Data)
	bool isRightSuccess;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Data)
		bool isLeftSuccess;
	
};
