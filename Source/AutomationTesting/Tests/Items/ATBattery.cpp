#include "ATBattery.h"

namespace UE::TEST
{
constexpr float ChargeAmount = 0.1f;

ATBattery::ATBattery(float PercentIn)
{
	SetPercent(PercentIn);
}

void ATBattery::Charge()
{
	SetPercent(Percent + ChargeAmount);
}

void ATBattery::UnCharge()
{
	SetPercent(Percent - ChargeAmount);
}

float ATBattery::GetPercent() const
{
	return Percent;
}

FColor ATBattery::GetColor() const
{
	if (Percent > 0.8f)
	{
		return FColor::Green;
	}

	if (Percent > 0.3f)
	{
		return FColor::Yellow;
	}
	return FColor::Red;
}

FString ATBattery::ToString() const
{
	return FString::Printf(TEXT("%i%%"), FMath::RoundToInt(Percent * 100));
}

bool ATBattery::operator>(const ATBattery& rhs) const
{
	return GetPercent() > rhs.GetPercent();
}

bool ATBattery::operator>=(const ATBattery& rhs) const
{
	return GetPercent() >= rhs.GetPercent();
}

bool ATBattery::operator==(const ATBattery& rhs) const
{
	return FMath::IsNearlyEqual(GetPercent(), rhs.GetPercent());
}

bool ATBattery::operator<(const ATBattery& rhs) const
{
	return GetPercent() < rhs.GetPercent();
}

bool ATBattery::operator<=(const ATBattery& rhs) const
{
	return GetPercent() <= rhs.GetPercent();
}

bool ATBattery::operator!=(const ATBattery& rhs) const
{
	return !FMath::IsNearlyEqual(GetPercent(), rhs.GetPercent());
}

void ATBattery::SetPercent(float PercentIn)
{
	Percent = FMath::Clamp(PercentIn, 0.f, 1.f);
}
}
