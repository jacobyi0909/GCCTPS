// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "Enemy.h"
#include "EnemyAnim.h"
#include "TPSPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GCCTPS/GCCTPS.h"
#include "AIController.h"
#include "EnemyHPWidget.h"
#include "NavigationSystem.h"
#include "Components/WidgetComponent.h"
#include "Navigation/PathFollowingComponent.h"

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

	// AI컨트롤러를 기억하고싶다.
	AI = Cast<AAIController>(Me->Controller);

	UpdatePatrolLocation(Me->GetActorLocation(), 1000.f, PatrolLocation);

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


	// AI를 이용해서 길찾기를 하고싶다.
	//Me->AddMovementInput(dir.GetSafeNormal2D());

	// 도착지의 위치가 Player위치라서 겹치게 된다.

	// 플레이어가 길위에 있는가?
	UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FPathFindingQuery query;
	FAIMoveRequest req;
	req.SetAcceptanceRadius(100.f);
	req.SetGoalLocation(destination);
	
	AI->BuildPathfindingQuery(req, query);

	FPathFindingResult findPathResult = ns->FindPathSync(query);
	// 플레이어가 길위에 있는가?
	if (findPathResult.IsSuccessful())
	{
		//  - 플레이어의 위치를 향해 이동하고싶다.
		AI->MoveToLocation(destination, 100.f);
	}
	// 그렇지 않다면(길위가 아니라면)
	else
	{
		//  - 랜덤한 위치로 이동하고싶다.
		EPathFollowingRequestResult::Type r = AI->MoveToLocation(PatrolLocation, 100.f);
		//   - 만약 랜덤한 위치에 도착했다면
		if (r == EPathFollowingRequestResult::Type::AlreadyAtGoal ||
			r == EPathFollowingRequestResult::Type::Failed)
		{
			//    => 랜덤한 위치를 재설정 하고싶다.
			UpdatePatrolLocation(Me->GetActorLocation(), 1000.f, PatrolLocation);
		}
	}

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
		}
		//  그렇지 않다면
		else
		{
			// 이동상태로 전이하고싶다.
			SetState(EEnemyState::Move);
		}
	}
}

void UEnemyFSM::TickDamage()
{
	// // 1초동안 멈췄다가 이동상태로 전이하고싶다.
	// CurrentTime += GetWorld()->GetDeltaSeconds();
	// if (CurrentTime > 1)
	// {
	// 	SetState(EEnemyState::Move);
	// }
}

void UEnemyFSM::TickDie()
{
	// 만약 죽음 애니메이션이 끝나지 않았으면 함수 종료
	if (false == EnemyAnim->bDie)
		return;
	
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
	HP = FMath::Max(0, HP - damage);
	// 만약 체력이 0보다 크다면 Damage상태로 전이하고
	if (HP > 0)
	{
		SetState(EEnemyState::Damage);
		EnemyAnim->PlayDamageAnimation();
	}
	// 그렇지 않다면 죽음상태로 전이하고싶다.
	else
	{
		SetState(EEnemyState::Die);
		Me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		EnemyAnim->PlayDieAnimation();
	}
}

void UEnemyFSM::SetHP(float newHP)
{
	CurHp = newHP;
	HpWidget->UpdateHP(CurHp, MaxHp);
}

float UEnemyFSM::GetHP() const
{
	return CurHp;
}

void UEnemyFSM::SetState(EEnemyState next)
{
	State = next;
	EnemyAnim->State = next;
	CurrentTime = 0;
	if (next != EEnemyState::Move)
	{
		AI->StopMovement();
	}
}


bool UEnemyFSM::UpdatePatrolLocation(FVector origin, float radius, FVector& outLocation)
{
	auto* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	if (ns->GetRandomReachablePointInRadius(origin, radius,loc))
	{
		outLocation = loc.Location;
		return true;
	}
	return false;
}

void UEnemyFSM::InitHp()
{
	// 오너의 HpWidget를 기억하고싶다.
	HpWidget = Cast<UEnemyHPWidget>(Me->HpWidgetComp->GetWidget());

	// 태어날 때 현재 체력을 최대 체력으로 초기화하고싶다.
	HP = MaxHp;
}

void UEnemyFSM::DoHit()
{
	// 만약 상대가 공격 가능 거리이고 나와 상대와의 각도가 30도 이하라면
	FVector dir = Player->GetActorLocation() - Me->GetActorLocation();
	float dist = dir.Length();
	
	if (dist < AttackRange && GetFovAngle(Me->GetActorForwardVector(), dir) <= 30)
	{
		// 공격하고싶다.
		UE_LOG(LogTemp, Warning, TEXT("DoHit"));
		Player->DoDamageFromEnemy(1);
	}
}

float UEnemyFSM::GetFovAngle(FVector forward, FVector targetDir)
{
	float dot = FVector::DotProduct(forward.GetSafeNormal2D(), targetDir.GetSafeNormal2D());

	float degree = FMath::RadiansToDegrees(FMath::Acos(dot));

	return degree;
}

