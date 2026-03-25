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
	UFUNCTION() void UpdateDamageText(int Damage);
	UFUNCTION() void UpdateFireRateText(float FireRate);
	UFUNCTION() void UpdateSpeedText(float Speed);
	UFUNCTION() void UpdateAimSpeedText(float AimSpeed);
	UFUNCTION() void UpdateAmmoCapacityText(int MaxAmmoCount);
	UFUNCTION() void UpdateReloadSpeedText(float ReloadSpeed);
	UPROPERTY(meta = (BindWidget)) UTextBlock* AmmoText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* DamageText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* FireRateText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* SpeedText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* AimSpeedText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* AmmoCapacityText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ReloadSpeedText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) APrimaryRifle* MasterGun;
	
};
