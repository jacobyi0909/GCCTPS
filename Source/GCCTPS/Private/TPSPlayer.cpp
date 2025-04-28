// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"

#include "Bullet.h"
#include "Enemy.h"
#include "EnemyFSM.h"
#include "../GCCTPS.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. Mesh의 내용을 채우고싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(
		TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90.f), FRotator(0, -90.f, 0));
	}

	// 2. SpringArmComp를 만들어서 Root에 붙이고 배치하고싶다.
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 190.f;
	SpringArmComp->SocketOffset = FVector(0, 50, 100.f);

	// 3. CameraComp를 만들어서 SpringArmComp에 붙이고싶다.
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	bUseControllerRotationYaw = true;
	SpringArmComp->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	ConstructorHelpers::FObjectFinder<UInputMappingContext> tempIMC(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/TPS/Input/IMC_TPSPlayer.IMC_TPSPlayer'"));
	if (tempIMC.Succeeded())
	{
		IMC_TPSPlayer = tempIMC.Object;
	}

	// 총 컴포넌트를 생성해서 몸에 붙이고싶다.
	// 총의 Mesh를 로딩해서 적용하고싶다.
	GunComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunComp"));
	GunComp->SetupAttachment(GetMesh());
	GunComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGun(
		TEXT("/Script/Engine.SkeletalMesh'/Game/TPS/Models/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (tempGun.Succeeded())
	{
		GunComp->SetSkeletalMesh(tempGun.Object);
	}

	SniperComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperComp"));
	SniperComp->SetupAttachment(GetMesh());
	SniperComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper(
		TEXT("/Script/Engine.StaticMesh'/Game/TPS/Models/SniperGun/sniper11.sniper11'"));
	if (tempGun.Succeeded())
	{
		SniperComp->SetStaticMesh(tempSniper.Object);
	}
}

void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	//PRINT_LOG(TEXT("%s %d"), TEXT("Hello World!"), 20);

	// 태어날 때 CrosshairUI, SniperUI를 생성하고 Viewport에 붙이고 보이지 않게 하고싶다.

	CrosshairUi = CreateWidget(GetWorld(), CrosshairUIFactory);
	CrosshairUi->AddToViewport();

	SniperUi = CreateWidget(GetWorld(), SniperUIFactory);
	SniperUi->AddToViewport();

	// 최대 점프 카운트를 2로 하고싶다.
	this->JumpMaxCount = 2;

	bChoosGun = true;
	OnActionChooseSniper(FInputActionValue());
}

void ATPSPlayer::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// 현재 컨트롤러가 플레이컨트롤러가 맞다면
	if (auto* pc = Cast<APlayerController>(Controller))
	{
		// UEnhancedInputLocalPlayerSubsystem를 가져와서
		auto* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			pc->GetLocalPlayer());

		// AddMappingContext를 하고싶다.
		subsystem->RemoveMappingContext(IMC_TPSPlayer);
		subsystem->AddMappingContext(IMC_TPSPlayer, 0);
	}
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ControlRotation값으로 Transform을 만들고 그것을 기준으로 방향을 변경하고
	// AddMovementInput 하고싶다.
	FTransform t = FTransform(GetControlRotation());
	Direction = t.TransformVector(Direction);
	AddMovementInput(Direction);

	Direction = FVector::ZeroVector;

	// A = Lerp(A, B, t)
	CameraComp->FieldOfView = FMath::Lerp(
		CameraComp->FieldOfView,
		TargetFOV,
		ZoomSpeed
	);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto* input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		input->BindAction(IA_TPSMove, ETriggerEvent::Triggered, this, &ATPSPlayer::OnActionMove);
		input->BindAction(IA_TPSLook, ETriggerEvent::Triggered, this, &ATPSPlayer::OnActionLook);
		input->BindAction(IA_TPSJump, ETriggerEvent::Started, this, &ATPSPlayer::OnActionJump);
		input->BindAction(IA_TPSFire, ETriggerEvent::Started, this, &ATPSPlayer::OnActionFire);
		input->BindAction(IA_ChooseGun, ETriggerEvent::Started, this, &ATPSPlayer::OnActionChooseGun);
		input->BindAction(IA_ChooseSniper, ETriggerEvent::Started, this, &ATPSPlayer::OnActionChooseSniper);
		input->BindAction(IA_Zoom, ETriggerEvent::Started, this, &ATPSPlayer::OnActionZoomIn);
		input->BindAction(IA_Zoom, ETriggerEvent::Completed, this, &ATPSPlayer::OnActionZoomOut);
	}
}

void ATPSPlayer::OnActionMove(const FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	Direction.X = v.X;
	Direction.Y = v.Y;
}

void ATPSPlayer::OnActionLook(const FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	AddControllerPitchInput(v.Y);
	AddControllerYawInput(v.X);
}

void ATPSPlayer::OnActionJump(const FInputActionValue& value)
{
	Jump();
}

void ATPSPlayer::OnActionFire(const FInputActionValue& value)
{
	// 총쏘기
	if (bChoosGun)
	{
		FTransform t = GunComp->GetSocketTransform(TEXT("FirePoint"));
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
		FVector Start = CameraComp->GetComponentLocation();
		FVector End = Start + CameraComp->GetForwardVector() * 100000;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

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
void ATPSPlayer::OnActionChooseGun(const FInputActionValue& value)
{
	// gun만 보이게 하고싶다.
	GunComp->SetVisibility(true);
	SniperComp->SetVisibility(false);
	bChoosGun = true;

	CrosshairUi->SetVisibility(ESlateVisibility::Hidden);
	SniperUi->SetVisibility(ESlateVisibility::Hidden);

	// ZoomOut을 하고 싶다.
	OnActionZoomOut(FInputActionValue());
}

void ATPSPlayer::OnActionChooseSniper(const FInputActionValue& value)
{
	// 만약 이미 sniper라면 함수를 바로 종료하고싶다.
	if (false == bChoosGun)
		return;

	// sniper만 보이게 하고싶다.
	GunComp->SetVisibility(false);
	SniperComp->SetVisibility(true);
	bChoosGun = false;

	CrosshairUi->SetVisibility(ESlateVisibility::Visible);
	SniperUi->SetVisibility(ESlateVisibility::Hidden);
}

// 타이머를 이용해서 FOV가 변경될때 부드럽게 처리되도록 하고싶다.

void ATPSPlayer::OnActionZoomIn(const FInputActionValue& value)
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

void ATPSPlayer::OnActionZoomOut(const FInputActionValue& value)
{
	TargetFOV = 90.f;
	ZoomSpeed = GetWorld()->GetDeltaSeconds() * 6.f;
	if (false == bChoosGun)
	{
		CrosshairUi->SetVisibility(ESlateVisibility::Visible);
		SniperUi->SetVisibility(ESlateVisibility::Hidden);
	}
}
