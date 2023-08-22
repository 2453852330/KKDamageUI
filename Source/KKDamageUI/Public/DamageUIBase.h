// Copyright CYC 2023, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageUIBase.generated.h"

UCLASS(Abstract)
class KKDAMAGEUI_API ADamageUIBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageUIBase();
	
	
	UPROPERTY(BlueprintReadWrite,meta=(ExposeOnSpawn=true),Category=DamageUI)
	float DamageValue;
	UPROPERTY(BlueprintReadWrite,meta=(ExposeOnSpawn=true),Category=DamageUI)
	FLinearColor DamageColor;
	
	FVector RootPos;
	FVector RandomRootOffset;
	FVector RandomVerticalOffset;
};
