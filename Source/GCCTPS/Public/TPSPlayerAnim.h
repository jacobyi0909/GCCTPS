// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GCCTPS/GCCTPS.h"
#include "TPSPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class GCCTPS_API UTPSPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY()
	class ATPSPlayer* Player;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PitchAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMoveState MoveState;
	
};
