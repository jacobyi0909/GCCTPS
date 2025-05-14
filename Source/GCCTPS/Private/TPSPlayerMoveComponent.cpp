// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerMoveComponent.h"

#include "EnhancedInputComponent.h"
#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UTPSPlayerMoveComponent::UTPSPlayerMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTPSPlayerMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTPSPlayerMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FTransform t = FTransform(Me->GetControlRotation());
	Direction = t.TransformVector(Direction);
	Me->AddMovementInput(Direction);

	Direction = FVector::ZeroVector;

}

void UTPSPlayerMoveComponent::SetupPlayerInputComp(ATPSPlayer* player, UEnhancedInputComponent* InputComp)
{
	Super::SetupPlayerInputComp(player, InputComp);
	
	InputComp->BindAction(player->IA_TPSMove, ETriggerEvent::Triggered, this, &UTPSPlayerMoveComponent::OnActionMove);
	InputComp->BindAction(player->IA_TPSLook, ETriggerEvent::Triggered, this, &UTPSPlayerMoveComponent::OnActionLook);
	InputComp->BindAction(player->IA_TPSJump, ETriggerEvent::Started, this, &UTPSPlayerMoveComponent::OnActionJump);
	InputComp->BindAction(player->IA_Run, ETriggerEvent::Started, this, &UTPSPlayerMoveComponent::OnActionRunStart);
	InputComp->BindAction(player->IA_Run, ETriggerEvent::Completed, this, &UTPSPlayerMoveComponent::OnActionRunEnd);
	InputComp->BindAction(player->IA_Crouch, ETriggerEvent::Started, this, &UTPSPlayerMoveComponent::OnActionCrouch);
	
}

void UTPSPlayerMoveComponent::OnActionMove(const FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	Direction.X = v.X;
	Direction.Y = v.Y;
}

void UTPSPlayerMoveComponent::OnActionLook(const FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	Me->AddControllerPitchInput(v.Y);
	Me->AddControllerYawInput(v.X);
}

void UTPSPlayerMoveComponent::OnActionJump(const FInputActionValue& value)
{
	// 쪼그려 걷기할 때는 점프를 할 수 없게 하고싶다.
	if (MoveState == EMoveState::Crouching)
		return;

	Me->Jump();
}

void UTPSPlayerMoveComponent::OnActionRunStart(const FInputActionValue& value)
{
	// 달리기는 걷고 있을때만 할 수 있다.
	if (MoveState != EMoveState::Walking)
		return;

	Me->GetCharacterMovement()->MaxWalkSpeed = 1000.f;
	MoveState = EMoveState::Running;
}

void UTPSPlayerMoveComponent::OnActionRunEnd(const FInputActionValue& value)
{
	// 달리기 취소는 달리고 있을때만 할 수 있다.
	if (MoveState != EMoveState::Running)
		return;

	Me->GetCharacterMovement()->MaxWalkSpeed = 600.f;
	MoveState = EMoveState::Walking;
}

void UTPSPlayerMoveComponent::OnActionCrouch(const FInputActionValue& value)
{
	if (Me->bIsCrouched)
	{
		Me->UnCrouch();
		// 걷기 상태로 바꾸고싶다.
		Me->GetCharacterMovement()->MaxWalkSpeed = 600.f;
		MoveState = EMoveState::Walking;
	}
	else
	{
		Me->Crouch(true);
		MoveState = EMoveState::Crouching;
	}
}