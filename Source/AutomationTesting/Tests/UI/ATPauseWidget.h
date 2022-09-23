// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ATPauseWidget.generated.h"

UCLASS()
class AUTOMATIONTESTING_API UATPauseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* CloseMenuButton;

	virtual void NativeOnInitialized() override;
private:
	UFUNCTION()
	void OnCloseMenu();
};
