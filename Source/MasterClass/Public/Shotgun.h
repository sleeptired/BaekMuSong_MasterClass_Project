// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTemplateBase.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class MASTERCLASS_API AShotgun : public AWeaponTemplateBase
{
	GENERATED_BODY()

public:
	AShotgun();

protected:
	// 부모의 빈 조각(NativeEvent)을 샷건에 맞게 구현합니다.
	virtual void ProcessFiring_Implementation() override;
	virtual void PlayEffects_Implementation() override;

	void OnAsyncPelletHit(const FTraceHandle& Handle, FTraceDatum& Data);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotgun Stats")
	int32 PelletCount; // 한 번에 발사될 산탄 조각의 개수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotgun Stats")
	float SpreadAngle; // 총알이 퍼지는 각도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotgun Stats")
	float RecoilPitch; // 위로 튀는 반동 값 
};
