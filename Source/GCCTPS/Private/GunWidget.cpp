// Fill out your copyright notice in the Description page of Project Settings.


#include "GunWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

void UGunWidget::ChooseGunImage(bool bShowSniper)
{
	auto* gunSlot = Cast<UCanvasPanelSlot>(Image_Gun->Slot);
	auto* sniperSlot = Cast<UCanvasPanelSlot>(Image_Sniper->Slot);

	gunSlot->SetZOrder(bShowSniper ? 0 : 2);
	sniperSlot->SetZOrder(bShowSniper ? 2 : 0);
}
