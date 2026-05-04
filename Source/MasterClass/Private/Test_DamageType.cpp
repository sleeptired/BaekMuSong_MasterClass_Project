// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_DamageType.h"

UTest_DamageType::UTest_DamageType()
{
	// 1. 물리적 충격량 설정 (PointDamage 시 적용됨)
	DamageImpulse = 5000.f;

	// 2. 무게에 따른 밀림 차등 적용
	bScaleMomentumByMass = true;

	// 3. 환경 데미지 여부 (AI가 범인을 찾지 못하게 할 때 유용)
	bCausedByWorld = true;
}
