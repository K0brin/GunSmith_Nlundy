// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Target.generated.h"

UCLASS()
class ATarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATarget();
	
	UFUNCTION(BlueprintCallable) bool TakeDamage(int32 damage);
	UFUNCTION(BlueprintCallable) void Revive();
	UPROPERTY(EditAnywhere) int32 MaxHealth = 100;
	UPROPERTY(EditAnywhere) int32 CurrentHealth = MaxHealth;
	UPROPERTY(EditAnywhere) float ReviveSpeed  = 10;
	UPROPERTY(EditAnywhere) bool Invincible = false;
	
private:
	FTimerHandle timerHandle;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
