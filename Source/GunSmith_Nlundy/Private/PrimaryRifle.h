// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrimaryRifle.generated.h"

UCLASS()
class APrimaryRifle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APrimaryRifle();
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* BarrelTipComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	
private:
	//Raycasts for bullets
	//accuracy ^ goes along with that
	//speed - stored locally, used to affect player speed
	//magazine
	UFUNCTION(BlueprintCallable) void FireWeapon();

};
