// Automation testing project copyright

#if WITH_AUTOMATION_TESTS
#include "AutomationTesting/Tests/ATCharacterInputTest.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"
#include "GameFramework/Character.h"
#include "Inventory/ATInventoryItem.h"
#include "Kismet/GameplayStatics.h"
#include "JsonUtils.h"
#include "Input/ATInputRecordingUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCanBeTakenOnJump, "OriginGame.Character.Input.InventoryItemCanBeTakenOnJump",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCantBeTakenOnJumpIfTooHigh,
	"OriginGame.Character.Input.InventoryItemCantBeTakenOnJumpIfTooHigh",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllItemsAreTakenOnMovement, "OriginGame.Character.Input.AllItemsAreTakenOnMovement",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FAllItemsAreTakenOnRecordingMovement, "OriginGame.Character.Input.AllItemsAreTakenOnRecordingMovement",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace UE::TEST;

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpLatentCommand, ACharacter*, Character);

bool FJumpLatentCommand::Update()
{
	if (!Character) return true;

	const int32 ActionIndex = GetActionBindingIndexByName(Character->InputComponent, "Jump", EInputEvent::IE_Pressed);
	if (ActionIndex != INDEX_NONE)
	{
		const auto JumpActionBind = Character->InputComponent->GetActionBinding(ActionIndex);
		JumpActionBind.ActionDelegate.Execute(EKeys::SpaceBar);
	}
	return true;
}

bool FInventoryItemCanBeTakenOnJump::RunTest(const FString& Parameters)
{
	const auto Level = LevelScope("/Game/AutomationTesting/Test/EmptyTestLevel1");

	UWorld* World = GetAnyGameWorld();
	if (!TestNotNull("World exists", World)) { return false; }

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("Character exists", Character)) { return false; }

	TArray<AActor*> InventoryItems;
	UGameplayStatics::GetAllActorsOfClass(World, AATInventoryItem::StaticClass(), InventoryItems);
	if (!TestEqual("Only one item exists", InventoryItems.Num(), 1)) { return false; }

	auto Func = [=]()
	{
		TArray<AActor*> NewInventoryItems;
		UGameplayStatics::GetAllActorsOfClass(World, AATInventoryItem::StaticClass(), NewInventoryItems);
		TestTrueExpr(NewInventoryItems.Num() == 0);
	};
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(Func, 2.0f));

	return true;
}

bool FInventoryItemCantBeTakenOnJumpIfTooHigh::RunTest(const FString& Parameters)
{
	const auto Level = LevelScope("/Game/AutomationTesting/Test/EmptyTestLevel2");

	UWorld* World = GetAnyGameWorld();
	if (!TestNotNull("World exists", World)) { return false; }

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("Character exists", Character)) { return false; }

	TArray<AActor*> InventoryItems;
	UGameplayStatics::GetAllActorsOfClass(World, AATInventoryItem::StaticClass(), InventoryItems);
	if (!TestEqual("Only one item exists", InventoryItems.Num(), 1)) { return false; }

	auto Func = [=]()
	{
		TArray<AActor*> NewInventoryItems;
		UGameplayStatics::GetAllActorsOfClass(World, AATInventoryItem::StaticClass(), NewInventoryItems);
		TestTrueExpr(NewInventoryItems.Num() == 1);
	};
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(Func, 2.0f));

	return true;
}

bool FAllItemsAreTakenOnMovement::RunTest(const FString& Parameters)
{
	const auto Level = LevelScope("/Game/AutomationTesting/Test/EmptyTestLevel3");

	UWorld* World = GetAnyGameWorld();
	if (!TestNotNull("World exists", World)) { return false; }

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("Character exists", Character)) { return false; }

	TArray<AActor*> InventoryItems;
	UGameplayStatics::GetAllActorsOfClass(World, AATInventoryItem::StaticClass(), InventoryItems);
	TestTrueExpr(InventoryItems.Num() == 7);

	const int32 MoveForwardIndex = GetAxisBindingIndexByName(Character->InputComponent, "Move Forward / Backward");
	TestTrueExpr(MoveForwardIndex != INDEX_NONE);

	const int32 MoveRightIndex = GetAxisBindingIndexByName(Character->InputComponent, "Move Right / Left");
	TestTrueExpr(MoveRightIndex != INDEX_NONE);

	auto FuncRun = [MyCharacter = Character](int32 MoveIndex)
	{
		MyCharacter->InputComponent->AxisBindings[MoveIndex].AxisDelegate.Execute(1.0f);
	};

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FATUntilLatentCommand<int32>(FuncRun, [](){}, 3.f, MoveForwardIndex));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FATUntilLatentCommand<int32>(FuncRun, [](){}, 3.f, MoveRightIndex));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

	auto FuncEndTest = [=]()
	{
		TArray<AActor*> NewInventoryItems;
		UGameplayStatics::GetAllActorsOfClass(World, AATInventoryItem::StaticClass(), NewInventoryItems);
		TestTrueExpr(NewInventoryItems.Num() == 0);
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(FuncEndTest));

	return true;
}

