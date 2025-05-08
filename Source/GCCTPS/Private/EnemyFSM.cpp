// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "Enemy.h"
#include "EnemyAnim.h"
#include "TPSPlayer.h"
#include "Components/CapsuleComponent.h"
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

	EnemyAnim = Cast<UEnemyAnim>(Me->GetMesh()->GetAnimInstance());
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
	case EEnemyState::Damage:
		TickDamage();
		break;
	case EEnemyState::Die:
		TickDie();
		break;
	}

	FString log = FString::Printf(TEXT("State : %s\nbAttack : %d"),
		*UEnum::GetValueAsString(State), EnemyAnim->bAttack);

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
		SetState(EEnemyState::Move);
	}
}

void UEnemyFSM::TickMove()
{
	// 주인공을 향해 이동하고싶다.
	FVector destination = Player->GetActorLocation();
	FVector dir = destination - Me->GetActorLocation();
	Me->AddMovementInput(dir.GetSafeNormal2D());

	// 만약 공격 가능한 거리라면
	//float dist = FVector::Dist(Player->GetActorLocation(), Me->GetActorLocation());
	float dist = dir.Length();
	if (dist < AttackRange)
	{
		// 공격상태로 전이하고싶다.
		SetState(EEnemyState::Attack);
		EnemyAnim->bAttack = true;
	}
}

void UEnemyFSM::TickAttack()
{
	// 시간이 흐르다가
	CurrentTime += GetWorld()->GetDeltaSeconds();
	// 만약 현재시간이 공격 대기시간을 초과하면
	if (CurrentTime > AttackDelayTime)
	{
		CurrentTime = 0;
		FVector dir = Player->GetActorLocation() - Me->GetActorLocation();
		//	만약 상대가 공격 가능하다면
		if (dir.Length() < AttackRange)
		{
			EnemyAnim->bAttack = true;
			PRINT_LOG(TEXT("Attack!!!"));
		}
		//  그렇지 않다면
		else
		{
			//		이동상태로 전이하고싶다.
			SetState(EEnemyState::Move);
		}
	}
}

void UEnemyFSM::TickDamage()
{
	// 1초동안 멈췄다가 이동상태로 전이하고싶다.
	CurrentTime += GetWorld()->GetDeltaSeconds();
	if (CurrentTime > 1)
	{
		SetState(EEnemyState::Move);
	}
}

void UEnemyFSM::TickDie()
{
	// 1초동안 아래로 이동하고싶다.  
	CurrentTime += GetWorld()->GetDeltaSeconds();
	FVector dir(0,0,-1);
	FVector newLoc = Me->GetActorLocation() + dir * DieDownSpeed * GetWorld()->GetDeltaSeconds();
	Me->SetActorLocation(newLoc);
	if (CurrentTime > 1)
	{
		// 1초가 지난 후 파괴되고싶다.
		Me->Destroy();
	}
}

void UEnemyFSM::OnMyTakeDamage(int32 damage)
{
	PRINT_LOG(TEXT("OnMyTakeDamage"));
	// 체력을 damage만큼 감소시키고싶다.
	CurHp = FMath::Max(0, CurHp - damage);
	// 만약 체력이 0보다 크다면 Damage상태로 전이하고
	if (CurHp > 0)
	{
		SetState(EEnemyState::Damage);
	}
	// 그렇지 않다면 죽음상태로 전이하고싶다.
	else
	{
		SetState(EEnemyState::Die);
		Me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UEnemyFSM::SetState(EEnemyState next)
{
	State = next;
	EnemyAnim->State = next;
	CurrentTime = 0;
}
