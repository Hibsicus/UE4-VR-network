// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlovesData.generated.h"

USTRUCT(BlueprintType)
struct FSingleGlovesData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gloves)
		int32 HandName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gloves)
		float WristVector_X = .0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gloves)
		float WristVector_Y = .0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gloves)
		float WristVector_Z = .0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gloves)
		int32 ThumbsCurv = .0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gloves)
		int32 ForeFingerCurv = .0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gloves)
		int32 MiddleFingerCurv = .0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gloves)
		int32 RingFingerCurv = .0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gloves)
		int32 LittleFinger = .0f;

	FSingleGlovesData()
	{}
};

UENUM(BlueprintType)
enum class EHandType : uint8
{
	RIGHT	UMETA(DisplayName = "RightHand"),
	LEFT	UMETA(DisplayName = "LeftHand")
};

USTRUCT(BlueprintType)
struct FTempData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TempData)
		int32 tempText;

	FTempData()
	{}
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, FTempData& TheStruct)
{
	Ar << TheStruct.tempText;

	return Ar;
}

FORCEINLINE FArchive& operator<<(FArchive &Ar, FSingleGlovesData& TheStruct)
{
	Ar << TheStruct.HandName;
	Ar << TheStruct.WristVector_X;
	Ar << TheStruct.WristVector_Y;
	Ar << TheStruct.WristVector_Z;
	Ar << TheStruct.ThumbsCurv;
	Ar << TheStruct.ForeFingerCurv;
	Ar << TheStruct.MiddleFingerCurv;
	Ar << TheStruct.RingFingerCurv;
	Ar << TheStruct.LittleFinger;

	return Ar;
}

UCLASS()
class VR_NETWORK_API AGlovesData : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlovesData();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
