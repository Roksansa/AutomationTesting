// Fill out your copyright notice in the Description page of Project Settings.


#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)
#include "SandboxTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"

// IMPLEMENT_SIMPLE_AUTOMATION_TEST_PRIVATE( TClass, TBaseClass, PrettyName, TFlags, FileName, LineNumber )

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, "OriginGame.Math.MaxInt",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrt, "OriginGame.Math.Sqrt",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FMathMaxInt::RunTest(const FString& Parameters)
{
	AddInfo("Max [int] func testing");

	const TArray<UE::TEST::TestPayload<TInterval<int32>, int32>> TestData{
		{{13, 25}, 25},
		{{25, 25}, 25},
		{{0, 0}, 0},
		{{123, 0}, 123},
		{{-12345, 0}, 0},
		{{-44, -64}, -44},
		{{-5, -5}, -5},
		{{-187, 190}, 190}
	};

	for (const auto& Data : TestData)
	{
		TestTrueExpr(FMath::Max(Data.TestValue.Min, Data.TestValue.Max) == Data.ExpectedValue);
	}

	return true;
}

bool FMathSqrt::RunTest(const FString& Parameters)
{
	AddInfo("Sqrt func testing");

	typedef float Tolerance;
	const TArray<UE::TEST::TestPayload<TInterval<float>, float>> TestData{
		{{4.f, Tolerance{KINDA_SMALL_NUMBER}}, 2.f},
		{{3.f, Tolerance{0.1f}}, 1.7f},
		{{3.f, Tolerance{0.01f}}, 1.73f},
		{{3.f, Tolerance{1.e-5f}}, 1.73205f}
	};

	for (const auto& Data : TestData)
	{
		const FString InfoString = FString::Printf(TEXT(" Sqrt (text value : %f , tolerance : %f), expected value: %f"),
			Data.TestValue.Min, Data.TestValue.Max, Data.ExpectedValue);
		TestEqual(InfoString, FMath::Sqrt(Data.TestValue.Min), Data.ExpectedValue, Data.TestValue.Max);
	}
	return true;
}
#endif
