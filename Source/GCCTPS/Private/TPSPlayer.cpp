// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"

#include "Bullet.h"
#include "../GCCTPS.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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

	ConstructorHelpers::FObjectFinder<UInputMappingContext> tempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/TPS/Input/IMC_TPSPlayer.IMC_TPSPlayer'"));
	if (tempIMC.Succeeded())
	{
		IMC_TPSPlayer =tempIMC.Object;
	}

	// 총 컴포넌트를 생성해서 몸에 붙이고싶다.
	GunComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunComp"));
	GunComp->SetupAttachment(GetMesh());
}

void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	//PRINT_LOG(TEXT("%s %d"), TEXT("Hello World!"), 20);

	// 최대 점프 카운트를 2로 하고싶다.
	this->JumpMaxCount = 2;
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
	FTransform t = GunComp->GetSocketTransform(TEXT("FirePoint"));
	GetWorld()->SpawnActor<ABullet>(BulletFactory, t);
}
