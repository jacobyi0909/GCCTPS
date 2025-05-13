// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"

#include "Enemy.h"

void UEnemyAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Me = Cast<AEnemy>(TryGetPawnOwner());
}

void UEnemyAnim::PlayDamageAnimation()
{
	if (Me && Montage)
	{
		int32 randValue = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), randValue);
		Me->PlayAnimMontage(Montage, 1, FName(*sectionName));
	}
}

void UEnemyAnim::PlayDieAnimation()
{
	if (Me && Montage)
	{
		Montage_Play(Montage, 1);
		Montage_JumpToSection("Die", Montage);

		//Me->PlayAnimMontage(Montage, 1, "Die");
	}
}

void UEnemyAnim::AnimNotify_EnemyKickEnd()
{
	bAttack = false;
}

void UEnemyAnim::AnimNotify_DamageEnd()
{
	// 몽타주를 멈추고싶다.
	if (Me && Montage)
	{
		Montage_Stop(0.2f, Montage);
		Me->EnemyFSM->SetState(EEnemyState::Move);
	}
}

void UEnemyAnim::AnimNotify_DieEnd()
{
	bDie = true;
}

void UEnemyAnim::AnimNotify_Hit()
{
	if (Me && Montage)
	{
		Me->EnemyFSM->DoHit();
	}
}
