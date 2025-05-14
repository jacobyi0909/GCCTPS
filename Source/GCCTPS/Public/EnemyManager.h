// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class GCCTPS_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// 타이머를 이용해서... 일정시간마다 적 공장에서 적을 생성해서 내 위치에 배치하고 싶다.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemy> EnemyFactory;

	FTimerHandle EnemyHandle;

	float MinTime = 1.f;
	float MaxTime = 2.f;
	void MakeEnemy();
	
	UPROPERTY(EditAnywhere)
	TArray<class AActor*> SpawnPoints;
	
	void InitSpawnPoints();
	

};
