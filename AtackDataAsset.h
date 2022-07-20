// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ActorDamageClass.h"
#include "Engine/DataAsset.h"
#include "AtackDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERMODULE_API UAtackDataAsset : public UDataAsset
{
	GENERATED_BODY()

	public:

#pragma region Variables

	//Montage de animação 1
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
	UAnimMontage * AtackMontage1;

	//Montage de animação 2
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
	UAnimMontage * AtackMontage2;

	//Montage de animação 3
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
	UAnimMontage * AtackMontage3;

	//Montage de animação 4
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
	UAnimMontage * AtackMontage4;
	
	//Velocidade da Animação 1
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
    float SpeedAtack1 = 1;
    	
    //Velocidade da  Animação 2
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
    float SpeedAtack2 = 1;
        	
    //Velocidade da Animação 3
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
    float SpeedAtack3 = 1;

	//Velocidade da Animação 4
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
	float SpeedAtack4 = 1;

	//Tempo que demora a Animação1
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
	float TimeAtack1;

	//Tempo que demora a Animação2
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
	float TimeAtack2;

	//Tempo que demora a Animação3
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
	float TimeAtack3;

	//Tempo que demora a Animação4
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
	float TimeAtack4;

	//BP de dano
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
	TSubclassOf<AActorDamageClass> ActorDamage;

	// HP do Player
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
	float Health;

	//Max HP do Player
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
	float MaxHealth;

	//Força do player
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
	float Atk;

	//Defesa do Player
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables")
	float Def;

	
#pragma  endregion

	
};
