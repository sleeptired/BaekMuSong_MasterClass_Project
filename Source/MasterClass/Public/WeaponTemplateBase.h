// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponTemplateBase.generated.h"

/**
 * 
 */
UCLASS()
class MASTERCLASS_API AWeaponTemplateBase : public AWeaponBase
{
	GENERATED_BODY()

public:

	virtual void Fire() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Reload();

protected:

	UFUNCTION(BlueprintNativeEvent)
	bool CheckAmmo();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ProcessFiring();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayEffects();
	UFUNCTION(BlueprintNativeEvent)
	void UpdateAmmo();
	
};
