// Fill out your copyright notice in the Description page of Project Settings.

#include "AComp_Health.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UAComp_Health::UAComp_Health()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAComp_Health::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentHealth = MaxHealth;
}


// Called every frame
void UAComp_Health::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int32 UAComp_Health::GetCurrentHealth() const
{
	return CurrentHealth;
}

int32 UAComp_Health::GetMaxHealth() const
{
	return MaxHealth;
}

bool UAComp_Health::GetisDead() const
{
	return IsDead;
}

void UAComp_Health::TakeDamage(int32 Damage)
{
	CurrentHealth -= Damage;

	OnHealthChanged();
}

void UAComp_Health::OnHealthChanged()
{
	if (OnHit.IsBound())
	{
		OnHit.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("Successfully triggered bound OnHit Delegate."));
	}

	if (CurrentHealth <= 0) 
	{
		IsDead = true;
		OnIsDeadChanged();
	}
}

void UAComp_Health::OnIsDeadChanged()
{
	if (IsDead && OnDeath.IsBound())
	{
		OnDeath.Broadcast();
		UE_LOG(LogTemp, Display, TEXT("Successfully triggered bound OnDeath Delegate."));
	}
}

void UAComp_Health::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAComp_Health, CurrentHealth);
	DOREPLIFETIME(UAComp_Health, IsDead);
}
