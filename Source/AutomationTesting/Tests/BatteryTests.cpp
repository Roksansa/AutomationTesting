// Fill out your copyright notice in the Description page of Project Settings.


#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "AutomationTesting/Tests/BatteryTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Items/ATBattery.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatteryTests, "OriginGame.Items.Battery",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FBatteryTests::RunTest(const FString& Parameters)
{
	using namespace UE::TEST;

	AddInfo("Battery with default ctor");
	const ATBattery BatteryDefault;
	TestTrueExpr(FMath::IsNearlyEqual(BatteryDefault.GetPercent(), 1.f));
	TestTrueExpr(BatteryDefault.GetColor() == FColor::Green);
	TestTrueExpr(BatteryDefault.ToString().Equals("100%"));

	AddInfo("Battery with params ctor");
	const auto BatteryTestFunc = [this](float Percent, const FColor& Color, const FString& PercentString)
	{
		const ATBattery BatteryObject{Percent};
		TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), FMath::Clamp(Percent, 0.f, 1.f)));
		TestTrueExpr(BatteryObject.GetColor() == Color);
		TestTrueExpr(BatteryObject.ToString().Equals(PercentString));
	};

	BatteryTestFunc(1.f, FColor::Green, "100%");
	BatteryTestFunc(0.8f, FColor::Yellow, "80%");
	BatteryTestFunc(0.39f, FColor::Yellow, "39%");
	BatteryTestFunc(0.3f, FColor::Red, "30%");
	BatteryTestFunc(-1.f, FColor::Red, "0%");
	BatteryTestFunc(102.f, FColor::Green, "100%");

	AddInfo("Battery func charge / uncharge");
	ATBattery BatteryObject(0.6f);
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.6f));
	BatteryObject.UnCharge();
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.5f));
	BatteryObject.Charge();
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.6f));

	AddInfo("Battery func uncharge. Corner cases");
	for (int32 i = 0; i < 10; ++i)
	{
		BatteryObject.UnCharge();
	}
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.0f));

	AddInfo("Battery func charge. Corner cases");
	for (int32 i = 0; i < 15; ++i)
	{
		BatteryObject.Charge();
	}
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 1.0f));

	AddInfo("Battery compare");
	const ATBattery BatteryLow(0.3f);
	const ATBattery BatteryHigh(0.9f);

	TestTrueExpr(BatteryLow <= BatteryHigh);
	TestTrueExpr(FMath::Min(BatteryLow, BatteryHigh) != BatteryHigh);

	AddInfo("Battery compare by address memory");
	TestNotSame("Not the same batteries", BatteryLow, BatteryHigh);

	const ATBattery BatteryHighCopy(0.9f);
	TestTrueExpr(BatteryHigh == BatteryHighCopy);
	TestNotSame("Not the same batteries", BatteryHighCopy, BatteryHigh);
	
	const ATBattery& BatteryHighRef =BatteryHigh;
	TestSame("The same batteries", BatteryHighRef, BatteryHigh);
	
	return true;
}

#endif
