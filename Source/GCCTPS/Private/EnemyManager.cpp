// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"

#include "Enemy.h"
#include "EngineUtils.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	// 태어날 위치목록을 기억하고싶다.
	InitSpawnPoints();
	
	// 타이머를 이용해서... 일정시간마다 적 공장에서 적을 생성해서 내 위치에 배치하고 싶다.
	GetWorldTimerManager().SetTimer(
		EnemyHandle,
		this,
		&AEnemyManager::MakeEnemy,
		FMath::RandRange(MinTime, MaxTime),
		false
	);
	
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyManager::MakeEnemy()
{
	// 적을 스폰
	int32 index = FMath::RandRange(0, SpawnPoints.Num() - 1);
	GetWorld()->SpawnActor<AEnemy>(EnemyFactory, SpawnPoints[index]->GetActorTransform());
	
	GetWorldTimerManager().SetTimer(
		EnemyHandle,
		this,
		&AEnemyManager::MakeEnemy,
		FMath::RandRange(MinTime, MaxTime),
		false
	);
}

void AEnemyManager::InitSpawnPoints()
{
	SpawnPoints.Empty(4);
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		// 만약 Actor의 이름에 "SpawnPoint"라는 이름을 포함하고 있다면
		//if (Actor->GetActorNameOrLabel().Contains(TEXT("SpawnPoint")))
		if (Actor->ActorHasTag(TEXT("SpawnPoint")))
		{
			// 목록에 추가하고싶다.
			SpawnPoints.Add(Actor);
		}
	}
}

