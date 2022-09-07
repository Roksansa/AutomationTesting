// Fill out your copyright notice in the Description page of Project Settings.


#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)
#include "ScienceFuncLibTests.h"
#include "Science/ATScienceFunctionLibrary.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"

// IMPLEMENT_SIMPLE_AUTOMATION_TEST_PRIVATE( TClass, TBaseClass, PrettyName, TFlags, FileName, LineNumber )

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciSimple, "OriginGame.Science.Fibonacci.Simple",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciStress, "OriginGame.Science.Fibonacci.Stress",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::LowPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciLogHasErrors, "OriginGame.Science.Fibonacci.LogHasErrors",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FFibonacciSimple::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci Simple func testing");

	// 0 1 1 2 3 5 8 ...

	TestTrueExpr(UATScienceFunctionLibrary::Fibonacci(0) == 0);
	TestTrueExpr(UATScienceFunctionLibrary::Fibonacci(1) == 1);
	TestTrueExpr(UATScienceFunctionLibrary::Fibonacci(2) == 1);
	TestTrueExpr(UATScienceFunctionLibrary::Fibonacci(3) == 2);

	const TArray<UE::TEST::TestPayload<int32, int32>> TestData{{0, 0}, {1, 1}, {2, 1}, {3, 2}, {4, 3}, {5, 5}};
	for (const auto& Data : TestData)
	{
		const FString InfoString = FString::Printf(TEXT("text value : %i, expected value: %i"), Data.TestValue, Data.ExpectedValue);
		TestEqual(InfoString, UATScienceFunctionLibrary::Fibonacci(Data.TestValue), Data.ExpectedValue);
	}
	return true;
}

bool FFibonacciStress::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci Stress func testing");

	// for (int32 i = 2; i < 40; i++)
	// {
	// 	TestTrueExpr(
	// 		UOScienceFunctionLibrary::Fibonacci(i) ==(UOScienceFunctionLibrary::Fibonacci(i-1) + UOScienceFunctionLibrary::Fibonacci(i-2)));
	// }
	int32 PrevPrevValue = 0;
	int32 PrevValue = 1;
	for (int32 i = 2; i < 40; i++)
	{
		int32 CurValue = UATScienceFunctionLibrary::Fibonacci(i);
		TestTrueExpr(CurValue == (PrevValue + PrevPrevValue));
		PrevPrevValue = PrevValue;
		PrevValue = CurValue;
	}

	return true;
}

bool FFibonacciLogHasErrors::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci Negative number on input func testing");

	AddExpectedError(TEXT("Invalid input for UOScienceFunctionLibrary::Fibonacci"), EAutomationExpectedErrorFlags::Contains);
	UATScienceFunctionLibrary::Fibonacci(-1);
	return true;
}

#endif
