// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AutomationTesting/Tests/ATTypes.h"
#include "GameFramework/Actor.h"
#include "ATInventoryItem.generated.h"

UCLASS(Abstract)
class AATInventoryItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AATInventoryItem();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
protected:
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventoryData InventoryData;
};
