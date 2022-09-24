// Automation testing project copyright

#if WITH_AUTOMATION_TESTS
#include "AutomationTesting/Tests/ATSettingUITests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "TestUtils.h"
#include "UITestUtils.h"
#include "Settings/ATGameSetting.h"
#include "Settings/ATGameUserSettings.h"
#include "UI/ATPauseWidget.h"
#include "UI/ATSettingOptionWidget.h"
#include "UI/ATSettingsWidget.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseMenuShouldBeCorrectWork, "OriginGame.UI.PauseMenuShouldBeCorrectWork",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAutoBenchmarkShouldBeCorrectWork, "OriginGame.UI.AutoBenchmarkShouldBeCorrectWork",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::MediumPriority);

constexpr char* NewMapName = "/Game/ThirdPerson/Maps/ThirdPersonMap";

using namespace UE::TEST;

bool FPauseMenuShouldBeCorrectWork::RunTest(const FString& Parameters)
{
	const FString ExpectedWarnMsg = FString::Printf(TEXT("Invalid world bounds"));
	AddExpectedError(ExpectedWarnMsg, EAutomationExpectedErrorFlags::Contains, 2);

	const auto OpenedLevel = LevelScope{NewMapName};
	UWorld* World = GetAnyGameWorld();
	if (!TestNotNull("World exist", World)) { return false; }
	const APlayerController* PlayerController = GetAnyGameWorld()->GetFirstPlayerController();
	if (!TestNotNull("PlayerController exist", PlayerController)) { return false; }

	const UATPauseWidget* PauseWidget = FindWidgetByClass<UATPauseWidget>();
	if (!TestNotNull("PauseWidget exist", PauseWidget)) { return false; }

	TestTrueExpr(PauseWidget->GetVisibility() == ESlateVisibility::Collapsed);

	ExecuteInputPressedAction(PlayerController->InputComponent, "Pause", EKeys::Escape);
	TestTrueExpr(PauseWidget->GetVisibility() == ESlateVisibility::Visible);

	ExecuteInputPressedAction(PlayerController->InputComponent, "Pause", EKeys::Escape);
	TestTrueExpr(PauseWidget->GetVisibility() == ESlateVisibility::Collapsed);
	return true;
}


bool FAutoBenchmarkShouldBeCorrectWork::RunTest(const FString& Parameters)
{
	const FString ExpectedWarnMsg = FString::Printf(TEXT("Invalid world bounds"));
	AddExpectedError(ExpectedWarnMsg, EAutomationExpectedErrorFlags::Contains, 2);

	const auto OpenedLevel = LevelScope{NewMapName};
	UWorld* World = GetAnyGameWorld();
	if (!TestNotNull("World exist", World)) { return false; }
	const APlayerController* PlayerController = GetAnyGameWorld()->GetFirstPlayerController();
	if (!TestNotNull("PlayerController exist", PlayerController)) { return false; }

	const UATPauseWidget* PauseWidget = FindWidgetByClass<UATPauseWidget>();
	if (!TestNotNull("PauseWidget exist", PauseWidget)) { return false; }

	ExecuteInputPressedAction(PlayerController->InputComponent, "Pause", EKeys::Escape);

	const UATSettingsWidget* SettingsWidget = FindWidgetByClass<UATSettingsWidget>();
	const UButton* BenchmarkButton = Cast<UButton>(FindWidgetByName(SettingsWidget, "StartBenchmarkButton"));
	BenchmarkButton->OnClicked.Broadcast();

	TArray<int32> SavedSettingValues;
	const TArray<UATGameSetting*>& VideoSettings = UATGameUserSettings::Get()->GetGraphicsSettings();
	for (const auto& Setting : VideoSettings)
	{
		SavedSettingValues.Add(Setting->GetCurrentOption().Value);
	}

	const FPointerEvent MouseEvent = FPointerEvent(0, FVector2D(0, 0), FVector2D(0, 0), TSet<FKey>{},
		EKeys::LeftMouseButton, 0.f, FModifierKeysState{});

	for (int32 i = 0; i < VideoSettings.Num(); ++i)
	{
		const UVerticalBox* VerticalBox = Cast<UVerticalBox>(FindWidgetByName(SettingsWidget, "SettingsVerticalBox"));
		const UATSettingOptionWidget* SettingOptionWidget = Cast<UATSettingOptionWidget>(VerticalBox->GetChildAt(i));
		UButton* NextSettingButton = Cast<UButton>(FindWidgetByName(SettingOptionWidget, "NextSettingButton"));
		SButton* SNextSettingButton = static_cast<SButton*>(&(NextSettingButton->TakeWidget().Get()));
		SNextSettingButton->SimulateClick();
		TestTrueExpr(SavedSettingValues[i] != VideoSettings[i]->GetCurrentOption().Value);
	}

	BenchmarkButton->OnClicked.Broadcast();
	for (int32 i = 0; i < VideoSettings.Num(); ++i)
	{
		TestTrueExpr(SavedSettingValues[i] == VideoSettings[i]->GetCurrentOption().Value);
	}

	return true;
}

#endif
