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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageChanged, int, NewAmmoCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFireRateChanged, float, NewAmmoCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpeedChanged, float, NewAmmoCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAimSpeedChanged, float, NewAmmoCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoCapacityChanged, int, NewAmmoCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReloadSpeedChanged, float, NewAmmoCount);

class UTextRenderComponent;

USTRUCT(BlueprintType)
struct FGunStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int Damage = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float FireRate = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MovementSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float AimWalkingSpeed = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FVector> RecoilArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int AmmoCapacity = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float ReloadSpeed = 1.5f;
	
};

USTRUCT(BlueprintType)
struct FGunAttachments
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Attachment DataAssets") TArray<UGripAttachment*> GripArray;
	UPROPERTY(EditAnywhere, Category = "Attachment DataAssets") TArray<UBarrelAttachment*> BarrelArray;
	UPROPERTY(EditAnywhere, Category = "Attachment DataAssets") TArray<UMagazineAttachment*> MagazineArray;
	UPROPERTY(EditAnywhere, Category = "Attachment DataAssets") TArray<UStockAttachment*> StockArray;
	
};

UCLASS()
class APrimaryRifle : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	APrimaryRifle();

	//_______________________________________FUNCTIONS___________________________________________________
	//Firing Weapon
	UFUNCTION(BlueprintCallable) void FireWeapon();
	UFUNCTION(BlueprintCallable) void FullAutoFire();
	UFUNCTION(BlueprintCallable) void CancelFullAutoFire();

	//Ammo / Reload
	UFUNCTION(BlueprintCallable) void DecrementAmmo();
	UFUNCTION(BlueprintCallable) void SetAmmoMax();
	UFUNCTION(BlueprintCallable) void ManualReload();

	//Hit Effect / Damage Indication
	UFUNCTION(BlueprintCallable) void SpawnHitEffect(FVector spawnLocation, FHitResult hitResult);

	//Recoil
	UFUNCTION(BlueprintCallable) FVector BulletRecoilDirection();

	//Data Management
	UFUNCTION(BlueprintCallable) void InitializeStats();
	UFUNCTION(BlueprintCallable) void ChangeAttachments(FString type, int index);


	//_______________________________________PROPERTIES__________________________________________________
	//Gun Functionality
	UPROPERTY(EditAnywhere) int LineTraceDistance = 2000;
	UPROPERTY() int CurrentAmmo = 0;
	UPROPERTY() int MaxAmmo = 25;
	UPROPERTY(EditAnywhere) int Damage = 10;
	UPROPERTY() float FireRate = 0.1f; //In Seconds
	UPROPERTY() float ReloadSpeed = 1.5f; //In Seconds
	UPROPERTY() float MovementSpeed = 600.0f;
	UPROPERTY() float AimWalkingSpeed = 300.0f;
	UPROPERTY() TArray<FVector> RecoilArray; //Holds vector offsets from original hit positon "(0,0)"
	UPROPERTY(BlueprintReadWrite) int recoilCount = 0;
	UPROPERTY(EditAnywhere) FTimerHandle timerHandle;
	UPROPERTY(EditAnywhere) FTimerHandle timerHandle2;
	UPROPERTY(EditAnywhere) TSubclassOf<AActor> HitEffectToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool IsAiming = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool IsReloading = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool DamageEnabled = false;
	
	//User Interface
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UUserWidget* PlayerUserInterface;
	UPROPERTY(BlueprintAssignable) FOnAmmoChanged OnAmmoChanged;
	UPROPERTY(BlueprintAssignable) FOnDamageChanged OnDamageChanged;
	UPROPERTY(BlueprintAssignable) FOnFireRateChanged OnFireRateChanged;
	UPROPERTY(BlueprintAssignable) FOnSpeedChanged OnSpeedChanged;
	UPROPERTY(BlueprintAssignable) FOnAimSpeedChanged OnAimSpeedChanged;
	UPROPERTY(BlueprintAssignable) FOnAmmoCapacityChanged OnAmmoCapacityChanged;
	UPROPERTY(BlueprintAssignable) FOnReloadSpeedChanged OnReloadSpeedChanged;
	
	//Structs
	UPROPERTY(EditAnywhere, Category = "Gun Stats") FGunStats GunStats;
	UPROPERTY(EditAnywhere, Category = "Gun Attachments") FGunAttachments GunAttachments;
	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
