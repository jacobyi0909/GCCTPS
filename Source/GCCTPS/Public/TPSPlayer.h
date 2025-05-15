// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "GCCTPS/GCCTPS.h"
#include "TPSPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FSetupInputDelegate,
	class ATPSPlayer* , class UEnhancedInputComponent*);

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

	// 델리게이트를 이용해서 컴포넌트들의 입력처리를 간소화 하고싶다.

	FSetupInputDelegate SetupInputDelegate;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
	class UTPSPlayerFireComponent* FireComp;

	UPROPERTY(EditAnywhere)
	class UTPSPlayerMoveComponent* MoveComp;
	
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

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Run;
	
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Crouch;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* GunComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* SniperComp;

	
	// 체력
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UTPSPlayerHPWidget> HPWidgetFactory;

	UPROPERTY()
	class UTPSPlayerHPWidget* HpWidget;

	float CurHp = 2.f;
	float MaxHp = 2.f;

	__declspec(property(get = GetHP, put = SetHP))
	float HP;

	void SetHP(float newHP);
	
	float GetHP() const;

	void DoDamageFromEnemy(int32 damage);


	// 메인위젯을 생성해서 화면에 보이게 하고싶다.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainWidget> MainWidgetFactory;

	UPROPERTY()
	class UMainWidget* MainWidget;
};




