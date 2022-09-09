// Automation testing project copyright

#if WITH_AUTOMATION_TESTS
#include "AutomationTesting/Tests/ATInventoryItemTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "TestUtils.h"
#include "Inventory/ATInventoryItem.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"

//fix warning - enable streaming in world settings
//map close automation after ended test -- upd 5.0 ue automationControllerManager - 501 line - GUnrealEd->RequestEndPlayMap();

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCantBeCreated, "OriginGame.Inventory.Item.CppActorCantBeCreated",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectly, "OriginGame.Inventory.Item.BlueprintShouldBeSetupCorrectly",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

constexpr char* NewMapName = "/Game/AutomationTesting/Test/EmptyTestLevel";
constexpr char* InventoryItemBPName = "Blueprint'/Game/AutomationTesting/Actors/BP_InventoryItem.BP_InventoryItem'";
constexpr char* InventoryItemBPTestName = "Blueprint'/Game/Tests/BP_Test_TPSInventoryItem.BP_Test_TPSInventoryItem'";

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

bool FCppActorCantBeCreated::RunTest(const FString& Parameters)
{
	const FString ExpectedWarnMsg =
		FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *AATInventoryItem::StaticClass()->GetName());
	AddExpectedError(ExpectedWarnMsg, EAutomationExpectedErrorFlags::Exact);

	LevelScope{NewMapName};
	UWorld* World = UE::TEST::GetAnyGameWorld();
	if (!TestNotNull("World exist", World))
	{
		return false;
	}

	const FTransform InitialTransform{FVector{100.f}};
	const AATInventoryItem* InventoryItem = World->SpawnActor<AATInventoryItem>(AATInventoryItem::StaticClass(), InitialTransform);
	if (!TestNull("Inventory item exist", InventoryItem))
	{
		return false;
	}

	return true;
}

bool FBlueprintShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(10.f));
	LevelScope{NewMapName};
	UWorld* World = UE::TEST::GetAnyGameWorld();
	if (!TestNotNull("World exist", World))
	{
		return false;
	}

	const FTransform InitialTransform{FVector{60.f}};
	const AATInventoryItem* InventoryItem = UE::TEST::CreateBlueprint<AATInventoryItem>(World, InventoryItemBPName, InitialTransform);
	if (!TestNotNull("Inventory item exist", InventoryItem))
	{
		return false;
	}

	const auto CollisionComp = InventoryItem->FindComponentByClass<USphereComponent>();
	if (!TestNotNull("Sphere component exists", CollisionComp)) return false;

	TestTrueExpr(CollisionComp->GetUnscaledSphereRadius() >= 30.0f);
	TestTrueExpr(CollisionComp->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);
	TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic) == ECollisionResponse::ECR_Overlap);
	TestTrueExpr(CollisionComp->GetGenerateOverlapEvents());
	TestTrueExpr(InventoryItem->GetRootComponent() == CollisionComp);

	UE::TEST::ForEach<ECollisionChannel>([&](const ECollisionChannel EnumValue, const FName& Name)
	{
		if (EnumValue != ECollisionChannel::ECC_OverlapAll_Deprecated && EnumValue != ECollisionChannel::ECC_MAX)
		{
			// 	const FString EnumTypeName = UEnum::GetValueAsString(EnumValue);
			// 	const FString InfoString = FString::Printf(TEXT("Deprecated %s"), *EnumTypeName);
			// TestEqual(InfoString, CollisionComp->GetCollisionResponseToChannel(EnumValue), ECollisionResponse::ECR_Overlap);
			TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(EnumValue) == ECollisionResponse::ECR_Overlap);
		}
	});

	const auto TextRenderComp = InventoryItem->FindComponentByClass<UTextRenderComponent>();
	if (!TestNotNull("Text render component exists", TextRenderComp)) return false;

	const auto StaticMeshComp = InventoryItem->FindComponentByClass<UStaticMeshComponent>();
	if (!TestNotNull("Static mesh component exists", StaticMeshComp)) return false;

	TestTrueExpr(StaticMeshComp->GetCollisionEnabled() == ECollisionEnabled::NoCollision);
	return true;
}

#endif
