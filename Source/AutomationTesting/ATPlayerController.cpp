// Fill out your copyright notice in the Description page of Project Settings.


#include "ATPlayerController.h"

#include "GameFramework/GameModeBase.h"

void AATPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!InputComponent)
	{
		return;
	}

	auto& Input = InputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &AATPlayerController::PauseGame);
	Input.bExecuteWhenPaused = true;
}

void AATPlayerController::PauseGame()
{
	bIsPause = !bIsPause;
	SetPause(bIsPause);
	bShowMouseCursor = bIsPause;
	bIsPause ? SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false)) : SetInputMode(FInputModeGameOnly());
	OnUpdatePauseState.Broadcast(bIsPause);
}
