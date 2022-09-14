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

	const APawn* Pawn = Cast<APawn>(GetOwner());
	check(Pawn);
	const APlayerController* PlayerController = Pawn->GetController<APlayerController>();
	check(PlayerController);

	PlayerInput = PlayerController->PlayerInput;

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
	const TObjectPtr<UInputComponent> InputComponent = GetOwner()->InputComponent;
	for (const auto& AxisBinding : InputComponent->AxisBindings)
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

	for (int32 i = 0; i < InputComponent->GetNumActionBindings(); ++i)
	{
		const auto Action = InputComponent->GetActionBinding(i);
		const auto ActionKeys = PlayerInput->GetKeysForAction(Action.GetActionName());
		if (ActionKeys.Num() > 0)
		{
			const bool Pressed = PlayerInput->IsPressed(ActionKeys[0].Key);
			const bool State = (Pressed && Action.KeyEvent == EInputEvent::IE_Pressed) || (
				!Pressed && Action.KeyEvent == EInputEvent::IE_Released);
			BindingsData.ActionValue.Add(FActionData{Action.GetActionName(), ActionKeys[0].Key, Action.KeyEvent, State});
		}
	}
	return BindingsData;
}

FString UATInputRecordingComponent::GeneratedFileName() const
{
	FString SavedDir = FPaths::ProjectSavedDir();
	const FString Date = FDateTime::Now().ToString();
	return SavedDir.Append("Tests/NewCharacterTestInput").Append(Date).Append(".json");
}
