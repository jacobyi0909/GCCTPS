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
#include "TPSPlayerAnim.h"
#include "TPSPlayerFireComponent.h"
#include "TPSPlayerHPWidget.h"
#include "TPSPlayerMoveComponent.h"
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
	GunComp->SetupAttachment(GetMesh(), TEXT("hand_r"));
	GunComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGun(
		TEXT("/Script/Engine.SkeletalMesh'/Game/TPS/Models/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (tempGun.Succeeded())
	{
		GunComp->SetSkeletalMesh(tempGun.Object);
	}

	SniperComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperComp"));
	SniperComp->SetupAttachment(GetMesh(), TEXT("hand_r"));
	SniperComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper(
		TEXT("/Script/Engine.StaticMesh'/Game/TPS/Models/SniperGun/sniper11.sniper11'"));
	if (tempGun.Succeeded())
	{
		SniperComp->SetStaticMesh(tempSniper.Object);
	}

	//GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	ConstructorHelpers::FClassFinder<UUserWidget> tempHpWidget(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/TPS/UI/WBP_PlayerHP.WBP_PlayerHP_C'"));
	if (tempHpWidget.Succeeded())
	{
		HPWidgetFactory = tempHpWidget.Class;
	}

	MoveComp = CreateDefaultSubobject<UTPSPlayerMoveComponent>(TEXT("MoveComp"));
	FireComp = CreateDefaultSubobject<UTPSPlayerFireComponent>(TEXT("FireComp"));
}

void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 최대 점프 카운트를 2로 하고싶다.
	this->JumpMaxCount = 2;

	//PRINT_LOG(TEXT("%s %d"), TEXT("Hello World!"), 20);

	// 태어날 때 CrosshairUI, SniperUI를 생성하고 Viewport에 붙이고 보이지 않게 하고싶다.



	HpWidget = CreateWidget<UTPSPlayerHPWidget>(GetWorld(), HPWidgetFactory);
	HpWidget->AddToViewport();

	HP = MaxHp;
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


}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto* input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		SetupInputDelegate.Broadcast(this, input);
	}
}

void ATPSPlayer::SetHP(float newHP)
{
	CurHp = newHP;
	HpWidget->UpdateHP(CurHp, MaxHp);
}

float ATPSPlayer::GetHP() const
{
	return CurHp;
}

void ATPSPlayer::DoDamageFromEnemy(int32 damage)
{
	// 만약 체력이 0이하라면 바로 종료
	if (HP <= 0)
		return;
	
	HP--;
	if (HP <= 0)
	{
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, [&]()
		{
			UGameplayStatics::SetGamePaused(GetWorld(), true);

			// 할일 : 게임오버 화면을 출력하고싶다.
		}, 0.2f, false);
	}
}
