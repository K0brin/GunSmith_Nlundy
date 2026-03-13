// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"

// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ATarget::TakeDamage(int32 damage)
{
	
	if (!Invincible)
	{
		CurrentHealth -= damage;
		if (CurrentHealth <= 0)
		{
			//Target is dead
			Invincible = true;
			TargetComponent->SetVisibility(false);
			GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ATarget::Revive, ReviveSpeed,false);
		}
		return true; //did damage
	}
	return false; //did not do damage
}

void ATarget::Revive()
{
	CurrentHealth = MaxHealth;
	Invincible = false;
	TargetComponent->SetVisibility(true);
	
}
