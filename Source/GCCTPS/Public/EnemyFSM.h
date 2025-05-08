// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA(DisplayName = "대기상태"),
	Move UMETA(DisplayName = "이동상태"),
	Attack UMETA(DisplayName = "공격상태"),
	Damage UMETA(DisplayName = "데미지상태"),
	Die UMETA(DisplayName = "죽음상태"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GCCTPS_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	EEnemyState State;

	UPROPERTY()
	class ATPSPlayer* Player;

	UPROPERTY()
	class AEnemy* Me;

	UPROPERTY()
	class UEnemyAnim* EnemyAnim;

	// 공격가능거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 200.f;

	// 공격대기시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDelayTime = 1.5f;

	float CurrentTime;
	
	
	void TickIdle();
	void TickMove();
	void TickAttack();
	void TickDamage();
	void TickDie();

	// 플레이어가 나를 공격하면 데미지처리를 하고싶다.
	// - 체력
	// - 공격받은 기능
	void OnMyTakeDamage(int32 damage);

	float CurHp = 2.f;
	float MaxHp = 2.f;

	void SetState(EEnemyState next);

	float DieDownSpeed = 200.f;
};
