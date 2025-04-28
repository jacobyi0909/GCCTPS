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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Direction;

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

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_ChooseGun;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_ChooseSniper;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Zoom;
	
	void OnActionMove(const FInputActionValue& value);
	void OnActionLook(const FInputActionValue& value);
	void OnActionJump(const FInputActionValue& value);
	void OnActionFire(const FInputActionValue& value);
	void OnActionChooseGun(const FInputActionValue& value);
	void OnActionChooseSniper(const FInputActionValue& value);

	void OnActionZoomIn(const FInputActionValue& value);
	void OnActionZoomOut(const FInputActionValue& value);

	// 총을 쏘고싶다.
	// - 총구위치
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* GunComp;
	// - 총알공장
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> BulletFactory;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* SniperComp;

	bool bChoosGun = false;

	// 태어날 때 CrosshairUI, SniperUI를 생성하고 Viewport에 붙이고 보이지 않게 하고싶다.
	// 1번을 누르면 UI가 다 안보여야한다.
	// 2번을 누르면 Crosshair만 보여야한다
	//   이 상태에서 ZoomIn 하면 Sniper가 보여야 한다.
	//   ZoomOut을 하면 Crosshair만 보여야한다
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> CrosshairUIFactory;

	UPROPERTY()
	UUserWidget* CrosshairUi;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> SniperUIFactory;

	UPROPERTY()
	UUserWidget* SniperUi;

	float TargetFOV = 90.f;

	float ZoomSpeed;

	// 스나이퍼총으로 맞은 위치에 BulletImpact VFX를 표현하고싶다.
	UPROPERTY(EditAnywhere)
	class UParticleSystem* BulletImpactVFX;

	// FTimerHandle handle;
};



