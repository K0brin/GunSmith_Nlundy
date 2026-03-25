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
	InitializeStats();
	SetAmmoMax();
	
}

// Called every frame
void APrimaryRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator playerRotation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass<UCameraComponent>()->GetComponentRotation();
	playerRotation.Yaw += 180.0f;
	playerRotation.Pitch *= -1;
	//playerRotation.SetComponentForAxis(EAxis::Z, playerRotation.GetComponentForAxis(EAxis::Z) * -1);
	SetActorRotation(playerRotation);
	
	
	//movement speed
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
	FireWeapon();
	//Delay and function call to shoot
	//have a bool instead of true if I want to have a fire rate switch - true = full auto -> false = semi
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
	
	
		//TODO:Second Raycast destination is wrong half of the time
		FHitResult hitResult1;
		//FHitResult hitResult;
		FVector lineTraceStart = barrelTipLocation;
		FVector playerLineStart = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass<UCameraComponent>()->GetComponentLocation();
		//calculating dynamic forward
		FVector lineTraceEnd = lineTraceStart + (BulletRecoilDirection() * -LineTraceDistance);
		//edit the direction of the cast for recoil
		//call function on for each bullet - raycast is held here
		//raycast from center of player (to get target position)
		GetWorld()->LineTraceSingleByChannel (hitResult1, playerLineStart, lineTraceEnd,ECC_Visibility, params);
		//AActor* hitActor1 = hitResult1.GetActor()->GetParentActor();
		//DrawDebugLine(GetWorld(),playerLineStart, hitResult1.ImpactPoint, FColor::Green, false, 1.0f, 0, 2.0f);
		//raycast from gun
		//GetWorld()->LineTraceSingleByChannel(hitResult, lineTraceStart, hitResult1.ImpactPoint,ECC_Visibility, params);
		//DrawDebugLine(GetWorld(),lineTraceStart, hitResult.ImpactPoint, FColor::Red, false, 1.0f, 0, 2.0f);
		//AActor* hitActor = hitResult.GetActor();
		
		SpawnHitEffect(hitResult1.ImpactPoint, hitResult1);
	
		DecrementAmmo();
		UE_LOG(LogTemp, Warning, TEXT("Current Ammo: %i"), CurrentAmmo);
	
		//auto-reload
		/*if (CurrentAmmo <= 0 )
		{
			ManualReload();
		}
	
		if (hitResult1.GetActor())
		{
			/*FString actor1Name = hitActor1->GetName();
			UE_LOG(LogTemp, Warning, TEXT("Hit1 Actor: %s"), *actor1Name);
			UE_LOG(LogTemp, Warning, TEXT("Hit1 Location: %s"), *hitResult1.Location.ToString());

			UE_LOG(LogTemp, Warning, TEXT("Hit actor class: %s"), *hitResult1.GetActor()->GetClass()->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit1 Nothing"));
			UE_LOG(LogTemp, Warning, TEXT("Hit1 Location: %s"), *hitResult1.Location.ToString());
		}*/
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
	//waiting time of reload]
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

void APrimaryRifle::InitializeStats() //initializing defaults
{
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
	
	//Damage = GunStats.Damage;
	//FireRate = GunStats.FireRate;
	//movement speed
	//aim movement speed
	//RecoilArray = GunStats.RecoilArray;
	//MaxAmmo = GunStats.AmmoCapacity;
	//ReloadSpeed = GunStats.ReloadSpeed;
}

void APrimaryRifle::ChangeAttachments(FString type, int index)//parameters: type (barrel, stock, etc); index #
{
	
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

