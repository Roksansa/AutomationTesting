// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ATSettingOptionWidget.generated.h"

class UButton;
class UTextBlock;
class UATGameSetting;

UCLASS()
class AUTOMATIONTESTING_API UATSettingOptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(UATGameSetting* CurSetting);
	void UpdateText();

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SettingDisplayText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CurrentSettingText;

	UPROPERTY(meta=(BindWidget))
	UButton* PrevSettingButton;
	UPROPERTY(meta=(BindWidget))
	UButton* NextSettingButton;

	virtual void NativeOnInitialized() override;

private:
	TWeakObjectPtr<UATGameSetting> Setting;

	void OnClickedSettingButton(bool bNext);
};
