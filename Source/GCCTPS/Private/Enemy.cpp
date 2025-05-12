// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemyFSM.h"
#include "EnemyHPWidget.h"
#include "NavigationInvokerComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(
	TEXT("/Script/Engine.SkeletalMesh'/Game/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"));

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90.f), FRotator(0, -90.f, 0));
	}
	
	EnemyFSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("EnemyFSM"));


	NavInvokerComp = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvokerComp"));
	NavInvokerComp->SetGenerationRadii(1000.f, 2000.f);
	
	HpWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpWidgetComp"));
	HpWidgetComp->SetupAttachment(RootComponent);
	HpWidgetComp->SetRelativeLocation(FVector(0, 0, 150.f));

	ConstructorHelpers::FClassFinder<UUserWidget> tempHpBar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/TPS/UI/WBP_EnemyHP.WBP_EnemyHP_C'"));
	if (tempHpBar.Succeeded())
	{
		HpWidgetComp->SetWidgetClass(tempHpBar.Class);
		HpWidgetComp->SetDrawSize(FVector2d(100.0, 20.0));
	}
		
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

