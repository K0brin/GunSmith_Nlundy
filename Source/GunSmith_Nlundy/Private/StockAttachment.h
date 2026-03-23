// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StockAttachment.generated.h"

/**
 * 
 */
UCLASS()
class UStockAttachment : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MovementSpeed = 75.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float AimWalkingSpeed = 50.0f;
	
};
