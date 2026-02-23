// Copyright Epic Games, Inc. All Rights Reserved.

#include "GunSmith_NlundyPickUpComponent.h"

UGunSmith_NlundyPickUpComponent::UGunSmith_NlundyPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UGunSmith_NlundyPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UGunSmith_NlundyPickUpComponent::OnSphereBeginOverlap);
}

void UGunSmith_NlundyPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AGunSmith_NlundyCharacter* Character = Cast<AGunSmith_NlundyCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
