// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSPlayerHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class GCCTPS_API UTPSPlayerHPWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* HPBar;

	void UpdateHP(float CurHp, float MaxHp);
};
