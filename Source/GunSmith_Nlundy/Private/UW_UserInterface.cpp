// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_UserInterface.h"

#include "PrimaryRifle.h"
#include "Components/TextBlock.h"

void UUW_UserInterface::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (MasterGun)
	{
		AmmoText->SetText(FText::AsNumber(MasterGun->CurrentAmmo));
		MasterGun->OnAmmoChanged.AddDynamic(this, &UUW_UserInterface::UpdateAmmoText);
		
		DamageText->SetText(FText::AsNumber(MasterGun->Damage));
		MasterGun->OnDamageChanged.AddDynamic(this, &UUW_UserInterface::UpdateDamageText);
		
		FireRateText->SetText(FText::AsNumber(MasterGun->FireRate));
		MasterGun->OnFireRateChanged.AddDynamic(this, &UUW_UserInterface::UpdateFireRateText);
		
		SpeedText->SetText(FText::AsNumber(MasterGun->MovementSpeed));
		MasterGun->OnSpeedChanged.AddDynamic(this, &UUW_UserInterface::UpdateSpeedText);
		
		AimSpeedText->SetText(FText::AsNumber(MasterGun->AimWalkingSpeed));
		MasterGun->OnAimSpeedChanged.AddDynamic(this, &UUW_UserInterface::UpdateAimSpeedText);
		
		AmmoCapacityText->SetText(FText::AsNumber(MasterGun->MaxAmmo));
		MasterGun->OnAmmoCapacityChanged.AddDynamic(this, &UUW_UserInterface::UpdateAmmoCapacityText);
		
		ReloadSpeedText->SetText(FText::AsNumber(MasterGun->ReloadSpeed));
		MasterGun->OnReloadSpeedChanged.AddDynamic(this, &UUW_UserInterface::UpdateReloadSpeedText);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Gun Not Associated"));
	}
	
}

void UUW_UserInterface::UpdateAmmoText(int32 NewAmmoCount)
{
	
	if (AmmoText)
	{
		AmmoText->SetText(FText::AsNumber(NewAmmoCount));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AmmoText Not Initialized"));
	}
	
}

void UUW_UserInterface::UpdateDamageText(int Damage)
{
	DamageText->SetText(FText::AsNumber(Damage));
}

void UUW_UserInterface::UpdateFireRateText(float FireRate)
{
	FireRateText->SetText(FText::AsNumber(FireRate));
}

void UUW_UserInterface::UpdateSpeedText(float Speed)
{
	SpeedText->SetText(FText::AsNumber(Speed));
}

void UUW_UserInterface::UpdateAimSpeedText(float AimSpeed)
{
	AimSpeedText->SetText(FText::AsNumber(AimSpeed));
}

void UUW_UserInterface::UpdateAmmoCapacityText(int MaxAmmoCount)
{
	AmmoCapacityText->SetText(FText::AsNumber(MaxAmmoCount));
}

void UUW_UserInterface::UpdateReloadSpeedText(float ReloadSpeed)
{
	ReloadSpeedText->SetText(FText::AsNumber(ReloadSpeed));
}
