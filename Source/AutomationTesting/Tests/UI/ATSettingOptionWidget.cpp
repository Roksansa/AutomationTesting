// Fill out your copyright notice in the Description page of Project Settings.


#include "Tests/UI/ATSettingOptionWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Tests/Settings/ATGameSetting.h"
#include "Widgets/Input/SButton.h"

void UATSettingOptionWidget::Init(UATGameSetting* CurSetting)
{
	Setting = MakeWeakObjectPtr(CurSetting);
	UpdateText();
}

void UATSettingOptionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//PrevSettingButton->OnClicked.AddDynamic(CreateLambda)

	SButton* ButtonWidget = static_cast<SButton*>(&(PrevSettingButton->TakeWidget().Get()));
	ButtonWidget->SetOnClicked(FOnClicked::CreateLambda([this]()
	 {
	 	OnClickedSettingButton(false);
		//For bp used
		PrevSettingButton->OnClicked.Broadcast();
	 	return FReply::Handled();
	 }));

	ButtonWidget = static_cast<SButton*>(&(NextSettingButton->TakeWidget().Get()));
	ButtonWidget->SetOnClicked(FOnClicked::CreateLambda([this]()
	{
		OnClickedSettingButton(true);
		//For bp used
		NextSettingButton->OnClicked.Broadcast();
		return FReply::Handled();
	}));
}

void UATSettingOptionWidget::UpdateText()
{
	if (!Setting.IsValid())
	{
		return;
	}

	SettingDisplayText->SetText(Setting->GetSettingName());
	CurrentSettingText->SetText(Setting->GetCurrentOption().Name);
}

void UATSettingOptionWidget::OnClickedSettingButton(bool bNext)
{
	if (!Setting.IsValid())
	{
		return;
	}

	bNext ? Setting->ApplyNextOption() : Setting->ApplyPrevOption();
	CurrentSettingText->SetText(Setting->GetCurrentOption().Name);
}
