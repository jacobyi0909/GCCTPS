// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class GCCTPS_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 태어날 때 오너(Enemy)를 가져와서 기억하고싶다.
	virtual void NativeBeginPlay() override;

	UPROPERTY()
	class AEnemy* Me;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	EEnemyState State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool bAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* Montage;

	void PlayDamageAnimation();

	// 적이 죽으면 PlayDieAnimation기능이 호출되게 하고싶다.
	// Die 섹션을 재생하고싶다.
	void PlayDieAnimation();
	
	
	UFUNCTION()
	void AnimNotify_EnemyKickEnd();

	UFUNCTION()
	void AnimNotify_DamageEnd();

	bool bDie = false;
	
	UFUNCTION()
	void AnimNotify_DieEnd();

	UFUNCTION()
	void AnimNotify_Hit();

	
};
