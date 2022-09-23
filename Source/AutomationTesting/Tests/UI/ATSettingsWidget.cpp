// Fill out your copyright notice in the Description page of Project Settings.


#include "Tests/UI/ATSettingsWidget.h"

#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Tests/Settings/ATGameSetting.h"
#include "Tests/Settings/ATGameUserSettings.h"
#include "Tests/UI/ATSettingOptionWidget.h"

void UATSettingsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UATGameUserSettings* UserSetting = UATGameUserSettings::Get();
	if (!UserSetting)
	{
		return;
	}

	UserSetting->LoadSettings();

	TArray<UATGameSetting*> GraphicsSettings = UserSetting->GetGraphicsSettings();
	SettingsVerticalBox->ClearChildren();

	for (auto* Setting : GraphicsSettings)
	{
		UATSettingOptionWidget* const SettingOptionWidget = CreateWidget<UATSettingOptionWidget>(this, SettingsOptionalWidgetClass);
		check(SettingOptionWidget);
		SettingOptionWidget->Init(Setting);
		SettingsVerticalBox->AddChild(SettingOptionWidget);
	}
	StartBenchmarkButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedStartBenchmarkButton);
	UserSetting->OnGraphicsSettingsUpdated.AddUObject(this, &ThisClass::OnUpdatedGraphicsSettings);
}

void UATSettingsWidget::OnClickedStartBenchmarkButton()
{
	if (auto* UserSettings = UATGameUserSettings::Get())
	{
		UserSettings->RunBenchmark();
	}
}

void UATSettingsWidget::OnUpdatedGraphicsSettings()
{
	for (auto* Widget : SettingsVerticalBox->GetAllChildren())
	{
		if (auto* SettingWidget = Cast<UATSettingOptionWidget>(Widget))
		{
			SettingWidget->UpdateText();
		}
	}
}
