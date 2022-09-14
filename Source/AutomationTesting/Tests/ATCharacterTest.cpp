// Automation testing project copyright

#if WITH_AUTOMATION_TESTS

#include "AutomationTesting/Tests/ATCharacterTest.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "TestUtils.h"
#include "AutomationTesting/AutomationTestingCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Inventory/ATInventoryItem.h"
#include "Kismet/GameplayStatics.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthMightBeChangedWithDamage, "OriginGame.Character.HealthMightBeChangedWithDamage",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacterCanBeKilled, "OriginGame.Character.CharacterCanBeKilled",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacterCanBeAutohealed, "OriginGame.Character.CharacterCanBeAutohealed",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

constexpr char* NewMapName = "/Game/AutomationTesting/Test/EmptyTestLevel";
constexpr char* CharacterTestBPName = "Blueprint'/Game/AutomationTesting/Test/BP_ThirdPC_Test.BP_ThirdPC_Test'";

using namespace UE::TEST;

bool FHealthMightBeChangedWithDamage::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f));
	LevelScope{NewMapName};
	UWorld* World = GetAnyGameWorld();
	if (!TestNotNull("World exist", World)) { return false; }

	const FTransform InitialTransform{FVector{100.f, 100.f, 100.f}};
	AAutomationTestingCharacter* Character = CreateBlueprintDeferred<AAutomationTestingCharacter>(World, CharacterTestBPName,
		InitialTransform);
	if (!TestNotNull("Character exist", Character)) { return false; }

	constexpr FHealthData HealthData{101.f, 11.f, 0.6f, 4.f};
	CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});
	Character->FinishSpawning(InitialTransform);

	TestEqual("Health is fully", Character->GetHealthPercent(), 1.f);

	constexpr float DamageAmount = 10.f;
	Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
	TestEqual("Damage was taken", Character->GetHealthPercent(), 1.f - DamageAmount / HealthData.MaxHealth);

	return true;
}

bool FCharacterCanBeKilled::RunTest(const FString& Parameters)
{
	const auto OpenedLevel = LevelScope{NewMapName};
	UWorld* World = GetAnyGameWorld();
	if (!TestNotNull("World exist", World)) { return false; }

	const FTransform InitialTransform{FVector{100.f, 100.f, 100.f}};
	AAutomationTestingCharacter* Character = CreateBlueprintDeferred<AAutomationTestingCharacter>(World, CharacterTestBPName,
		InitialTransform);
	if (!TestNotNull("Character exist", Character)) { return false; }

	constexpr FHealthData HealthData{100.f, 11.f, 0.6f, 2.f};
	CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});
	Character->FinishSpawning(InitialTransform);

	constexpr float DamageAmount = HealthData.MaxHealth + 1;
	Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);

	TestEqual("Character dead - health = 0", Character->GetHealthPercent(), 0.f);
	TestTrueExpr(Character->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_None);

	const auto CollisionComp = Character->FindComponentByClass<UCapsuleComponent>();
	if (!TestNotNull("CollisionComp exist", CollisionComp)) { return false; }
	UE::TEST::ForEach<ECollisionChannel>([&](const ECollisionChannel EnumValue, const FName& Name)
	{
		if (EnumValue != ECollisionChannel::ECC_OverlapAll_Deprecated)
		{
			TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(EnumValue) == ECollisionResponse::ECR_Ignore);
		}
	});

	TestTrueExpr(Character->GetMesh() && Character->GetMesh()->GetCollisionEnabled() == ECollisionEnabled::QueryAndPhysics);
	TestTrueExpr(Character->GetMesh() && Character->GetMesh()->IsSimulatingPhysics());
	TestEqual("Life span correctly", Character->GetLifeSpan(), HealthData.LifeSpan);

	auto Func = [Character]()
	{
		if (IsValid(Character))
		{
			UE_LOG(LogEngineAutomationTests, Error, TEXT("Character is not destroyed %i"), Character->GetLifeSpan());
		}
	};
	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(Func, HealthData.LifeSpan + 0.3f));
	return true;
}

bool FCharacterCanBeAutohealed::RunTest(const FString& Parameters)
{
	const auto OpenedLevel = LevelScope{NewMapName};
	UWorld* World = GetAnyGameWorld();
	if (!TestNotNull("World exist", World)) { return false; }

	const FTransform InitialTransform{FVector{100.f, 100.f, 100.f}};
	AAutomationTestingCharacter* Character = CreateBlueprintDeferred<AAutomationTestingCharacter>(World, CharacterTestBPName,
		InitialTransform);
	if (!TestNotNull("Character exist", Character)) { return false; }

	constexpr FHealthData HealthData{100.f, 11.f, 0.5f, 4.f};
	CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});
	Character->FinishSpawning(InitialTransform);

	constexpr float DamageAmount = 50;
	Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
	TestEqual("Damage was taken", Character->GetHealthPercent(), 1.f - DamageAmount / HealthData.MaxHealth);

	const float HealthDiff = HealthData.MaxHealth - (1.f - Character->GetHealthPercent());
	const float HealingDuration = HealthData.HealRate * HealthDiff / HealthData.HealModifier;

	auto Func = [Character]()
	{
		if (!FMath::IsNearlyEqual(Character->GetHealthPercent(), 1.f))
		{
			UE_LOG(LogEngineAutomationTests, Error, TEXT("Character health is not full"));
		}
	};
	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(Func, HealingDuration));

	return true;
}

#endif
