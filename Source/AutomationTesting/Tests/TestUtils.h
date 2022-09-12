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

UWorld* GetAnyGameWorld();

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params);

}
