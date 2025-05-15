// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class GCCTPS_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetActiveGameOver(bool bActive);
	void ChooseGunImage(bool bSniper);

	UPROPERTY(meta=(BindWidget))
	class UGameOverWidget* GameOver;

	UPROPERTY(meta=(BindWidget))
	class UGunWidget* Gun;

};

