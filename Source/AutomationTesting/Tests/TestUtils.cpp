#include "TestUtils.h"
#include "Misc/OutputDeviceNull.h"

namespace UE::TEST
{
int32 GetActionBindingIndexByName(UInputComponent* InputComp, const FString& ActionName, EInputEvent InputEvent)
{
	if (!InputComp) return INDEX_NONE;

	for (int32 i = 0; i < InputComp->GetNumActionBindings(); ++i)
	{
		const FInputActionBinding Action = InputComp->GetActionBinding(i);
		if (Action.GetActionName().ToString().Equals(ActionName) && Action.KeyEvent == InputEvent)
		{
			return i;
		}
	}

	return INDEX_NONE;
}

int32 GetAxisBindingIndexByName(UInputComponent* InputComp, const FString& AxisName)
{
	if (!InputComp) return INDEX_NONE;

	const int32 AxisIndex = InputComp->AxisBindings.IndexOfByPredicate(
		[=](const FInputAxisBinding& AxisBind) { return AxisBind.AxisName.ToString().Equals(AxisName); });

	return AxisIndex;
}

UWorld* GetAnyGameWorld()
{
	UWorld* TestWorld = nullptr;
	const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
	for (const FWorldContext& Context : WorldContexts)
	{
		if ((Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game) && Context.World() != nullptr)
		{
			TestWorld = Context.World();
			break;
		}
	}

	return TestWorld;
}

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params)
{
	if (!Object) { return; }

	FString Command = FString::Printf(TEXT("%s"), *FuncName);
	for (const auto Param : Params)
	{
		Command.Append(" ").Append(Param);
	}
	FOutputDeviceNull OutputDeviceNull;
	Object->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);
}

FString GetTestDataDir()
{
	return FPaths::GameSourceDir().Append("AutomationTesting/Tests/Data/");
}
}
