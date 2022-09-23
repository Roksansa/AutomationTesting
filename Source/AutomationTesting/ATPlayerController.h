// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ATPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdatePauseState, bool, bPauseState);

UCLASS()
class AUTOMATIONTESTING_API AATPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;

public:
	void PauseGame();

	UPROPERTY(BlueprintAssignable)
	FOnUpdatePauseState OnUpdatePauseState;
private:
	bool bIsPause = false;
};
