// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryRifle.h"

#include "BarrelAttachment.h"
#include "GripAttachment.h"
#include "MagazineAttachment.h"
#include "StockAttachment.h"
#include "Target.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/TextBlock.h"
#include "Engine/TextRenderActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APrimaryRifle::APrimaryRifle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APrimaryRifle::BeginPlay()
{
	Super::BeginPlay();
	//Initialize default Data Assets
	InitializeStats();
	SetAmmoMax();
	
}

// Called every frame
void APrimaryRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//gun rotates with player
	FRotator playerRotation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass<UCameraComponent>()->GetComponentRotation();
	playerRotation.Yaw += 180.0f;
	playerRotation.Pitch *= -1;
	SetActorRotation(playerRotation);
	
	
	//setting movement speed
	if (!IsAiming)
	{
		GetWorld()->GetFirstPlayerController()->GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	}
	else
	{
		GetWorld()->GetFirstPlayerController()->GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = AimWalkingSpeed;
	}
	
	
}

void APrimaryRifle::FullAutoFire()
{
	//initial shot for tap fire
	FireWeapon();
	//Wait until next shot
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &APrimaryRifle::FireWeapon, FireRate, true);
}

void APrimaryRifle::CancelFullAutoFire()
{
	GetWorldTimerManager().ClearTimer(timerHandle);
}

void APrimaryRifle::FireWeapon()
{
	
	if (CurrentAmmo <= 0 || IsReloading)
	{
		if (IsReloading)
		{
			UE_LOG(LogTemp, Warning, TEXT("Gun is Reloading"))
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Gun is Out of Ammo"))
			recoilCount = 0;
		}
	}
	else
	{
		FCollisionQueryParams params;
		params.AddIgnoredActor(this); // Ignores the Rifle
		params.AddIgnoredActor(GetWorld()->GetFirstPlayerController()->GetPawn()); // Ignores the Player holding it
		FVector barrelTipLocation = {100,100,100};
		//get line trace start
		TArray<UStaticMeshComponent*> meshCompArray;
		GetComponents<UStaticMeshComponent>(meshCompArray);
		for (UStaticMeshComponent* mesh : meshCompArray)
		{
			if (mesh->GetName() == "BarrelTip")
			{
				barrelTipLocation = mesh->GetComponentLocation();
			}
		}
	
		//Single LineTrace from center of camera
		FHitResult hitResult1;
		FVector lineTraceStart = barrelTipLocation;
		FVector playerLineStart = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass<UCameraComponent>()->GetComponentLocation();
		//calculating dynamic forward
		FVector lineTraceEnd = lineTraceStart + (BulletRecoilDirection() * -LineTraceDistance);
		GetWorld()->LineTraceSingleByChannel (hitResult1, playerLineStart, lineTraceEnd,ECC_Visibility, params);
		
		SpawnHitEffect(hitResult1.ImpactPoint, hitResult1);
	
		DecrementAmmo();
		UE_LOG(LogTemp, Warning, TEXT("Current Ammo: %i"), CurrentAmmo);
	}
	
}


void APrimaryRifle::DecrementAmmo()
{
	CurrentAmmo--;
	CurrentAmmo = FMath::Clamp(CurrentAmmo, 0,MaxAmmo);
	OnAmmoChanged.Broadcast(CurrentAmmo);
}

void APrimaryRifle::SetAmmoMax()
{
	CurrentAmmo = MaxAmmo;
	OnAmmoChanged.Broadcast(CurrentAmmo);
	IsReloading = false;
	UE_LOG(LogTemp, Warning, TEXT("Ammo Set To Max; Current Ammo: %i"), CurrentAmmo);
}

void APrimaryRifle::ManualReload()
{
	//call reload animation
	//waiting time of reload
	IsReloading = true;
	recoilCount = 0;
	GetWorld()->GetTimerManager().SetTimer(timerHandle2, this, &APrimaryRifle::SetAmmoMax, ReloadSpeed,false);
}

