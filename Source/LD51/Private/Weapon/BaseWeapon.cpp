// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	WeaponMeshComponent->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	TimeToNextShot = TimeBetweenShots;
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bIsActivated) return;
	TimeToNextShot -= DeltaTime;
	if(TimeToNextShot<= 0)
	{
		MakeShot();
	}
}

void ABaseWeapon::MakeShot()
{
	if(!OwnerCharacter) return;
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
	
}

void ABaseWeapon::LauncherShot(const FVector& HitTarget)
{
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));

	UWorld* World = GetWorld();
	if (MuzzleFlashSocket && World)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		// From muzzle flash socket to HitTarget
		FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTarget.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();

		ABaseProjectile* SpawnedProjectile = World->SpawnActor<ABaseProjectile>(LauncherProjectile, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
	}
}

void ABaseWeapon::BurstShot()
{
	
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

