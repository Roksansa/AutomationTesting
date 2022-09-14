// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ATInputRecordingUtils.h"
#include "Components/ActorComponent.h"
#include "ATInputRecordingComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UATInputRecordingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UATInputRecordingComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	TSet<FString> ExcludeWords;
	UPROPERTY(EditAnywhere, meta=(ClampMin=15.f, UIMin=15.f))
	float FixedCountFps = 60.f;

private:
	FInputData InputData;

	FBindingsData MakeBindingsData() const;
	FString GeneratedFileName() const;
};
