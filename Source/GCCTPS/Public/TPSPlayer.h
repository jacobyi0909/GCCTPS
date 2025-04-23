// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class GCCTPS_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* IMC_TPSPlayer;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_TPSMove;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_TPSLook;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_TPSJump;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_TPSFire;
	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyControllerChanged() override;

	// 1. Mesh의 내용을 채우고싶다.
	// 2. SpringArmComp를 만들어서 Root에 붙이고 배치하고싶다.
	// 3. CameraComp를 만들어서 SpringArmComp에 붙이고싶다.

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Direction;

	void OnActionMove(const FInputActionValue& value);
	void OnActionLook(const FInputActionValue& value);
	void OnActionJump(const FInputActionValue& value);
	void OnActionFire(const FInputActionValue& value);

	// 총을 쏘고싶다.
	// - 총구위치
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* GunComp;
	// - 총알공장
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> BulletFactory;
	
};



