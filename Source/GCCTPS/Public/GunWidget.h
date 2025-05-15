// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GunWidget.generated.h"

/**
 * 
 */
UCLASS()
class GCCTPS_API UGunWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	class UBackgroundBlur* Blur;

	UPROPERTY(meta=(BindWidget))
	class UImage* Image_Gun;

	UPROPERTY(meta=(BindWidget))
	class UImage* Image_Sniper;

	void ChooseGunImage(bool bShowSniper);

};
