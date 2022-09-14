// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ATInputRecordingUtils.generated.h"

USTRUCT()
struct FAxisData
{
	GENERATED_BODY()

	UPROPERTY()
	FName Name;

	UPROPERTY()
	float Value;
};

USTRUCT()
struct FBindingsData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FAxisData> AxisValue;

	UPROPERTY()
	float GlobalTime = 0.f;
};

USTRUCT()
struct FInputData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FBindingsData> Bindings;

	UPROPERTY()
	FTransform InitialTransform;

	UPROPERTY()
	float FixedCountFps;
};
