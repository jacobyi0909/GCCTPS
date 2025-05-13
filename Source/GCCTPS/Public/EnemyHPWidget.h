// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class GCCTPS_API UEnemyHPWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* HPBarBack;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* HPBar;
	
	void UpdateHP(float CurHp, float MaxHp);
};
