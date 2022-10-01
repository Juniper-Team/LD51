// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Weapon/BaseWeapon.h>
#include "CombatComponent.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unarmed UMETA(DisplayName = "Unarmed"),
	ECS_Equipped UMETA(DisplayName = "Equipped"),
	ECS_Burst UMETA(DisplayName = "Burst Mode")
};

class ABaseWeapon;
class ACharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class LD51_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	UFUNCTION(BlueprintCallable)
	virtual void SetEquippedWeapon(ABaseWeapon* NewWeapon, USkeletalMeshComponent* AttachMesh, FName SocketName);

protected:
	virtual void BeginPlay() override;

private:
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	ABaseWeapon* EquippedWeapon;
	ACharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceLenght = 10000.f;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector HitTarget;


public:	

		
};
