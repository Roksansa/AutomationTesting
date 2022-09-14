// Fill out your copyright notice in the Description page of Project Settings.

#include "AutomationTesting/Tests/Input/ATInputRecordingComponent.h"

#include "Tests/JsonUtils.h"

UATInputRecordingComponent::UATInputRecordingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UATInputRecordingComponent::BeginPlay()
{
	Super::BeginPlay();

	check(GetOwner());
	check(GetWorld());
	check(GetOwner()->InputComponent);

	GEngine->bUseFixedFrameRate = true;
	GEngine->FixedFrameRate = FixedCountFps;
	InputData.InitialTransform = GetOwner()->GetActorTransform();
	InputData.FixedCountFps = FixedCountFps;
	InputData.Bindings.Add(MakeBindingsData());
}

void UATInputRecordingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UE::TEST::JsonUtils::WriteInputData(GeneratedFileName(), InputData);
	if (GEngine)
	{
		GEngine->ReloadConfig();
	}
}

void UATInputRecordingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	InputData.Bindings.Add(MakeBindingsData());
}

FBindingsData UATInputRecordingComponent::MakeBindingsData() const
{
	FBindingsData BindingsData;
	BindingsData.GlobalTime = GetWorld()->GetTimeSeconds();
	for (const auto& AxisBinding : GetOwner()->InputComponent->AxisBindings)
	{
		FString StringName = AxisBinding.AxisName.ToString();
		bool bExclude = false;
		for (const auto& ExcludeWord : ExcludeWords)
		{
			if (!StringName.Contains(ExcludeWord)) { continue; }
			bExclude = true;
			break;
		}
		if (!bExclude) { BindingsData.AxisValue.Add(FAxisData{AxisBinding.AxisName, AxisBinding.AxisValue}); }
	}
	return BindingsData;
}

FString UATInputRecordingComponent::GeneratedFileName() const
{
	FString SavedDir = FPaths::ProjectSavedDir();
	const FString Date = FDateTime::Now().ToString();
	return SavedDir.Append("Tests/NewCharacterTestInput").Append(Date).Append(".json");
}
