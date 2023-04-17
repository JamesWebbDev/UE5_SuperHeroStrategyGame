// Fill out your copyright notice in the Description page of Project Settings.


#include "AComp_Attack.h"

// Sets default values for this component's properties
UAComp_Attack::UAComp_Attack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAComp_Attack::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	const int32 ListLength = AttackList.Num();

	if (ListLength == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO Attacks found in Component!"));
		return;
	}
	if (ListLength > ListMax)
	{
		UE_LOG(LogTemp, Warning, TEXT("TOO MANY Attacks found in Component!"));
		return;
	}

	SelectedAttack = AttackList[0];
}


// Called every frame
void UAComp_Attack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<UDA_Attack*> UAComp_Attack::GetAttackList() const
{
	return AttackList;
}

UDA_Attack* UAComp_Attack::GetAttack() const
{
	return SelectedAttack;
}

void UAComp_Attack::GetAttackVariables(int32& OutDamage, bool& OutIsAOE, int32& OutAttackRange, TArray<FVector2D>& OutAttackTiles) const
{
	if (SelectedAttack == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO Attack is Selected on Component."));
		return;
	}

	OutDamage = SelectedAttack->GetDamage();
	OutIsAOE = SelectedAttack->GetisAOE();
	OutAttackRange = SelectedAttack->GetAttackRange();
	OutAttackTiles = SelectedAttack->GetAttackTiles();
}

void UAComp_Attack::GetInfo(FText& OutName, FText& OutDescription, UTexture2D*& OutImage, TSubclassOf<class UAnimInstance>& OutAnim) const
{
	if (SelectedAttack == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO Attack is Selected on Component."));
		return;
	}

	OutName = SelectedAttack->GetName();
	OutDescription = SelectedAttack->GetDescription();
	OutImage = SelectedAttack->GetImage();
	OutAnim = SelectedAttack->GetAnimInstance();
}

int32 UAComp_Attack::GetListLength() const
{
	return AttackList.Num();
}

bool UAComp_Attack::GetAttackAtIndex(int32 InIndex, UDA_Attack*& OutAttack)
{
	if (AttackList[InIndex] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't get Attack in Component at index %d."), InIndex);
		return false;
	}

	OutAttack = AttackList[InIndex];
	return true;
}

void UAComp_Attack::SetNewAttack(int32 InAttackIndex)
{
	if (InAttackIndex >= AttackList.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("New attack could not be set from index %d."), InAttackIndex);
		return;
	}

	SelectedAttack = AttackList[InAttackIndex];

	if (OnAttackSet.IsBound())
	{
		OnAttackSet.Broadcast(SelectedAttack);
		UE_LOG(LogTemp, Display, TEXT("Successfully Executed 'OnAttackSet' delegate."));
	}
}

