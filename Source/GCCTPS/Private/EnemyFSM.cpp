// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "Enemy.h"
#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GCCTPS/GCCTPS.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	Me = Cast<AEnemy>(GetOwner());

	Me->GetCharacterMovement()->bOrientRotationToMovement = true;
	
}

// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (State)
	{
	case EEnemyState::Idle:
		TickIdle();
		break;
	case EEnemyState::Move:
		TickMove();
		break;
	case EEnemyState::Attack:
		TickAttack();
		break;
	}

	FString log = UEnum::GetValueAsString(State);
	PRINT_LOG(TEXT("%s"), *log);
	DrawDebugString(GetWorld(), Me->GetActorLocation(), log, nullptr, FColor::Yellow, 0, true, 1);
}

void UEnemyFSM::TickIdle()
{
	// 주인공을 찾아서 기억하고싶다.
	auto* c = GetWorld()->GetFirstPlayerController()->GetCharacter();
	Player = Cast<ATPSPlayer>(c);
	// 만약 주인공을 기억했으면
	if (Player)
	{
		//  이동상태로 전이하고싶다.
		State = EEnemyState::Move;
	}
}

void UEnemyFSM::TickMove()
{
	// 주인공을 향해 이동하고싶다.
	FVector destination = Player->GetActorLocation();
	FVector dir = destination - Me->GetActorLocation();
	Me->AddMovementInput(dir.GetSafeNormal2D());
}

void UEnemyFSM::TickAttack()
{
}

