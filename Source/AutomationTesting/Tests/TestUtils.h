#pragma once
#include "Tests/AutomationCommon.h"

namespace UE::TEST
{
template <typename TestType, typename ExpectedType>
struct TestPayload
{
	TestType TestValue;
	ExpectedType ExpectedValue;
};

template <typename EnumType, typename FunctionType>
void ForEach(FunctionType&& Function)
{
	const UEnum* Enum = StaticEnum<EnumType>();
	for (int32 i = 0; i < Enum->NumEnums() - 1; ++i)
	{
		Function(static_cast<EnumType>(Enum->GetValueByIndex(i)), Enum->GetNameByIndex(i));
	}
}

class LevelScope
{
public:
	LevelScope(const FString& MapName)
	{
		AutomationOpenMap(MapName);
	}

	~LevelScope()
	{
		ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
	}
};

template <class T>
class FATUntilLatentCommand : public IAutomationLatentCommand
{
public:
	FATUntilLatentCommand(TFunction<void(T Param)> InCallback, TFunction<void()> InTimeoutCallback, float InTimeout, T CurParam);
	virtual bool Update() override;

private:
	TFunction<void(T Param)> Callback;
	TFunction<void()> TimeoutCallback;
	T Param;
	float Timeout;
};

template <class T>
FATUntilLatentCommand<T>::FATUntilLatentCommand(TFunction<void(T Type)> InCallback, TFunction<void()> InTimeoutCallback, float InTimeout,
	T CurParam)
	: Callback(MoveTemp(InCallback)), TimeoutCallback(InTimeoutCallback), Param(CurParam), Timeout(InTimeout)
{
}

template <class T>
bool FATUntilLatentCommand<T>::Update()
{
	const double NewTime = FPlatformTime::Seconds();
	if (NewTime - StartTime >= Timeout)
	{
		TimeoutCallback();
		return true;
	}

	Callback(Param);
	return false;
}

template <typename T>
T* CreateBlueprint(UWorld* World, const FString& Name, const FTransform& Transform = FTransform::Identity)
{
	const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *Name);
	return (World && Blueprint) ? World->SpawnActor<T>(Blueprint->GeneratedClass, Transform) : nullptr;
}

template <typename T>
T* CreateBlueprintDeferred(UWorld* World, const FString& Name, const FTransform& Transform = FTransform::Identity)
{
	const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *Name);
	return (World && Blueprint) ? World->SpawnActorDeferred<T>(Blueprint->GeneratedClass, Transform) : nullptr;
}

int32 GetActionBindingIndexByName(UInputComponent* InputComp, const FString& ActionName, EInputEvent InputEvent);

int32 GetAxisBindingIndexByName(UInputComponent* InputComp, const FString& AxisName);

void ExecuteInputPressedAction(UInputComponent* InputComponent, const FString& ActionName, const FKey& Key);

UWorld* GetAnyGameWorld();

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params);

FString GetTestDataDir();
}
