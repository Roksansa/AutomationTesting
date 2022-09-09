#include "TestUtils.h"

namespace UE::TEST
{
UWorld* GetAnyGameWorld()
{
	UWorld* TestWorld = nullptr;
	const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
	for (const FWorldContext& Context : WorldContexts)
	{
		if ((Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game) && Context.World() != nullptr)
		{
			TestWorld = Context.World();
			break;
		}
	}

	return TestWorld;
}
}
