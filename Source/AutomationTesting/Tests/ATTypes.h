#pragma once

#include "CoreMinimal.h"
#include "ATTypes.generated.h"

UENUM(BlueprintType)
enum class EInventoryItemType : uint8
{
	SPHERE = 0,
	CUBE = 1,
	CYLINDER = 2,
	CONE,
};

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInventoryItemType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin = "0"))
	int32 Score;

	FString ToString() const
	{
		return FString::Printf(TEXT("(Type=%i,Score=%i)"), Type, Score);
	}
};
