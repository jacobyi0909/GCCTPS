// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class GCCTPS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 충돌체와 외관을 만들고싶다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USphereComponent> CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UProjectileMovementComponent> MovementComp;
};
