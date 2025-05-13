// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerHPWidget.h"

#include "Components/ProgressBar.h"

void UTPSPlayerHPWidget::UpdateHP(float CurHp, float MaxHp)
{
	HPBar->SetPercent(CurHp / MaxHp);
}
