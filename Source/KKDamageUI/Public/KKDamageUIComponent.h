// Copyright CYC 2023, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KKDamageUIComponent.generated.h"


class ADamageUIBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KKDAMAGEUI_API UKKDamageUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKKDamageUIComponent();


	UPROPERTY(EditDefaultsOnly,Category="Offset|BaseSet")
	FVector RootOffset;
	UPROPERTY(EditDefaultsOnly,Category="Offset|BaseSet")
	FVector VerticalOffset;
	UPROPERTY(EditDefaultsOnly,Category="Offset|BaseSet")
	TSubclassOf<class ADamageUIBase> SpawnedClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly,Category="Offset|RandomRootOffset")
	bool bRandomRootOffset = false;
	UPROPERTY(EditDefaultsOnly,Category="Offset|RandomRootOffset")
	FVector MinRootOffset;
	UPROPERTY(EditDefaultsOnly,Category="Offset|RandomRootOffset")
	FVector MaxRootOffset;
	
	UPROPERTY(EditDefaultsOnly,Category="Offset|RandomVerticalOffset")
	bool bRandomVerticalOffset = false;
	UPROPERTY(EditDefaultsOnly,Category="Offset|RandomVerticalOffset")
	FVector MinVerticalOffset;
	UPROPERTY(EditDefaultsOnly,Category="Offset|RandomVerticalOffset")
	FVector MaxVerticalOffset;

	UPROPERTY(EditDefaultsOnly,Category="Color|Random")
	bool bRandomColor = false;
	UPROPERTY(EditDefaultsOnly,Category="Color|Random")
	TArray<FLinearColor> RandomColorList;

	UPROPERTY(EditDefaultsOnly,Category="Color|DamageMap")
	TMap<float,FLinearColor> DamageColorMapList;

	
	UFUNCTION(BlueprintCallable,Category="KKDamageUI")
	void KKAddDamageUI(FVector Pos,float DamageValue,bool bMultiCast);

	UFUNCTION(Client,Unreliable)
	void KKAddDamageUI_Client(FVector Pos,float DamageValue);
	UFUNCTION(NetMulticast,Unreliable)
	void KKAddDamageUI_Multi(FVector Pos,float DamageValue);

private:
	UPROPERTY()
	TArray<ADamageUIBase * > DamageUIList;
	
	void KKSpawnAndAddDamageUIActor(FVector Pos,float DamageValue);
	void UpdateDamageUI();
	FVector GetRandomVector(const FVector & Min,const FVector & Max);
	FLinearColor GetRandomColorFromList();
	FLinearColor GetColorFromDamage(float DamageValue);
};
