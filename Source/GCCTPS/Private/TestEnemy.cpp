// Fill out your copyright notice in the Description page of Project Settings.


#include "TestEnemy.h"

// Sets default values
ATestEnemy::ATestEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ATestEnemy::GetFovAngle(FVector forward, FVector targetDir)
{
	float dot = FVector::DotProduct(forward.GetSafeNormal2D(), targetDir.GetSafeNormal2D());

	float degree = FMath::RadiansToDegrees(FMath::Acos(dot));

	return degree;
}

