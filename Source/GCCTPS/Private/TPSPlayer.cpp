// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"

#include "../GCCTPS.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. Mesh의 내용을 채우고싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));

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

}

void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	PRINT_LOG(TEXT("%s %d"), TEXT("Hello World!"), 20);
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

	PlayerInputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &ATPSPlayer::OnAxisForward);

	PlayerInputComponent->BindAxis(TEXT("Move Right / Left"), this, &ATPSPlayer::OnAxisRight);

	PlayerInputComponent->BindAxis(TEXT("Look Up / Down Mouse"), this, &ATPSPlayer::OnAxisLookUp);

	PlayerInputComponent->BindAxis(TEXT("Turn Right / Left Mouse"), this, &ATPSPlayer::OnAxisTurn);

	
}


void ATPSPlayer::OnAxisRight(float value)
{
	Direction.Y = value;
}

void ATPSPlayer::OnAxisForward(float value)
{
	Direction.X = value;
}

void ATPSPlayer::OnAxisLookUp(float value)
{
	AddControllerPitchInput(value);
}

void ATPSPlayer::OnAxisTurn(float value)
{
	AddControllerYawInput(value);
}


