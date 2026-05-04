// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"

AShotgun::AShotgun()
{

	AmmoPerFire = 1;
	MaxAmmo = 8;
	CurrentAmmo = MaxAmmo;
	DamagePerHit = 15.f;
	Range = 1500.f;

	PelletCount = 8;
	SpreadAngle = 15.f;
	RecoilPitch = -2.0f;
}

void AShotgun::ProcessFiring_Implementation()
{
	if (!FirePoint)
	{
		return;
	}

	FVector StartPos = FirePoint->GetComponentLocation();
	FVector ForwardDir = FirePoint->GetForwardVector();

	FTraceDelegate TraceDelegate;
	TraceDelegate.BindUObject(this, &AShotgun::OnAsyncPelletHit);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		QueryParams.AddIgnoredActor(PlayerPawn);
	}
	QueryParams.bTraceComplex = false;

	for (int32 i = 0; i < PelletCount; i++)
	{
		// 부채꼴 범위 랜덤 방향
		FVector RandomDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(ForwardDir, SpreadAngle);
		FVector EndPos = StartPos + (RandomDir * Range);


		GetWorld()->AsyncLineTraceByChannel(
			EAsyncTraceType::Single,
			StartPos,
			EndPos,
			ECC_Visibility,
			QueryParams,
			FCollisionResponseParams::DefaultResponseParam,
			&TraceDelegate,
			i // UserData
		);
	}
}

void AShotgun::OnAsyncPelletHit(const FTraceHandle& Handle, FTraceDatum& Data)
{
	FVector TraceStart = Data.Start;
	FVector TraceEnd = Data.End;
	FVector ShotDir = (TraceEnd - TraceStart).GetSafeNormal();

	// 맞은 물체가 하나라도 있다면?
	if (Data.OutHits.Num() > 0)
	{
		for (const FHitResult& Hit : Data.OutHits)
		{
			// 발사 선- 빨간, 충돌 - 초록 박스
			DrawDebugLine(GetWorld(), TraceStart, Hit.ImpactPoint, FColor::Red, false, 1.5f, 0, 1.0f);
			DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5.f), FQuat::Identity, FColor::Green, false, 1.5f);

			// 데미지 적용
			//AActor* HitActor = Hit.GetActor();
			//if (HitActor)
			//{
			//	
			//	UGameplayStatics::ApplyPointDamage(
			//		HitActor,
			//		DamagePerHit,
			//		ShotDir,
			//		Hit,
			//		GetInstigatorController(),
			//		this,
			//		UDamageType::StaticClass()
			//	);
			//}
		}
	}
	else
	{
		// 충돌이 안된 경우
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.5f, 0, 1.0f);
	}
}

void AShotgun::PlayEffects_Implementation()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		// 위로 튀는 반동 적용
		PC->AddPitchInput(RecoilPitch);
	}
}
