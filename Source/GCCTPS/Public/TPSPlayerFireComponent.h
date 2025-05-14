// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSPlayerBaseComponent.h"
#include "GCCTPS/GCCTPS.h"
#include "InputActionValue.h"
#include "TPSPlayerFireComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GCCTPS_API UTPSPlayerFireComponent : public UTPSPlayerBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTPSPlayerFireComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupPlayerInputComp(class ATPSPlayer* player, class UEnhancedInputComponent* InputComp) override;

	void OnActionFire(const FInputActionValue& value);
	void OnActionChooseGun(const FInputActionValue& value);
	void OnActionChooseSniper(const FInputActionValue& value);
	void OnActionZoomIn(const FInputActionValue& value);
	void OnActionZoomOut(const FInputActionValue& value);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> BulletFactory;

	bool bChoosGun = false;

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

	// 총쏘기 애니메이션을 재생하고싶다.
	UPROPERTY(EditAnywhere)
	class UAnimMontage* FireAnimMontage;

	// 소리재생
	UPROPERTY(EditAnywhere)
	class USoundWave* FireSound;

	// 화면 흔들기
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> FireCameraShake;
};
