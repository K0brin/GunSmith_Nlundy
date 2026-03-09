// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryRifle.h"

#include "Camera/CameraComponent.h"

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

void APrimaryRifle::FireWeapon()
{
	FVector barrelTipLocation = {0,0,0};
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
	
	FHitResult hitResult1;
	FHitResult hitResult;
	FVector lineTraceStart = {barrelTipLocation.X,barrelTipLocation.Y,barrelTipLocation.Z};
	FVector playerLineStart = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass<UCameraComponent>()->GetComponentLocation();
	//calculating dynamic forward
	FVector lineTraceEnd = lineTraceStart + (GetActorForwardVector() * -LineTraceDistance);
	//call function on for each bullet - raycast is held here
	//raycast from center of player (to get target position)
	GetWorld()->LineTraceSingleByChannel (hitResult1, playerLineStart, lineTraceEnd,ECC_Visibility);
	DrawDebugLine(GetWorld(),playerLineStart, hitResult1.Location, FColor::Green, false, 1.0f, 0, 2.0f);
	//raycast from gun
	GetWorld()->LineTraceSingleByChannel(hitResult, lineTraceStart, hitResult1.Location,ECC_Visibility);
	DrawDebugLine(GetWorld(),lineTraceStart, hitResult.Location, FColor::Red, false, 1.0f, 0, 2.0f);
	AActor* hitActor = hitResult.GetActor();
	if (hitActor)
	{
		FString actorName = hitActor->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Hit1 Actor: %s"), *actorName);
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Nothing"));
	}
	
}

