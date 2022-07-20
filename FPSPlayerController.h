// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	
	/** Disable Input */
	UFUNCTION(BlueprintCallable, Category=" c++ Functions")
	void DisableInputF();
	
};
