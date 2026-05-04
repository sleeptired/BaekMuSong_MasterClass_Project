// Fill out your copyright notice in the Description page of Project Settings.


#include "Trace_Test.h"
#include "Test_DamageType.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATrace_Test::ATrace_Test()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrace_Test::BeginPlay()
{
	Super::BeginPlay();

	//도전과제 센서 타이머
	GetWorld()->GetTimerManager().SetTimer(SearchTimerHandle, this, &ATrace_Test::StartAsyncSearch, 0.5f, true);
	
}

// Called every frame
void ATrace_Test::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//StartMultiTrace();

	//StartAsyncTrace();
}

void ATrace_Test::StartSingleTrace()
{
	FHitResult HitResult;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		GetActorLocation(),
		GetActorForwardVector() * 1000.f + GetActorLocation(), // GetActorForwardVector() * 1000.f 이거는 방향 벡터만 이니 GetActorLocation()를 더해줘서 1000까지 앞까지 쏘는거다.
		UEngineTypes::ConvertToTraceType(ECC_Visibility),//트레이스 채널 뭘 쓰겠냐?
		false,
		ActorsToIgnore,//누구를 무시처리 할꺼냐 무시처리한 애들은 위에 TArray처럼 담아서 사용한다.
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true,//자기자신의 포함되어 있는 모든 걸 충돌처리를 다 무시한다. (그래서 보통 
		FLinearColor::Red,
		FLinearColor::Green
		);

}

void ATrace_Test::StartMultiTrace()
{
	TArray<FHitResult> HitResult;

	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(this);


	UKismetSystemLibrary::LineTraceMulti(
		GetWorld(),
		GetActorLocation(),
		GetActorForwardVector() * 1000.f + GetActorLocation(),
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green
	);
}

//UWorld에서 가져오는 방식 
void ATrace_Test::StartAsyncTrace()
{
	FTraceDelegate TraceDelegate;
	TraceDelegate.BindUObject(this, &ATrace_Test::OnAsyncTraceCompleted);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.WorldDynamic = ECR_Block;


	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false;


	GetWorld()->AsyncLineTraceByChannel(
		EAsyncTraceType::Multi,
		GetActorLocation(),
		GetActorForwardVector() * 1000.f + GetActorLocation(),
		ECC_Visibility,
		QueryParams,
		ResponseParams,
		&TraceDelegate
	);


}

void ATrace_Test::OnAsyncTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data)
{
	for (const FHitResult& Hit : Data.OutHits)
	{
		AActor* HitActor = Hit.GetActor();

		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Multi Hit Actor : %s"), *HitActor->GetName()));

		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 20.f, 12, FColor::Green, false, 2.f);

		UGameplayStatics::ApplyPointDamage(
			HitActor,
			50.f,
			GetActorForwardVector(),
			Hit,
			GetInstigatorController(),
			this,
			UTest_DamageType::StaticClass()
		);
	}
}

void ATrace_Test::StartAsyncSearch()
{
	FTraceDelegate TraceDelegate;
	TraceDelegate.BindUObject(this, &ATrace_Test::OnSearchCompleted);


	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false;

	FVector StartPos = GetActorLocation();
	FVector EndPos = StartPos + (GetActorForwardVector() * 1500.f);

	GetWorld()->AsyncLineTraceByChannel(
		EAsyncTraceType::Single, // 가장 먼저 걸리는 놈 하나만 찾음
		StartPos,
		EndPos,
		ECC_Pawn, // 검사할 콜리전 채널을 Pawn으로 설정
		QueryParams,
		FCollisionResponseParams::DefaultResponseParam,
		&TraceDelegate
	);
}

void ATrace_Test::OnSearchCompleted(const FTraceHandle& Handle, FTraceDatum& Data)
{
	if (Data.OutHits.Num() > 0)
	{
		for (const FHitResult& Hit : Data.OutHits)
		{
			AActor* HitActor = Hit.GetActor();

			// 맞은 대상이 존재하고, 그 대상이 'Character' 클래스인지 확인
			if (HitActor && HitActor->IsA<ACharacter>())
			{
				// 탐지됬는지 디버그
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("비동기 방식으로 %s 캐릭터 탐지"), *HitActor->GetName()));

				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 30.f, 12, FColor::Red, false, 0.5f);
			}
		}
	}
}