class FSimulateMovementLatentCommand : public IAutomationLatentCommand
{
public:
	FSimulateMovementLatentCommand(UWorld* InWorld, UInputComponent* InInputComponent, const TArray<FBindingsData>& InBindingsData)
		: World(InWorld), InputComponent(InInputComponent), BindingsData(InBindingsData)
	{
	}

	virtual bool Update() override
	{
		if (!World || !InputComponent) return true;

		if (WorldStartTime == 0.0f)
		{
			WorldStartTime = World->TimeSeconds;
		}

		while (World->TimeSeconds - WorldStartTime > BindingsData[Index].GlobalTime || FMath::IsNearlyZero(
			       (World->TimeSeconds - WorldStartTime) - BindingsData[Index].GlobalTime, 3 * KINDA_SMALL_NUMBER))
		{
			for (const auto& AxisValue : BindingsData[Index].AxisValue)
			{
				const int32 BindingIndex = GetAxisBindingIndexByName(InputComponent, AxisValue.Name.ToString());
				if (BindingIndex == INDEX_NONE) { continue; }
				InputComponent->AxisBindings[BindingIndex].AxisDelegate.Execute(AxisValue.Value);
			}

			if (Index > 0)
			{
				for (int32 i = 0; i < BindingsData[Index].ActionValue.Num(); ++i)
				{
					const FActionData& ActionData = BindingsData[Index].ActionValue[i];
					const FActionData& PrevActionData = BindingsData[Index - 1].ActionValue[i];
					if (!ActionData.State || ActionData.State == PrevActionData.State) { continue; }
					const FString ActionName = ActionData.Name.ToString();
					const int32 ActionIndex = GetActionBindingIndexByName(InputComponent, ActionName, ActionData.InputEvent);
					if (ActionIndex == INDEX_NONE) { continue; }
					InputComponent->GetActionBinding(ActionIndex).ActionDelegate.Execute(ActionData.Key);
				}
			}

			if (++Index >= BindingsData.Num()) return true;
		}

		return false;
	}

private:
	const UWorld* World;
	UInputComponent* InputComponent;
	const TArray<FBindingsData> BindingsData;
	int32 Index{0};
	float WorldStartTime{0.0f};
};

void FAllItemsAreTakenOnRecordingMovement::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	struct FTestData
	{
		FString TestName;
		FString MapPath;
		FString JsonName;
	};

	const TArray<FTestData> TestData = //
	{
		{"MainMap", "/Game/ThirdPerson/Maps/ThirdPersonMap", "CharacterTestInput.json"},                      //
		{"CustomMap", "/Game/AutomationTesting/Test/ThirdPersonMap_Custom", "CharacterTestInputCustom.json"}, //
	};

	for (const auto OneTestData : TestData)
	{
		OutBeautifiedNames.Add(OneTestData.TestName);
		OutTestCommands.Add(FString::Printf(TEXT("%s,%s"), *OneTestData.MapPath, *OneTestData.JsonName));
	}
}

bool FAllItemsAreTakenOnRecordingMovement::RunTest(const FString& Parameters)
{
	const FString ExpectedWarnMsg = FString::Printf(TEXT("Invalid world bounds"));
	AddExpectedError(ExpectedWarnMsg, EAutomationExpectedErrorFlags::Contains, 2);

	TArray<FString> ParsedParams;
	Parameters.ParseIntoArray(ParsedParams, TEXT(","));
	if (!TestTrue("Map name and JSON params should exist", ParsedParams.Num() == 2)) return false;

	const auto Level = LevelScope(ParsedParams[0]);

	UWorld* World = GetAnyGameWorld();
	if (!TestNotNull("World exists", World)) { return false; }

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("Character exists", Character)) { return false; }

	TArray<AActor*> InventoryItems;
	UGameplayStatics::GetAllActorsOfClass(World, AATInventoryItem::StaticClass(), InventoryItems);
	TestTrueExpr(InventoryItems.Num() == 5);

	const FString FileName = GetTestDataDir().Append(ParsedParams[1]);
	FInputData InputData;
	if (!JsonUtils::ReadInputData(FileName, InputData)) { return false; }
	if (!TestTrue("Input data is not empty", InputData.Bindings.Num() > 0)) { return false; }

	GEngine->bUseFixedFrameRate = true;
	GEngine->FixedFrameRate = InputData.FixedCountFps;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (!TestNotNull("PlayerController exists", PlayerController)) { return false; }

	Character->SetActorTransform(InputData.InitialTransform);
	PlayerController->SetControlRotation(InputData.InitialTransform.Rotator());

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

	//simulateinput
	//work only fixed rate fps!!! equal write and read!
	ADD_LATENT_AUTOMATION_COMMAND(FSimulateMovementLatentCommand(World, Character->InputComponent, InputData.Bindings));

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	auto FuncEndTest = [=]()
	{
		TArray<AActor*> NewInventoryItems;
		UGameplayStatics::GetAllActorsOfClass(World, AATInventoryItem::StaticClass(), NewInventoryItems);
		TestTrueExpr(NewInventoryItems.Num() == 1);
		GEngine->ReloadConfig();
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(FuncEndTest));

	return true;
}

#endif
