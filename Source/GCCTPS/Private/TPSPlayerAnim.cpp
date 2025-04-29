// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerAnim.h"
#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTPSPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Player = Cast<ATPSPlayer>(TryGetPawnOwner());
}

void UTPSPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 폴링 
	// 1. 오너가 유효하지 않으면 종료
	if (nullptr == Player || false == Player->IsValidLowLevel())
		return;
	
	// 2. Speed값을 채우고싶다.
	Speed = FVector::DotProduct(Player->GetVelocity(), Player->GetActorForwardVector());

	// 3. Direction을 채우고싶다.
	Direction = FVector::DotProduct(Player->GetVelocity(), Player->GetActorRightVector());

	// 4. 오너가 캐릭터이고 공중에 있다면 bIsInAir에 값을 채우고싶다.
	bIsInAir = Player->GetCharacterMovement()->IsFalling();

	// PitchAngle을 채우고싶다.
	PitchAngle = Player->GetBaseAimRotation().Pitch;

	// 플레이어의 이동상태를 동기화 하고싶다.
	MoveState = Player->MoveState;
}
