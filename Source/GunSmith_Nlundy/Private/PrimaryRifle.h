// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "PrimaryRifle.generated.h"

class UMagazineAttachment;
class UStockAttachment;
class UBarrelAttachment;
class UGripAttachment;
class UAttachmentSlot;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32, NewAmmoCount);

class UTextRenderComponent;

USTRUCT(BlueprintType)
struct FGunStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int Damage = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float FireRate = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MovementSpeed = 75.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float AimWalkingSpeed = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FVector> RecoilArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int AmmoCapacity = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float ReloadSpeed = 1.5f;
	
};

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
	UFUNCTION(BlueprintCallable) void SpawnHitEffect(FVector spawnLocation, FHitResult hitResult);
	UFUNCTION(BlueprintCallable) FVector BulletRecoilDirection();
	UFUNCTION(BlueprintCallable) void InitializeStats();
	UPROPERTY(EditAnywhere) int LineTraceDistance = 2000;
	UPROPERTY() int CurrentAmmo = 0;
	UPROPERTY() int MaxAmmo = 25;
	UPROPERTY() float FireRate = 0.1f; //In Seconds
	UPROPERTY() float ReloadSpeed = 1.5f; //In Seconds
	UPROPERTY(EditAnywhere) FTimerHandle timerHandle;
	UPROPERTY(EditAnywhere) FTimerHandle timerHandle2;
	UPROPERTY(EditAnywhere) TSubclassOf<AActor> HitEffectToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UUserWidget* PlayerUserInterface;
	UPROPERTY(BlueprintAssignable) FOnAmmoChanged OnAmmoChanged;
	UPROPERTY(EditAnywhere) int Damage = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool IsAiming = false;
	UPROPERTY(EditAnywhere) bool IsReloading = false;
	UPROPERTY() TArray<FVector> RecoilArray; //Holds vector offsets from original hit positon "(0,0)"
	UPROPERTY(BlueprintReadWrite) int recoilCount = 0;
	UPROPERTY(EditAnywhere, Category = "Gun Stats") FGunStats GunStats;
	UPROPERTY(EditAnywhere, Category = "Arrays") TArray<UGripAttachment*> GripArray;
	UPROPERTY(EditAnywhere, Category = "Arrays") TArray<UBarrelAttachment*> BarrelArray;
	UPROPERTY(EditAnywhere, Category = "Arrays") TArray<UMagazineAttachment*> MagazineArray;
	UPROPERTY(EditAnywhere, Category = "Arrays") TArray<UStockAttachment*> StockArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
