// Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY(BlueprintReadWrite,meta=(ExposeOnSpawn=true))
	float DamageValue;
	UPROPERTY(BlueprintReadWrite,meta=(ExposeOnSpawn=true))
	FLinearColor DamageColor;
};
