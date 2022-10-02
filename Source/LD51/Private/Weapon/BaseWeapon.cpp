// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include <GameFramework/Pawn.h>
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"


// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	WeaponMeshComponent->SetupAttachment(GetRootComponent());

	LaserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserMeshComponent"));
	LaserMesh->SetupAttachment(WeaponMeshComponent);

	GunMode = EWeaponMode::EWM_Launcher;

}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	TimeToNextShot = TimeBetweenShots;
	EnableLaser(false);
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bUpdateLaser)
	{
		UpdateLaser();
	}
	if(!bIsActivated || bIsFiring) return;
	TimeToNextShot = FMath::Clamp(TimeToNextShot-DeltaTime, 0.f, TimeBetweenShots);
	if(TimeToNextShot <= 0.f)
	{
		bIsFiring = true;
		MakeShot();
	}
}

void ABaseWeapon::MakeShot()
{
	//if(!OwnerCharacter) return;
	switch (GunMode)
	{
	case EWeaponMode::EWM_Laser:
		LaserShot();
		break;
	case EWeaponMode::EWM_Launcher:
		LauncherShot();
		break;
	case EWeaponMode::EWM_Burst:
		BurstShot();
		break;
	}
}

void ABaseWeapon::LaserShot()
{
	EnableLaser(true);
	GetWorldTimerManager().SetTimer(
		FireTimer,
		this,
		&ABaseWeapon::StopLaser,
		LaserDuration
	);
}

void ABaseWeapon::StopLaser()
{
	EnableLaser(false);
	bIsFiring = false;
	TimeToNextShot = TimeBetweenShots;
}

void ABaseWeapon::UpdateLaser()
{
	FHitResult HitResult;
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
	UWorld* World = GetWorld();

	float DistanceToTarget;
	if (MuzzleFlashSocket && World)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		DistanceToTarget = FVector::Distance(
			SocketTransform.GetLocation(),
			HitTarget);

		FVector ToTarget = (HitTarget - SocketTransform.GetLocation());
		FRotator TargetRotation = ToTarget.Rotation();

		LaserMesh->SetWorldRotation(TargetRotation);
		LaserMesh->SetWorldScale3D(FVector(DistanceToTarget / 200.f, LaserWidth, LaserWidth));
	}
}

void ABaseWeapon::LauncherShot()
{
	SpawnProjectile(LauncherProjectile, HitTarget);
	TimeToNextShot = TimeBetweenShots;
	bIsFiring = false;
}

void ABaseWeapon::EnableLaser(bool value)
{
	LaserMesh->SetVisibility(value);
	bUpdateLaser = value;
	if(value)
	{
		LaserMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//LaserBeamEffect->
	}
	else
	{
		LaserMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}

void ABaseWeapon::BurstShot()
{
	
	BulletsToShot = (int32)(ShotsPerSecond*BurstDuration);
	ShotBullet();
}


void ABaseWeapon::ShotBullet()
{
	if(BulletsToShot <= 0)
	{
		bIsFiring = false;
		TimeToNextShot = TimeBetweenShots;
		return;
	}
	SpawnProjectile(BurstProjectile, HitTarget);
	--BulletsToShot;
	const FVector Recoil(FMath::RandRange(MinXRecoil, MaxXRecoil), FMath::RandRange(MinYRecoil, MaxYRecoil), 0.f);
	AddCameraRecoilRotation(Recoil);
	GetWorldTimerManager().SetTimer(
		FireTimer,
		this,
		&ABaseWeapon::ShotBullet,
		BurstDuration/ShotsPerSecond
		);
}

void ABaseWeapon::SpawnProjectile(TSubclassOf<ABaseProjectile> ProjectileToSpawn, FVector HitDirection)
{
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));

	UWorld* World = GetWorld();
	if (MuzzleFlashSocket && World)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		// From muzzle flash socket to HitTarget
		FVector ToTarget = HitDirection - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTarget.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();

		ABaseProjectile* SpawnedProjectile = World->SpawnActor<ABaseProjectile>(ProjectileToSpawn, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
	}
}

void ABaseWeapon::AddCameraRecoilRotation(const FVector CameraRotation)
{
	if(!OwnerCharacter) return;
	OwnerCharacter->AddControllerPitchInput(-CameraRotation.X);
	OwnerCharacter->AddControllerYawInput(CameraRotation.Y);
}

float ABaseWeapon::GetLaserDamgae()
{
	return LaserDamage;
}

void ABaseWeapon::SetMode(EWeaponMode NewMode)
{
	GunMode = NewMode;
}

void ABaseWeapon::Disactivate()
{
	bIsActivated = false;
}

void ABaseWeapon::Activate()
{
	bIsActivated = true;
	TimeToNextShot = TimeBetweenShots;
}

