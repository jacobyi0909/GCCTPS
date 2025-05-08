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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	EEnemyState State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool bAttack;

	UFUNCTION()
	void AnimNotify_EnemyKickEnd();
	
};
