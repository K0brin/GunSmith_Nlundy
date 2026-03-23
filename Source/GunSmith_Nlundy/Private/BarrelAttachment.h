// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BarrelAttachment.generated.h"

/**
 * 
 */
UCLASS()
class UBarrelAttachment : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int Damage = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float FireRate = 0.1f;
	
};
