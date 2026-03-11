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
	//Raycasts for bullets
	//accuracy ^ goes along with that
	//speed - stored locally, used to affect player speed
	//magazine
	UFUNCTION(BlueprintCallable) void FireWeapon();
	UFUNCTION(BlueprintCallable) void FullAutoFire();
	UFUNCTION(BlueprintCallable) void CancelFullAutoFire();
	UFUNCTION(BlueprintCallable) void DecrementAmmo();
	UFUNCTION(BlueprintCallable) void SetAmmoMax();
	UFUNCTION(BlueprintCallable) void ManualReload();
	UFUNCTION(BlueprintCallable) void SpawnHitEffect(FVector spawnLocation);
	UPROPERTY(EditAnywhere) int LineTraceDistance = 2000;
	UPROPERTY(EditAnywhere) int CurrentAmmo = 0;
	UPROPERTY(EditAnywhere) int MaxAmmo = 50;
	UPROPERTY(EditAnywhere) float FireRate = 0.1f; //In Seconds
	UPROPERTY(EditAnywhere) FTimerHandle timerHandle;
	UPROPERTY(EditAnywhere) TSubclassOf<AActor> HitEffectToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UUserWidget* PlayerUserInterface;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
