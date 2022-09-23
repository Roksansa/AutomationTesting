// Fill out your copyright notice in the Description page of Project Settings.


#include "Tests/Settings/ATGameSetting.h"

DEFINE_LOG_CATEGORY_STATIC(LogATGameSetting, Verbose, Verbose);

UATGameSetting& UATGameSetting::SetName(const FText& Name)
{
	SettingName = Name;
	return *this;
}

UATGameSetting& UATGameSetting::SetOption(const TArray<FATSettingOption>& CurOptions)
{
	Options = CurOptions;
	return *this;
}

FATSettingOption UATGameSetting::GetCurrentOption() const
{
	const int32 Index = Getter ? Getter() : INDEX_NONE;
	if (Index == INDEX_NONE || Index >= Options.Num())
	{
		UE_LOG(LogATGameSetting, Warning, TEXT("GetCurrentOption not corrent -- Index == INDEX_NONE || Index >= Options.Num()"));
		return Options[0];
	}
	return Options[Index];
}

FText UATGameSetting::GetSettingName() const
{
	return SettingName;
}

void UATGameSetting::ApplyNextOption()
{
	const int32 CurrentIndex = Getter ? Getter() : 0;
	const int32 NextIndex = (CurrentIndex + 1) % Options.Num();
	SetCurrentValue(Options[NextIndex].Value);
}

void UATGameSetting::ApplyPrevOption()
{
	const int32 CurrentIndex = Getter ? Getter() : 0;
	const int32 PrevIndex = CurrentIndex == 0 ? Options.Num() - 1 : CurrentIndex - 1;
	SetCurrentValue(Options[PrevIndex].Value);
}

UATGameSetting& UATGameSetting::SetGetter(TFunction<int32()> FuncGetter)
{
	Getter = FuncGetter;
	return *this;
}

UATGameSetting& UATGameSetting::SetSetter(TFunction<void(int32)> FuncSetter)
{
	Setter = FuncSetter;
	return *this;
}

void UATGameSetting::SetCurrentValue(int32 Quality)
{
	if (Setter)
	{
		Setter(Quality);
	}
}
