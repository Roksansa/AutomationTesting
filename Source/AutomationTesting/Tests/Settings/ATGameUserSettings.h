// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "ATGameUserSettings.generated.h"

class UATGameSetting;
DECLARE_MULTICAST_DELEGATE(FOnGraphicsSettingsUpdated);

UCLASS()
class UATGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UATGameUserSettings();

	static UATGameUserSettings* Get();

	const TArray<UATGameSetting*>& GetGraphicsSettings() const;

	void RunBenchmark();

	FOnGraphicsSettingsUpdated OnGraphicsSettingsUpdated;
private:
	UPROPERTY()
	TArray<UATGameSetting*> GraphicsSettings;
};
