// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// 선언
DECLARE_LOG_CATEGORY_EXTERN(TPSLog, Log, All)

#define CALL_INFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")")) 
#define PRINT_CALL_INFO() UE_LOG(TPSLog, Warning, TEXT("%s"), *CALL_INFO) 
#define PRINT_LOG(fmt, ...) UE_LOG(TPSLog, Warning, TEXT("%s %s"), *CALL_INFO, *FString::Printf(fmt, ##__VA_ARGS__))

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	Walking UMETA(DisplayName = "걷기"),
	Running UMETA(DisplayName = "뛰기"),
	Crouching UMETA(DisplayName = "오리걸음")
};