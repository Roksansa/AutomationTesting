// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ATGameSetting.generated.h"

USTRUCT()
struct FATSettingOption
{
	GENERATED_BODY()
	FText Name;
	int32 Value;
};

UCLASS()
class UATGameSetting : public UObject
{
	GENERATED_BODY()

	friend class UATGameUserSettings;
public:
	FATSettingOption GetCurrentOption() const;
	FText GetSettingName() const;

	void ApplyNextOption();
	void ApplyPrevOption();

private:
	FText SettingName;
	TArray<FATSettingOption> Options;

	TFunction<int32()> Getter;
	TFunction<void(int32)> Setter;

	UATGameSetting& SetName(const FText& Name);
	UATGameSetting& SetOption(const TArray<FATSettingOption>& Options);
	UATGameSetting& SetGetter(TFunction<int32()> FuncGetter);
	UATGameSetting& SetSetter(TFunction<void(int32)> FuncSetter);

	void SetCurrentValue(int32 Quality);
};
