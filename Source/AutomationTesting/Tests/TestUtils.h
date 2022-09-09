#pragma once


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
	for (int32 i = 0; i < Enum->NumEnums(); ++i)
	{
		Function(static_cast<EnumType>(Enum->GetValueByIndex(i)), Enum->GetNameByIndex(i));
	}
}

template <typename T>
T* CreateBlueprint(UWorld* World, const FString& Name, const FTransform& Transform = FTransform::Identity)
{
	const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *Name);
	return (World && Blueprint) ? World->SpawnActor<T>(Blueprint->GeneratedClass, Transform) : nullptr;
}

UWorld* GetAnyGameWorld();
}
