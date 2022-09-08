// Fill out your copyright notice in the Description page of Project Settings.


#include "ATInventoryComponent.h"

// Sets default values for this component's properties
UATInventoryComponent::UATInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UATInventoryComponent::TryToAddItem(const FInventoryData& Data)
{
	if (Data.Score < 0)
	{
		return false;
	}
	if (!Inventory.Contains(Data.Type))
	{
		Inventory.Add(Data.Type, 0);
	}

	const int32 NextScore = Inventory[Data.Type] + Data.Score;
	if (/*InventoryLimits.Contains(Data.Type) &&*/ NextScore > InventoryLimits[Data.Type])
	{
		return false;
	}

	Inventory[Data.Type] += Data.Score;
	return true;
}

int32 UATInventoryComponent::GetInventoryAmountByType(EInventoryItemType Type) const
{
	return Inventory.Contains(Type) ? Inventory[Type] : 0;
}

// Called when the game starts
void UATInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

#if !UE_BUILD_SHIPPING
	const auto InvEnum = StaticEnum<EInventoryItemType>();
	check(InvEnum);
	for (int32 i = 0; i < InvEnum->NumEnums() - 1; i++)
	{
		const EInventoryItemType EnumType = static_cast<EInventoryItemType>(i);
		const FString EnumTypeName = UEnum::GetValueAsString(EnumType);
		const bool LimitCheckCond = InventoryLimits.Contains(EnumType) && InventoryLimits[EnumType] >= 0;
		checkf(LimitCheckCond, TEXT("Limit for %s doesn't exist or less then zero"), *EnumTypeName);
	}
#endif
}
