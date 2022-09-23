// Fill out your copyright notice in the Description page of Project Settings.


#include "Tests/UI/ATPauseWidget.h"

#include "ATPlayerController.h"

void UATPauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseMenuButton->OnClicked.AddDynamic(this, &UATPauseWidget::OnCloseMenu);
}

void UATPauseWidget::OnCloseMenu()
{
	if (!GetWorld()) return;

	if (AATPlayerController* PlayerController = Cast<AATPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerController->PauseGame();
	}
}
