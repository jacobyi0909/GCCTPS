// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSPlayerBaseComponent.h"
#include "GCCTPS/GCCTPS.h"
#include "InputActionValue.h"
#include "TPSPlayerMoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GCCTPS_API UTPSPlayerMoveComponent : public UTPSPlayerBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTPSPlayerMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupPlayerInputComp(class ATPSPlayer* player, class UEnhancedInputComponent* InputComp) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Direction;

	EMoveState MoveState = EMoveState::Walking;

	void OnActionMove(const FInputActionValue& value);
	void OnActionLook(const FInputActionValue& value);
	void OnActionJump(const FInputActionValue& value);
	void OnActionRunStart(const FInputActionValue& value);
	void OnActionRunEnd(const FInputActionValue& value);
	void OnActionCrouch(const FInputActionValue& value);
		
};
