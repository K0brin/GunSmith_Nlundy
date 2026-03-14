// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_UserInterface.generated.h"

class APrimaryRifle;
class UTextBlock;
/**
 * 
 */
UCLASS()
class UUW_UserInterface : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	UFUNCTION() void UpdateAmmoText(int32 NewAmmoCount);
	UPROPERTY(meta = (BindWidget)) UTextBlock* AmmoText;
	//UPROPERTY(meta = (BindWidget)) UTextBlock* DamageText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) APrimaryRifle* PlayerGun;
	
};