void APrimaryRifle::SpawnHitEffect(FVector spawnLocation, FHitResult hitResult)
{
	//Damage Numbers
	if (ATarget* hitActor = Cast<ATarget>(hitResult.GetActor()))
	{
		FRotator textRotation(0,180,0);
		spawnLocation = FVector(spawnLocation.X - 10, spawnLocation.Y, spawnLocation.Z);
		if (hitActor->RecieveDamage(Damage))
		{
			if (DamageEnabled)
			{
				ATextRenderActor* DamageText = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), spawnLocation, textRotation);
				DamageText->GetTextRender()->SetText(FText::AsNumber(Damage));
				DamageText->SetLifeSpan(3.0f);
			}
			
		}
		else
		{
			if (DamageEnabled)
			{
				ATextRenderActor* DamageText = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), spawnLocation, textRotation);
				DamageText->GetTextRender()->SetText(FText::FromString("Dead"));
				DamageText->SetLifeSpan(3.0f);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Object is not ATarget"));
	}
	
	//HitEffect
	if (hitResult.GetActor())
	{
		AActor* hitEffect = GetWorld()->SpawnActor<AActor>(HitEffectToSpawn, hitResult.ImpactPoint, FRotator::ZeroRotator);
		hitEffect->SetLifeSpan(3.0f);
	}
	
}

FVector APrimaryRifle::BulletRecoilDirection()
{
	if (recoilCount <= MaxAmmo - 1)
	{
		//take spawn position -> edit the value depending on what index of bullet is fired in a array, reset index on lmb up, reload, or mag empty
		FVector direction =(GetActorForwardVector() + RecoilArray[recoilCount]).GetSafeNormal();
		recoilCount++;
		//return direction to be used in raycast
		return direction;
	}
	else
	{
		recoilCount = 0;
		return BulletRecoilDirection();
	}
}

void APrimaryRifle::InitializeStats() //initializing default Data Assets
{

	//assigns default data asset values to hard code
	
	//Barrel
	Damage = GunAttachments.BarrelArray[0]->Damage;
	FireRate = GunAttachments.BarrelArray[0]->FireRate;
	//Stock
	MovementSpeed = GunAttachments.StockArray[0]->MovementSpeed;
	AimWalkingSpeed = GunAttachments.StockArray[0]->AimWalkingSpeed;
	//Grip
	RecoilArray = GunAttachments.GripArray[0]->RecoilArray;
	//Magazine
	MaxAmmo = GunAttachments.MagazineArray[0]->AmmoCapacity;
	ReloadSpeed = GunAttachments.MagazineArray[0]->ReloadSpeed;
	
	OnDamageChanged.Broadcast(Damage);
	OnFireRateChanged.Broadcast(FireRate);
	OnSpeedChanged.Broadcast(MovementSpeed);
	OnAimSpeedChanged.Broadcast(AimWalkingSpeed);
	OnAmmoCapacityChanged.Broadcast(MaxAmmo);
	OnReloadSpeedChanged.Broadcast(ReloadSpeed);
}

void APrimaryRifle::ChangeAttachments(FString type, int index) //called when player changes attachment on UI
{

		//assigns data asset values to hard code
	
	if (type == "Barrel")
	{
		Damage = GunAttachments.BarrelArray[index]->Damage;
		FireRate = GunAttachments.BarrelArray[index]->FireRate;
		OnDamageChanged.Broadcast(Damage);
		OnFireRateChanged.Broadcast(FireRate);
	}
	else if (type == "Magazine")
	{
		MaxAmmo = GunAttachments.MagazineArray[index]->AmmoCapacity;
		ReloadSpeed = GunAttachments.MagazineArray[index]->ReloadSpeed;
		OnAmmoCapacityChanged.Broadcast(MaxAmmo);
		OnReloadSpeedChanged.Broadcast(ReloadSpeed);
	}
	else if (type == "Grip")
	{
		RecoilArray = GunAttachments.GripArray[index]->RecoilArray;
	}
	else if (type == "Stock")
	{
		MovementSpeed = GunAttachments.StockArray[index]->MovementSpeed;
		AimWalkingSpeed = GunAttachments.StockArray[index]->AimWalkingSpeed;
		OnSpeedChanged.Broadcast(MovementSpeed);
		OnAimSpeedChanged.Broadcast(AimWalkingSpeed);
	}
}

