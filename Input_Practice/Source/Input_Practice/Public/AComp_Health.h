// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AComp_Health.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHitDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INPUT_PRACTICE_API UAComp_Health : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAComp_Health();

	UPROPERTY(BlueprintAssignable)
		FHitDelegate OnHit;

	UPROPERTY(BlueprintAssignable)
		FDeathDelegate OnDeath;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP")
		int32 MaxHealth;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnHealthChanged)
	int32 CurrentHealth;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnIsDeadChanged)
	bool IsDead;

	// Called when the game starts
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		int32 GetCurrentHealth() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		int32 GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		bool GetisDead() const;

	UFUNCTION(BlueprintCallable, Category = "CPP")
		void TakeDamage(int32 Damage);

	UFUNCTION()
		void OnHealthChanged();

	UFUNCTION()
		void OnIsDeadChanged();
		
};
