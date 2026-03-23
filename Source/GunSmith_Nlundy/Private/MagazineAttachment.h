// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MagazineAttachment.generated.h"

/**
 * 
 */
UCLASS()
class UMagazineAttachment : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int AmmoCapacity = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float ReloadSpeed = 1.5f;
	
};
