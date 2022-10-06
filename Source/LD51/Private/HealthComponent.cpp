// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamageHandle);
	}
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!EnableRegeneration || !bNeedAutoHealing || GetOwner() == nullptr) return;
	if (LastHitTime + TimeToRegen <= GetWorld()->GetTimeSeconds())
	{
		if (Health < MaxHealth)
		{
			Heal(HealAmount * DeltaTime / Frequency);
		}
		else
		{
			bNeedAutoHealing = false;
		}
	}
}

void UHealthComponent::OnDeath_Implementation()
{
}

void UHealthComponent::OnTakeAnyDamageHandle(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);

	if(EnableRegeneration)
	{
		LastHitTime = GetWorld()->GetTimeSeconds();
		bNeedAutoHealing = true;
	}

	if (Health <= 0.f)
	{
        OnDeath();
	}
}

void UHealthComponent::Heal(float HealValue)
{
	SetHealth(FMath::Clamp(Health + HealValue, 0.f, MaxHealth));
}

float UHealthComponent::GetHealth() const
{
	return Health;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UHealthComponent::SetHealth(float NewValue)
{
	Health = NewValue;
}

