// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHPWidget.h"

#include "Components/ProgressBar.h"

void UEnemyHPWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// HPBar, HPBarBack
	// HPBarBack은 HPBar의 Percent값을 따라가고싶다.
	float target = HPBar->GetPercent();
	float newPercent = FMath::Lerp(HPBarBack->GetPercent(), target, InDeltaTime * 6.f);

	HPBarBack->SetPercent(newPercent);
}

void UEnemyHPWidget::UpdateHP(float CurHp, float MaxHp)
{
	if (HPBar)
		HPBar->SetPercent(CurHp / MaxHp);
}
