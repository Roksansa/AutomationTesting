// Fill out your copyright notice in the Description page of Project Settings.


#include "Tests/JsonUtils.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"

using namespace UE::TEST;

DEFINE_LOG_CATEGORY_STATIC(LogATJsonUtils, Verbose, Verbose);

bool JsonUtils::WriteInputData(const FString& FullPathFileName, const TSharedPtr<FJsonObject>& MainJsonObject)
{
	FString OutputString;
	const TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

	if (!FJsonSerializer::Serialize(MainJsonObject.ToSharedRef(), JsonWriter))
	{
		UE_LOG(LogATJsonUtils, Error, TEXT("Json serializer error"));
		return false;
	}
	if (!JsonWriter->Close())
	{
		UE_LOG(LogATJsonUtils, Error, TEXT("Json file close error"));
		return false;
	}
	if (!FFileHelper::SaveStringToFile(OutputString, *FullPathFileName))
	{
		UE_LOG(LogATJsonUtils, Error, TEXT("Json save to file %s - error"), *FullPathFileName);
		return false;
	}

	return true;
}

bool JsonUtils::ReadInputData(const FString& FullPathFileName, const UStruct* StructDefinition, void* OutStruct)
{
	TSharedPtr<FJsonObject> MainJsonObject;
	FString FileContentString;
	if (!FFileHelper::LoadFileToString(FileContentString, *FullPathFileName))
	{
		UE_LOG(LogATJsonUtils, Error, TEXT("Json load file %s -- error"), *FullPathFileName);
		return false;
	}

	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(FileContentString);
	if (!FJsonSerializer::Deserialize(JsonReader, MainJsonObject))
	{
		UE_LOG(LogATJsonUtils, Error, TEXT("Json deserializer error"));
		return false;
	}
	if (!FJsonObjectConverter::JsonObjectToUStruct(MainJsonObject.ToSharedRef(), StructDefinition, OutStruct, 0, 0))
	{
		UE_LOG(LogATJsonUtils, Error, TEXT("Json converter to object error"));
		return false;
	}

	return true;
}