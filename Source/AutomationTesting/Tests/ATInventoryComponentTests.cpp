// Automation testing project copyright
#if WITH_AUTOMATION_TESTS

#include "TestUtils.h"
#include "AutomationTesting/Tests/ATInventoryComponentTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Inventory/ATInventoryComponent.h"
#include "ATTypes.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentCouldBeCreated, "OriginGame.Inventory.ComponentCouldBeCreated",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FItemScoresShouldBeZeroByDefault, "OriginGame.Inventory.ItemScoresShouldBeZeroByDefault",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNegativeScoresShouldntBeAdded, "OriginGame.Inventory.NegativeScoresShouldntBeAdded",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNotNegativeScoresShouldBeAdded, "OriginGame.Inventory.NotNegativeScoresShouldBeAdded",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScoreMoreThanLimitCantBeAdded, "OriginGame.Inventory.ScoreMoreThanLimitCantBeAdded",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);


namespace UE::TEST
{
class UATInventoryComponentTestable : public UATInventoryComponent
{
public:
	void SetLimits(const TMap<EInventoryItemType, int32>& Limits)
	{
		InventoryLimits = Limits;
	}
};

TMap<EInventoryItemType, int32> InitLimits(UATInventoryComponentTestable* InvComp, int32 LimitScore)
{
	TMap<EInventoryItemType, int32> InvLimits;
	UE::TEST::ForEach<EInventoryItemType>([&](const EInventoryItemType EnumValue, const FName& Name)
	{
		InvLimits.Add(EnumValue, LimitScore);
	});
	InvComp->SetLimits(InvLimits);
	return InvLimits;
}
}


bool FComponentCouldBeCreated::RunTest(const FString& Parameters)
{
	const UATInventoryComponent* InventoryComponent = NewObject<UATInventoryComponent>();
	return TestNotNull("Inventory component exists", InventoryComponent);
}

bool FItemScoresShouldBeZeroByDefault::RunTest(const FString& Parameters)
{
	const UATInventoryComponent* InventoryComponent = NewObject<UATInventoryComponent>();
	if (!TestNotNull("Inventory component exists", InventoryComponent))
	{
		return false;
	}

	UE::TEST::ForEach<EInventoryItemType>([&](const EInventoryItemType EnumValue, const FName& Name)
	{
		TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumValue) == 0);
	});

	return true;
}

bool FNegativeScoresShouldntBeAdded::RunTest(const FString& Parameters)
{
	UE::TEST::UATInventoryComponentTestable* InventoryComponent = NewObject<UE::TEST::UATInventoryComponentTestable>();
	if (!TestNotNull("Inventory component exists", InventoryComponent))
	{
		return false;
	}

	constexpr int32 ScoreLimitConstant = 55;
	TMap<EInventoryItemType, int32> InvLimits = InitLimits(InventoryComponent, ScoreLimitConstant);

	constexpr int32 PositiveConstant = 1;
	constexpr int32 NegativeConstant = -55;
	UE::TEST::ForEach<EInventoryItemType>([&](const EInventoryItemType EnumValue, const FName& Name)
	{
		TestTrueExpr(InventoryComponent->TryToAddItem({EnumValue, PositiveConstant}));
		TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumValue) == PositiveConstant);
		TestTrueExpr(!InventoryComponent->TryToAddItem({EnumValue, NegativeConstant}));
		TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumValue) == PositiveConstant);
	});

	return true;
}

bool FNotNegativeScoresShouldBeAdded::RunTest(const FString& Parameters)
{
	UE::TEST::UATInventoryComponentTestable* InventoryComponent = NewObject<UE::TEST::UATInventoryComponentTestable>();
	if (!TestNotNull("Inventory component exists", InventoryComponent))
	{
		return false;
	}

	constexpr int32 ScoreLimitConstant = 55;
	TMap<EInventoryItemType, int32> InvLimits = InitLimits(InventoryComponent, ScoreLimitConstant);

	constexpr int32 PositiveConstant = 1;
	UE::TEST::ForEach<EInventoryItemType>([&](const EInventoryItemType EnumValue, const FName& Name)
	{
		for (int32 i = 0; i < InvLimits[EnumValue]; ++i)
		{
			TestTrueExpr(InventoryComponent->TryToAddItem({EnumValue, PositiveConstant}));
			TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumValue) == i * PositiveConstant + PositiveConstant);
		}
	});

	return true;
}

bool FScoreMoreThanLimitCantBeAdded::RunTest(const FString& Parameters)
{
	UE::TEST::UATInventoryComponentTestable* InventoryComponent = NewObject<UE::TEST::UATInventoryComponentTestable>();
	if (!TestNotNull("Inventory component exists", InventoryComponent))
	{
		return false;
	}
	
	constexpr int32 ScoreLimitConstant = 55;
	TMap<EInventoryItemType, int32> InvLimits = InitLimits(InventoryComponent, ScoreLimitConstant);

	constexpr int32 PositiveConstant = 10;
	UE::TEST::ForEach<EInventoryItemType>([&](const EInventoryItemType EnumValue, const FName& Name)
	{
		TestTrueExpr(InventoryComponent->TryToAddItem({EnumValue, PositiveConstant}));
		TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumValue) == PositiveConstant);
		TestTrueExpr(!InventoryComponent->TryToAddItem({EnumValue, ScoreLimitConstant}));
		TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumValue) == PositiveConstant);
	});

	return true;
}

#endif
