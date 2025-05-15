// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"

#include "GameOverWidget.h"
#include "GunWidget.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetActiveGameOver(false);
}

void UMainWidget::SetActiveGameOver(bool bActive)
{
	GameOver->SetVisibility(bActive ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UMainWidget::ChooseGunImage(bool bSniper)
{
	Gun->ChooseGunImage(bSniper);
}
