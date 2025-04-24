// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	SetRootComponent(CollisionComp);
	// 충돌체를 BlockAll로 하고싶다.
	CollisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	CollisionComp->SetSphereRadius(8.f);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetRelativeScale3D(FVector(0.15f));

	// 외형은 충돌체를 끄고싶다.
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	// 이동 속도와 바운스 설정을 하고싶다.
	MovementComp->InitialSpeed = 2000.f;
	MovementComp->MaxSpeed = 2000.f;
	MovementComp->bShouldBounce = true;
	// 실제로 이동할 컴포넌트를 지정하고싶다.
	MovementComp->SetUpdatedComponent(RootComponent);

	//InitialLifeSpan = 10.f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	// 타이머를 이용해서 10초 후에 파괴되게 하고싶다.
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle,
		[&]()
		{
			this->Destroy();
		},
		MyLifeSpan, false);
	
	//GetWorld()->GetTimerManager().SetTimer(handle, this, &ABullet::MyDestroy, MyLifeSpan, false);
	// Lambda 식, 익명함수(Anonymous function)
	// auto Add = [&](int a, int b)->int
	// {
	// 	return a + b;
	// };
	//
	// int result = Add(100, 200);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABullet::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	// 여러가지 프로퍼티 중에 어떤 프로퍼티의 값이 변경되었다면 감지할 수 있다.
	// 만약 BulletSpeed의 값이 바뀌었다면
	if (PropertyChangedEvent.GetPropertyName() == TEXT("BulletSpeed"))
	{
		// MovementComp의 Speed를 BulletSpeed값으로 대입하고싶다.
		MovementComp->InitialSpeed = BulletSpeed;
		MovementComp->MaxSpeed = BulletSpeed;
	}
}
