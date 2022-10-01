// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class ABaseProjectile;

UENUM(BlueprintType)
enum class EWeaponMode : uint8
{
	EWM_Laser UMETA (DisplayName = "Laser Mode"),
	EWM_Launcher UMETA (DisplayName = "Launcher Mode"),
	EWM_Burst UMETA (DisplayName = "Burst Mode")
};

UCLASS()
class LD51_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();

	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY()
	bool bIsActivated = false;

	UPROPERTY()
	EWeaponMode GunMode = EWeaponMode::EWM_Laser;

	UFUNCTION(BlueprintCallable)
	void SetMode(EWeaponMode NewMode);

	UFUNCTION(BlueprintCallable)
	void Disactivate();

	UFUNCTION(BlueprintCallable)
	void Activate();

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	virtual void MakeShot();

	UFUNCTION()
	virtual void LaserShot();

	UFUNCTION()
	virtual void LauncherShot();

	UFUNCTION()
	virtual void BurstShot();

protected:
	UPROPERTY(EditAnywhere, Category = "Projectiles")
	TSubclassOf <ABaseProjectile> BurstProjectile;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
	TSubclassOf <ABaseProjectile> LauncherProjectile;
	
private: 
	UPROPERTY(EditAnywhere, Category = "Weapon Mesh")
	USkeletalMeshComponent* WeaponMeshComponent;

	ACharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere, Category = "Shot Proprties")
	float TimeBetweenShots = 10.f;
	float TimeToNextShot;


public:	
};
