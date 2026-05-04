// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trace_Test.generated.h"

UCLASS()
class MASTERCLASS_API ATrace_Test : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrace_Test();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartSingleTrace();
	void StartMultiTrace();
	void StartAsyncTrace();
	void OnAsyncTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data);

	//도전과제
	void StartAsyncSearch();
	void OnSearchCompleted(const FTraceHandle& Handle, FTraceDatum& Data);
	FTimerHandle SearchTimerHandle;//Tick보다 타이머를 활용
};
