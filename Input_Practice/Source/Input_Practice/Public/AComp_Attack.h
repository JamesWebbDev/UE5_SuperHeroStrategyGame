// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DA_Attack.h"
#include "AComp_Attack.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttackSetDelegate, UDA_Attack*, NewAttack);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INPUT_PRACTICE_API UAComp_Attack : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAComp_Attack();

	UPROPERTY(BlueprintAssignable)
		FAttackSetDelegate OnAttackSet;

private:

	UPROPERTY(EditDefaultsOnly, Category = "CPP", meta = (ClampMin = 0, ClampMax = 4))
		int32 ListMax = 4;

	UPROPERTY(EditAnywhere, Category = "CPP")
		TArray<UDA_Attack*> AttackList;

	UDA_Attack* SelectedAttack;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		TArray<UDA_Attack*> GetAttackList() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		UDA_Attack* GetAttack() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		void GetAttackVariables(int32& OutDamage, bool& OutIsAOE, int32& OutAttackRange, TArray<FVector2D>& OutAttackTiles) const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		void GetInfo(FText& OutName, FText& OutDescription, UTexture2D*& OutImage, TSubclassOf<class UAnimInstance>& OutAnim) const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		int32 GetListLength() const;
	
	UFUNCTION(BlueprintPure, Category = "CPP")
		bool GetAttackAtIndex(int32 InIndex, UDA_Attack*& OutAttack);

	UFUNCTION(BlueprintCallable, Category = "CPP")
		void SetNewAttack(int32 InAttackIndex);

};
