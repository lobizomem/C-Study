// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AtackDataAsset.h"
#include "GameFramework/Character.h"
#include "Myplayer2.generated.h"

UCLASS()
class PLAYERMODULE_API AMyplayer2 : public ACharacter
{
	GENERATED_BODY()

public:

	

	
	// Sets default values for this character's properties
	AMyplayer2();

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------


// Variaveis //

#pragma region Variaveis

	// Define se pode ataca para nao ficar flicando
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Variaveis C++")
	bool PodeAtacar = true;

	// Define se o jogador quer continuar o combo
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Variaveis C++")
	int32 NClicks;

	// Define em qual ataque do combo esta
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Variaveis C++")
	int32 NAtaque;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Variaveis C++")
	FTimerHandle Timer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Variaveis C++")
	UAtackDataAsset* DataAssetRef;

#pragma	endregion



//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------


// Funções //
#pragma region Funções

	// Função que vai iniciar o combo
	UFUNCTION(BlueprintCallable, Category = "Funções C++")
	virtual void Combo();

	UFUNCTION(BlueprintCallable, Category = "Funções C++")
	virtual void ComboResete();
	
#pragma  endregion 




//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
	
#pragma region Padrão
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma endregion
	
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
};
