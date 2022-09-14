// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "UObject/Class.h"

namespace UE::TEST
{
DEFINE_LOG_CATEGORY_STATIC(LogATJsonUtils, Verbose, Verbose);
class JsonUtils
{
public:
	template <class ConverterType>
	static bool WriteInputData(const FString& FullPathFileName, const ConverterType& InInputData);
	template <class ConverterType>
	static bool ReadInputData(const FString& FullPathFileName, ConverterType& OutInputData);

private:
	static bool WriteInputData(const FString& FullPathFileName,const TSharedPtr<FJsonObject>& MainJsonObject);
	static bool ReadInputData(const FString& FullPathFileName, const UStruct* StructDefinition, void* OutStruct);
};

template <class ConverterType>
bool JsonUtils::WriteInputData(const FString& FullPathFileName, const ConverterType& InInputData)
{
	TSharedPtr<FJsonObject> MainJsonObject = FJsonObjectConverter::UStructToJsonObject(InInputData);
	if (!MainJsonObject.IsValid()) { return false; }
	return WriteInputData(FullPathFileName, MainJsonObject);
}

template <class ConverterType>
bool JsonUtils::ReadInputData(const FString& FullPathFileName, ConverterType& OutInputData)
{
	return ReadInputData(FullPathFileName, ConverterType::StaticStruct(), &OutInputData);
}

}
