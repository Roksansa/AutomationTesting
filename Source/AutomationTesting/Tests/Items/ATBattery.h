// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace UE::TEST
{
class ATBattery
{
public:
	ATBattery() = default;
	ATBattery(float PercentIn);

	void Charge();
	void UnCharge();

	float GetPercent() const;
	FColor GetColor() const;
	FString ToString() const;

	bool operator>(const ATBattery& rhs) const;
	bool operator>=(const ATBattery& rhs) const;
	bool operator==(const ATBattery& rhs) const;

	bool operator<(const ATBattery& rhs) const;
	bool operator<=(const ATBattery& rhs) const;
	bool operator!=(const ATBattery& rhs) const;

private:
	float Percent{1.f};

	void SetPercent(float PercentIn);
};
}
