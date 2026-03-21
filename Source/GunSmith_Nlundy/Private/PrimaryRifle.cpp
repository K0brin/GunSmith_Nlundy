// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryRifle.h"

#include "Target.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/TextBlock.h"
#include "Engine/TextRenderActor.h"

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
		FVector lineTraceEnd = lineTraceStart + (GetActorForwardVector() * -LineTraceDistance);
		//call function on for each bullet - raycast is held here
		//raycast from center of player (to get target position)
		GetWorld()->LineTraceSingleByChannel (hitResult1, playerLineStart, lineTraceEnd,ECC_Visibility, params);
		//DrawDebugLine(GetWorld(),playerLineStart, hitResult1.ImpactPoint, FColor::Green, false, 1.0f, 0, 2.0f);
		//raycast from gun
		//GetWorld()->LineTraceSingleByChannel(hitResult, lineTraceStart, hitResult1.ImpactPoint,ECC_Visibility, params);
		//DrawDebugLine(GetWorld(),lineTraceStart, hitResult.ImpactPoint, FColor::Red, false, 1.0f, 0, 2.0f);
		AActor* hitActor1 = hitResult1.GetActor()->GetParentActor();
		//AActor* hitActor = hitResult.GetActor();
	
		SpawnHitEffect(hitResult1.Location, hitResult1);
	
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
	GetWorld()->GetTimerManager().SetTimer(timerHandle2, this, &APrimaryRifle::SetAmmoMax, ReloadSpeed,false);
}

void APrimaryRifle::SpawnHitEffect(FVector spawnLocation, FHitResult hitResult)
{
	if (ATarget* hitActor = Cast<ATarget>(hitResult.GetActor()))
	{
		FRotator textRotation(0,180,0);
		spawnLocation = FVector(spawnLocation.X - 10, spawnLocation.Y, spawnLocation.Z);
		if (hitActor->RecieveDamage(Damage))
		{
			ATextRenderActor* DamageText = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), spawnLocation, textRotation);
			DamageText->GetTextRender()->SetText(FText::AsNumber(Damage));
			DamageText->SetLifeSpan(3.0f);
			
		}
		else
		{
			ATextRenderActor* DamageText = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), spawnLocation, textRotation);
			DamageText->GetTextRender()->SetText(FText::FromString("Dead"));
			DamageText->SetLifeSpan(3.0f);
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Object is not ATarget"));
	}
	
}

