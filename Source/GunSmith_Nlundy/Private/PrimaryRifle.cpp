// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryRifle.h"

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

}

void APrimaryRifle::FireWeapon()
{
	FHitResult hitResult;
	//call function on for each bullet - raycast is held here
	//GetWorld()->LineTraceSingleByChannel(hitResult, , )
}

