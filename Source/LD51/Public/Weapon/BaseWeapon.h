// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectiles/BaseProjectile.h"
#include "BaseWeapon.generated.h"

class ABaseProjectile;
class USkeletalMeshComponent;
class UStaticMeshComponent;
class UNiagaraEmitter;

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
	bool bIsFiring = false;

	UPROPERTY()
	EWeaponMode GunMode;

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
	virtual void UpdateLaser();

	UFUNCTION()
	virtual void StopLaser();

	UFUNCTION()
	virtual void LauncherShot();

	UFUNCTION()
	void EnableLaser(bool value);

	UFUNCTION()
	virtual void BurstShot();

	UFUNCTION()
	void ShotBullet();

	UFUNCTION()
	virtual void SpawnProjectile(TSubclassOf<ABaseProjectile> ProjectileToSpawn, FVector HitDirection);

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
	int32 BulletsToShot;

	FVector HitTarget;

private:
	UPROPERTY(EditAnywhere, Category = "Burst")
	float ShotsPerSecond = 15.f;

	UPROPERTY(EditAnywhere, Category = "Burst")
	float BurstDuration = 2.f;
	int ShotsInBurst;
	FTimerHandle FireTimer;

	UFUNCTION()
	void AddCameraRecoilRotation(const FVector CameraRotation);

	UPROPERTY(EditAnywhere, Category = "BurstRecoil")
	float MinXRecoil = 0.1f;

	UPROPERTY(EditAnywhere, Category = "BurstRecoil")
	float MaxXRecoil = 0.2f;

	UPROPERTY(EditAnywhere, Category = "BurstRecoil")
	float MinYRecoil = -0.1f;

	UPROPERTY(EditAnywhere, Category = "BurstRecoil")
	float MaxYRecoil = 0.1f;

private:
	UPROPERTY(EditAnywhere, Category = "LaserBeam")
	UStaticMeshComponent* LaserMesh;

	UPROPERTY(EditAnywhere, Category = "LaserBeam")
	float LaserDuration = 2.f;
	float LaserTimerRemain = 0.f;

	UPROPERTY(EditAnywhere, Category = "LaserBeam")
	float LaserWidth = 2.f;

	UPROPERTY(EditAnywhere, Category = "LaserBeam")
	float LaserDamage = 100.f;

	bool bUpdateLaser = false;

	UPROPERTY(EditAnywhere, Category = "LaserBeam")
	UNiagaraEmitter* LaserBeamEffect;

	UFUNCTION(BlueprintCallable)
	float GetLaserDamgae();
	
private:


public:	
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() { return WeaponMeshComponent; }
	FORCEINLINE void SetHitTarget (FVector NewHitTarget) { HitTarget = NewHitTarget; }
	FORCEINLINE void SetOwnerCharacter(ACharacter * NewOwner) { OwnerCharacter = NewOwner; }
	
};
