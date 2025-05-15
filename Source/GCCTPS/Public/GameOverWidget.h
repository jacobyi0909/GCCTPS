// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class GCCTPS_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* Button_Restart;

	UPROPERTY(meta=(BindWidget))
	class UButton* Button_Quit;

	UFUNCTION()
	void OnClickRestart();

	UFUNCTION()
	void OnClickQuit();
};
