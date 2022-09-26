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
	template <class ConverterType, typename TEnableIf<TIsDerivedFrom<typename TRemovePointer<decltype(ConverterType::StaticStruct())>::Type,
		UScriptStruct>::Value, bool>::Type = true>
	static bool WriteInputData(const FString& FullPathFileName, const ConverterType& InInputData);

	template <class ConverterType, typename TEnableIf<TIsDerivedFrom<typename TRemovePointer<decltype(ConverterType::StaticStruct())>::Type,
		UScriptStruct>::Value, bool>::Type = true>
	static bool ReadInputData(const FString& FullPathFileName, ConverterType& OutInputData);

private:
	static bool WriteInputDataInner(const FString& FullPathFileName, const TSharedPtr<FJsonObject>& MainJsonObject);
	static bool ReadInputDataInner(const FString& FullPathFileName, const UStruct* StructDefinition, void* OutStruct);
};

template <class ConverterType, typename TEnableIf<TIsDerivedFrom<typename TRemovePointer<decltype(ConverterType::StaticStruct())>::Type,
	UScriptStruct>::Value, bool>::Type>
bool JsonUtils::WriteInputData(const FString& FullPathFileName, const ConverterType& InInputData)
{
	TSharedPtr<FJsonObject> MainJsonObject = FJsonObjectConverter::UStructToJsonObject(InInputData);
	if (!MainJsonObject.IsValid()) { return false; }
	return WriteInputDataInner(FullPathFileName, MainJsonObject);
}

template <class ConverterType, typename TEnableIf<TIsDerivedFrom<typename TRemovePointer<decltype(ConverterType::StaticStruct())>::Type,
	UScriptStruct>::Value, bool>::Type>
bool JsonUtils::ReadInputData(const FString& FullPathFileName, ConverterType& OutInputData)
{
	return ReadInputDataInner(FullPathFileName, ConverterType::StaticStruct(), &OutInputData);
}
}
