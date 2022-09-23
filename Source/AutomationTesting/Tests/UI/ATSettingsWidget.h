// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ATSettingsWidget.generated.h"

class UButton;
class UATSettingOptionWidget;
class UVerticalBox;

UCLASS()
class AUTOMATIONTESTING_API UATSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* SettingsVerticalBox;

	UPROPERTY(meta=(BindWidget))
	UButton* StartBenchmarkButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "UI")
	TSubclassOf<UATSettingOptionWidget> SettingsOptionalWidgetClass;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnClickedStartBenchmarkButton();

	UFUNCTION()
	void OnUpdatedGraphicsSettings();
};
