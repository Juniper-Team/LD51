// Fill out your copyright notice in the Description page of Project Settings

#include "CombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include <Engine/SkeletalMeshSocket.h>

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if(Cast<ACharacter>(GetOwner()))
	{
		OwnerCharacter = Cast<ACharacter>(GetOwner());
	}
}

void UCombatComponent::SetEquippedWeapon(ABaseWeapon* NewWeapon, USkeletalMeshComponent* AttachMesh, FName SocketName)
{
	if (!NewWeapon) return;
	if (!AttachMesh) return;
	const USkeletalMeshSocket* WeaponSocket = AttachMesh->GetSocketByName(SocketName);
	if (WeaponSocket)
	{
		EquippedWeapon = NewWeapon;
		EquippedWeapon->SetOwnerCharacter(OwnerCharacter);
		WeaponSocket->AttachActor(NewWeapon, AttachMesh);
		UE_LOG(LogTemp, Error, TEXT("Success!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Succ:("));
	}
}

ABaseWeapon* UCombatComponent::GetEquippedWeapon()
{
	return EquippedWeapon;
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	FHitResult HitResult;
	if(EquippedWeapon)
	{
		if(EquippedWeapon->GunMode == EWeaponMode::EWM_Laser)
		{
			TraceUnderCrosshairs(HitResult, ECC_GameTraceChannel2);
		}
		else
		{
			TraceUnderCrosshairs(HitResult, ECC_Visibility);
		}
		HitTarget = !HitResult.ImpactPoint.IsZero() ? HitResult.ImpactPoint : HitResult.TraceEnd;
		EquippedWeapon->SetHitTarget(HitTarget);
	}
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult, ECollisionChannel TraceChannel)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + CrosshairWorldDirection * TraceLenght;

		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this->GetOwner());
		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			TraceChannel,
			TraceParams
		);
	}
}


