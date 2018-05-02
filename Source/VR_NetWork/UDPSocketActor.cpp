// Fill out your copyright notice in the Description page of Project Settings.

#include "UDPSocketActor.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AUDPSocketActor::AUDPSocketActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/*bReplicates = true;*/

	ListenSocket = NULL;
}

void AUDPSocketActor::Data_Received_Implementation(const FSingleGlovesData & ReceiveData)
{
}

void AUDPSocketActor::ReceivedArduino_Implementation()
{
	if (!ListenSocket)
	{
		UE_LOG(LogActor, Error, TEXT("No sender socket"));
		isLeftSuccess = false;
		isRightSuccess = false;
	}
	
	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	TArray<uint8> str;
	uint32 Size;
	uint32 CheckSize;
	

	if (GEngine->GameViewport->Viewport->IsForegroundWindow() && ListenSocket->HasPendingData(CheckSize))
	{	
		/*sucess = true;
		ReceiveData = FSingleGlovesData();*/
		FSingleGlovesData TempData = FSingleGlovesData();
		while (ListenSocket->HasPendingData(Size))
		{
			int32 BytesRead = 0;
			str.SetNumUninitialized(FMath::Min(Size, 65507u));
			ListenSocket->RecvFrom(str.GetData(), str.Num(), BytesRead, *targetAddr);
			
			memcpy(&TempData, str.GetData(), BytesRead);
			
			if (TempData.HandName == 0)
			{
				LeftHandData = TempData;
				isLeftSuccess = true;
			}
			else if (TempData.HandName == 1)
			{
				RightHandData = TempData;
				isRightSuccess = true;
			}
		}

	}
	else {
		isLeftSuccess = false;
		isRightSuccess = false;
	}
		
}

void AUDPSocketActor::tData_Received_Implementation(const FTempData & ReceiveData)
{
}

void AUDPSocketActor::DDataReceived_Implementation(FString & ReceiveData, bool & sucess)
{
	if (!ListenSocket)
	{
		UE_LOG(LogActor, Error, TEXT("No sender socket"));
		sucess = false;
	}

	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	TArray<uint8> str;
	uint32 Size;

	if (ListenSocket->HasPendingData(Size))
	{
		sucess = true;
		ReceiveData = "";
		uint8 *Recv = new uint8[Size];
		int32 BytesRead = 0;

		str.SetNumUninitialized(FMath::Min(Size, 65507u));
		ListenSocket->RecvFrom(str.GetData(), str.Num(), BytesRead, *targetAddr);
		char ansiiData[1024];
		memcpy(ansiiData, str.GetData(), BytesRead);
		ansiiData[BytesRead] = 0;
		FString debugData = ANSI_TO_TCHAR(ansiiData);
		ReceiveData = debugData;
	}
	else
		sucess = false;
}

void AUDPSocketActor::Recv(const FArrayReaderPtr & ArrayReaderPtr, const FIPv4Endpoint & EndPt)
{
	UE_LOG(LogActor, Warning, TEXT("Recv!!!"));

	if (!ListenSocket)
	{
		return;
	}
	uint32 Size;
	if (ListenSocket->HasPendingData(Size))
	{
		FSingleGlovesData Data;
		//FGlovesData Data;
		//FTempData Data;
		*ArrayReaderPtr << Data;

		Data_Received(Data);
	}
	//tData_Received(Data);
}

bool AUDPSocketActor::StartUDPReceiver(const FString & ChosenSocketName, const FString & TheIP, const int32 ThePort)
{
	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	FIPv4Address Addr;
	FIPv4Address::Parse(TheIP, Addr);

	FIPv4Endpoint EndPoint(FIPv4Address::Any, ThePort);

	int32 BufferSize = 2 * 1024 * 1024;

	ListenSocket = FUdpSocketBuilder(*ChosenSocketName)
		.AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(EndPoint)
		.WithReceiveBufferSize(BufferSize);

	int32 OutNewSize;

	ListenSocket->SetSendBufferSize(BufferSize, OutNewSize);
	ListenSocket->SetReceiveBufferSize(BufferSize, OutNewSize);

	/*FTimespan ThreadWaitTime = FTimespan::FromMicroseconds(100);
	UDPReceiver = new FUdpSocketReceiver(ListenSocket, ThreadWaitTime, TEXT("UDP RECEIVER"));
	UDPReceiver->OnDataReceived().BindUObject(this, &AUDPSocketActor::Recv);*/

	/*UDPReceiver->Start();*/

	UE_LOG(LogActor, Warning, TEXT("StartUdpReceiver!!"));

	return true;
}

// Called when the game starts or when spawned
void AUDPSocketActor::BeginPlay()
{
	Super::BeginPlay();
	StartUDPReceiver(FString("UDPSocket"), FString("127.0.0.1"), 8888);
}

void AUDPSocketActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	/*UDPReceiver->Stop();*/

	delete UDPReceiver;
	UDPReceiver = nullptr;

	if (ListenSocket)
	{
		ListenSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenSocket);
	}
}

// Called every frame
void AUDPSocketActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	ReceivedArduino();
}

