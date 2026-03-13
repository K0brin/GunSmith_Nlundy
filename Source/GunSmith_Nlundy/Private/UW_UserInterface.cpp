// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_UserInterface.h"

#include "PrimaryRifle.h"
#include "Components/TextBlock.h"

void UUW_UserInterface::NativeConstruct()
{
	Super::NativeConstruct();
	
	AmmoText->SetText(FText::AsNumber(PlayerGun->CurrentAmmo));
	
	if (PlayerGun)
	{
		PlayerGun->OnAmmoChanged.AddDynamic(this, &UUW_UserInterface::UpdateAmmoText);
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
