// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerFireComponent.h"

#include "Bullet.h"
#include "Enemy.h"
#include "EnemyFSM.h"
#include "EnhancedInputComponent.h"
#include "TPSPlayer.h"
#include "TPSPlayerAnim.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTPSPlayerFireComponent::UTPSPlayerFireComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTPSPlayerFireComponent::BeginPlay()
{
	Super::BeginPlay();

	CrosshairUi = CreateWidget(GetWorld(), CrosshairUIFactory);
	CrosshairUi->AddToViewport();

	SniperUi = CreateWidget(GetWorld(), SniperUIFactory);
	SniperUi->AddToViewport();


	bChoosGun = true;
	OnActionChooseSniper(FInputActionValue());
	
}


// Called every frame
void UTPSPlayerFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// A = Lerp(A, B, t)
	Me->CameraComp->FieldOfView = FMath::Lerp(
		Me->CameraComp->FieldOfView,
		TargetFOV,
		ZoomSpeed
	);
}

void UTPSPlayerFireComponent::SetupPlayerInputComp(ATPSPlayer* player, class UEnhancedInputComponent* InputComp)
{
	Super::SetupPlayerInputComp(player, InputComp);

	InputComp->BindAction(player->IA_TPSFire, ETriggerEvent::Started, this, &UTPSPlayerFireComponent::OnActionFire);
	InputComp->BindAction(player->IA_ChooseGun, ETriggerEvent::Started, this, &UTPSPlayerFireComponent::OnActionChooseGun);
	InputComp->BindAction(player->IA_ChooseSniper, ETriggerEvent::Started, this, &UTPSPlayerFireComponent::OnActionChooseSniper);
	InputComp->BindAction(player->IA_Zoom, ETriggerEvent::Started, this, &UTPSPlayerFireComponent::OnActionZoomIn);
	InputComp->BindAction(player->IA_Zoom, ETriggerEvent::Completed, this, &UTPSPlayerFireComponent::OnActionZoomOut);
}


void UTPSPlayerFireComponent::OnActionFire(const FInputActionValue& value)
{
	// 화면을 흔들고싶다.
	check(FireCameraShake);
	if (auto camMgr = GetWorld()->GetFirstPlayerController()->PlayerCameraManager)
	{
		camMgr->StartCameraShake(FireCameraShake);

		//auto* CameraShakeInst = camMgr->StartCameraShake(FireCameraShake);
		// camMgr->StopCameraShake(CameraShakeInst);
	}

	// 총쏘기 애니메이션을 재생하고싶다.
	if (auto* anim = Cast<UTPSPlayerAnim>(Me->GetMesh()->GetAnimInstance()))
	{
		anim->Montage_Play(FireAnimMontage);
	}

	// 총쏘기 소리 재생하고싶다.
	check(FireSound);
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), FireSound);
	}

	// 총쏘기
	if (bChoosGun)
	{
		FTransform t = Me->GunComp->GetSocketTransform(TEXT("FirePoint"));
		GetWorld()->SpawnActor<ABullet>(BulletFactory, t);
	}
	else
	{
		// struct FHitResult& OutHit,
		// const FVector& Start,
		// const FVector& End,
		// ECollisionChannel TraceChannel,
		// const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam,

		FHitResult OutHit;
		FVector Start = Me->CameraComp->GetComponentLocation();
		FVector End = Start + Me->CameraComp->GetForwardVector() * 100000;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Me);

		// 카메라 위치에서 카메라 앞 방향으로 LineTrace하고싶다.
		bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params);

		// 만약 부딪혔다면
		if (bHit)
		{
			check(BulletImpactVFX);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletImpactVFX, OutHit.ImpactPoint);

			// 만약 부딪힌 물체가 적이라면
			if (AEnemy* enemy = Cast<AEnemy>(OutHit.GetActor()))
			{
				// 적의 FSM의 OnMyTakeDamage를 호출하고싶다.
				enemy->EnemyFSM->OnMyTakeDamage(1);
			}


			// 만약 부딪힌 물체가 물리가 켜져있다면
			auto* hitComp = OutHit.GetComponent();
			if (hitComp->IsSimulatingPhysics())
			{
				// 그 물체에게 힘을 가하고싶다.
				FVector traceDir = End - Start;
				FVector impulse = -OutHit.ImpactNormal + traceDir.GetSafeNormal();
				impulse.Normalize();
				impulse = impulse * hitComp->GetMass() * 1000.f;
				hitComp->AddImpulse(impulse);
			}
		}
	}
}

// 태어날 때 Sniper가 보이게 하고싶다.
// gun을 선택했을 때만 bullet이 발사되게 하고싶다.
void UTPSPlayerFireComponent::OnActionChooseGun(const FInputActionValue& value)
{
	// gun만 보이게 하고싶다.
	Me->GunComp->SetVisibility(true);
	Me->SniperComp->SetVisibility(false);
	bChoosGun = true;

	CrosshairUi->SetVisibility(ESlateVisibility::Hidden);
	SniperUi->SetVisibility(ESlateVisibility::Hidden);

	// ZoomOut을 하고 싶다.
	OnActionZoomOut(FInputActionValue());
}

void UTPSPlayerFireComponent::OnActionChooseSniper(const FInputActionValue& value)
{
	// 만약 이미 sniper라면 함수를 바로 종료하고싶다.
	if (false == bChoosGun)
		return;

	// sniper만 보이게 하고싶다.
	Me->GunComp->SetVisibility(false);
	Me->SniperComp->SetVisibility(true);
	bChoosGun = false;

	CrosshairUi->SetVisibility(ESlateVisibility::Visible);
	SniperUi->SetVisibility(ESlateVisibility::Hidden);
}

// 타이머를 이용해서 FOV가 변경될때 부드럽게 처리되도록 하고싶다.

void UTPSPlayerFireComponent::OnActionZoomIn(const FInputActionValue& value)
{
	// 만약 내가 스나를 들었을 때만 ZoomIn기능을 활성화 하고싶다.
	if (false == bChoosGun)
	{
		TargetFOV = 30.f;
		ZoomSpeed = GetWorld()->GetDeltaSeconds() * 30.f;
		CrosshairUi->SetVisibility(ESlateVisibility::Hidden);
		SniperUi->SetVisibility(ESlateVisibility::Visible);
	}
}

void UTPSPlayerFireComponent::OnActionZoomOut(const FInputActionValue& value)
{
	TargetFOV = 90.f;
	ZoomSpeed = GetWorld()->GetDeltaSeconds() * 6.f;
	if (false == bChoosGun)
	{
		CrosshairUi->SetVisibility(ESlateVisibility::Visible);
		SniperUi->SetVisibility(ESlateVisibility::Hidden);
	}
}

