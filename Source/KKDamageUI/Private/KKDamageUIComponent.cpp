// Copyright CYC 2023, Inc. All Rights Reserved.


#include "KKDamageUIComponent.h"

#include "DamageUIBase.h"
#include "Engine/UserInterfaceSettings.h"
#include "Engine/World.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UKKDamageUIComponent::UKKDamageUIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

void UKKDamageUIComponent::UpdateDamageUI()
{
	FVector2D ViewPortSize;
	UWorld * World = GetWorld();
	if (!World)
	{
		return;
	}
	UGameViewportClient * ViewportClient = World->GetGameViewport();
	if (!ViewportClient)
	{
		return;
	}
	ViewportClient->GetViewportSize(ViewPortSize);
	FIntPoint Size(FMath::TruncToInt(ViewPortSize.X),FMath::TruncToInt(ViewPortSize.Y));
	
	float DPIScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(Size);
	
	TArray<ADamageUIBase *> InValidList;
	int i = 0;
	for (ADamageUIBase * it : DamageUIList)
	{
		if (!it)
		{
			InValidList.Add(it);
			continue;
		}
		it->SetActorLocation(it->RootPos +it->RandomRootOffset + i * DPIScale * it->RandomVerticalOffset);
		i++;
	}

	for (ADamageUIBase * it : InValidList)
	{
		DamageUIList.RemoveSwap(it);
	}
}

FVector UKKDamageUIComponent::GetRandomVector(const FVector & Min,const FVector & Max)
{
	FVector Tmp;
	Tmp.X = FMath::RandRange(Min.X,Max.X);
	Tmp.Y = FMath::RandRange(Min.Y,Max.Y);
	Tmp.Z = FMath::RandRange(Min.Z,Max.Z);
	return Tmp;
}

FLinearColor UKKDamageUIComponent::GetRandomColorFromList()
{
	FLinearColor Tmp(1,1,1,1);
	if (RandomColorList.Num())
	{
		Tmp = RandomColorList[FMath::RandRange(0,RandomColorList.Num() -1 )];
	}
	return Tmp;
}

FLinearColor UKKDamageUIComponent::GetColorFromDamage(float DamageValue)
{
	FLinearColor Tmp(1,1,1,1);
	for (TTuple<float,FLinearColor> it : DamageColorMapList)
	{
		if (DamageValue > it.Key)
		{
			Tmp = it.Value;
		}
	}
	return Tmp;
}


void UKKDamageUIComponent::KKAddDamageUI(FVector Pos, float DamageValue,bool bMultiCast)
{
	if (!SpawnedClass && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,TEXT("Please Set The DamageUI Class In KKDamageUIComponent"));
		return;
	}
	// 必须开启组件的Replicated才能正常使用MultiCast
	if (bMultiCast && !GetIsReplicated())
	{
		SetIsReplicated(true);
	}
	if (bMultiCast)
	{
		KKAddDamageUI_Multi(Pos,DamageValue);
	}
	else
	{
		KKAddDamageUI_Client(Pos,DamageValue);
	}
}

void UKKDamageUIComponent::KKAddDamageUI_Multi_Implementation(FVector Pos, float DamageValue)
{
	KKSpawnAndAddDamageUIActor(Pos,DamageValue);
}

void UKKDamageUIComponent::KKAddDamageUI_Client_Implementation(FVector Pos, float DamageValue)
{
	KKSpawnAndAddDamageUIActor(Pos,DamageValue);
}

void UKKDamageUIComponent::KKSpawnAndAddDamageUIActor(FVector Pos,float DamageValue)
{
	check(SpawnedClass!=nullptr)
	FTransform SpawnTransform(FRotator::ZeroRotator,Pos);
	ADamageUIBase * SpawnedActor = GetWorld()->SpawnActorDeferred<ADamageUIBase>(SpawnedClass,SpawnTransform,GetOwner());
	SpawnedActor->DamageValue = DamageValue;
	SpawnedActor->DamageColor = bRandomColor ? GetRandomColorFromList() : GetColorFromDamage(DamageValue);
	SpawnedActor->RootPos = Pos;
	SpawnedActor->RandomRootOffset = GetRandomVector(MinRootOffset,MaxRootOffset);
	SpawnedActor->RandomVerticalOffset = GetRandomVector(MinVerticalOffset,MaxVerticalOffset);
	SpawnedActor->FinishSpawning(SpawnTransform);
	
	DamageUIList.Insert(SpawnedActor,0);
	UpdateDamageUI();
}


