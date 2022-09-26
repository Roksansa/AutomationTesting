// Automation testing project copyright

#if WITH_AUTOMATION_TESTS

#include "ATScreenshotsTest.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Camera/CameraActor.h"
#include "TestUtils.h"
#include "UITestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthWidgetShouldBeRenderedCorrectlyAfterDamage,
	"OriginGame.Screenshots.HealthWidgetShouldBeRenderedCorrectlyAfterDamage",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingsWidgetBeRenderedWhenGamePaused, "OriginGame.Screenshots.SettingsWidgetBeRenderedWhenGamePaused",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace UE::TEST;

constexpr char* NewMapName = "/Game/ThirdPerson/Maps/ThirdPersonMap";

bool FHealthWidgetShouldBeRenderedCorrectlyAfterDamage::RunTest(const FString& Parameters)
{
	const FString ExpectedWarnMsg = FString::Printf(TEXT("Invalid world bounds"));
	AddExpectedError(ExpectedWarnMsg, EAutomationExpectedErrorFlags::Contains, 2);

	const auto OpenedLevel = LevelScope{NewMapName};
	UWorld* World = GetAnyGameWorld();
	if (!TestNotNull("World exist", World)) { return false; }
	const APlayerController* PlayerController = GetAnyGameWorld()->GetFirstPlayerController();
	if (!TestNotNull("PlayerController exist", PlayerController)) { return false; }
	if (!TestNotNull("Pawn exists", PlayerController->GetPawn())) return false;

	constexpr float DamageAmount = 40.0f;
	PlayerController->GetPawn()->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
	ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("HealthWidgetScreenshot"));
	return true;
}

bool FSettingsWidgetBeRenderedWhenGamePaused::RunTest(const FString& Parameters)
{
	const FString ExpectedWarnMsg = FString::Printf(TEXT("Invalid world bounds"));
	AddExpectedError(ExpectedWarnMsg, EAutomationExpectedErrorFlags::Contains, 2);

	const auto OpenedLevel = LevelScope{NewMapName};
	UWorld* World = GetAnyGameWorld();
	if (!TestNotNull("World exist", World)) { return false; }
	const APlayerController* PlayerController = GetAnyGameWorld()->GetFirstPlayerController();
	if (!TestNotNull("PlayerController exist", PlayerController)) { return false; }

	ExecuteInputPressedAction(PlayerController->InputComponent, "Pause", EKeys::Escape);
	ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("SettingWidgetScreenshot"));
	return true;
}

#endif
