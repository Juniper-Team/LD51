// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class LD51_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnDeath();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "HealthStats")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, Category = "HealthStats")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, Category = "Regeneration")
	bool EnableRegeneration = true;

	UPROPERTY(EditAnywhere, Category = "Regeneration")
	float TimeToRegen = 5.f;

	UPROPERTY(EditAnywhere, Category = "Regeneration")
	float Frequency = 0.25f;

	UPROPERTY(EditAnywhere, Category = "Regeneration")
	float HealAmount = 5.0f;

	bool bNeedAutoHealing = false;
	float LastHitTime = 0.f;

	UFUNCTION()
	virtual void OnTakeAnyDamageHandle(AActor* DamageActor, float Damage, const UDamageType* DamageType,
			AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void Heal(float HealValue);

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	void SetHealth(float NewValue);

public:	
		
};
