// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttachmentSlot.generated.h"

/**
 * 
 */
UCLASS()
class UAttachmentSlot : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int Damage = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float FireRate = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MovementSpeed = 75.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float AimWalkingSpeed = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FVector> RecoilArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int AmmoCapacity = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float ReloadSpeed = 1.5f;
	
};
