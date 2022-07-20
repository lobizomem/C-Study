// Fill out your copyright notice in the Description page of Project Settings.


#include "Myplayer2.h"

// Sets default values
AMyplayer2::AMyplayer2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

//Região Padrão 
#pragma  region  Padrão

// Called when the game starts or when spawned
void AMyplayer2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyplayer2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyplayer2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

#pragma endregion 

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------



//Inicia o combo //
void AMyplayer2::Combo()
{
	NClicks = NClicks +1;

	
	if (PodeAtacar)
	{
		switch (NAtaque)
		{
		case 0:

			PodeAtacar = false;
			PlayAnimMontage(DataAssetRef->AtackMontage1, DataAssetRef->SpeedAtack1);
			GetWorld()->GetTimerManager().SetTimer(Timer, [this](){ComboResete();}, DataAssetRef->TimeAtack1, 1);
		
			break;

		case 1:

			PodeAtacar = false;
			PlayAnimMontage(DataAssetRef->AtackMontage2, DataAssetRef->SpeedAtack2);
			GetWorld()->GetTimerManager().SetTimer(Timer, [this](){ComboResete();}, DataAssetRef->TimeAtack2, 1);
			
			break;

		case 2:

			PodeAtacar = false;
			PlayAnimMontage(DataAssetRef->AtackMontage3, DataAssetRef->SpeedAtack3);
			GetWorld()->GetTimerManager().SetTimer(Timer, [this](){ComboResete();}, DataAssetRef->TimeAtack3, 1);
			
			break;

		case 3:

			PodeAtacar = false;
			PlayAnimMontage(DataAssetRef->AtackMontage4, DataAssetRef->SpeedAtack4);
			GetWorld()->GetTimerManager().SetTimer(Timer, [this](){ComboResete();}, DataAssetRef->TimeAtack4, 1);
			
			break;
			
		default: break;
		}
	
	}
	
	
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

// Reseta o combo //
void AMyplayer2::ComboResete()
{

	PodeAtacar = true;
	if (NAtaque < 3)
	{
		NAtaque = NAtaque + 1; 	
	}
	else
	{
		NClicks = 0;
		NAtaque = 0;
		GetWorld()->GetTimerManager().ClearTimer(Timer);
	}
	if (NClicks <= NAtaque)
	{
		NClicks = 0;
		NAtaque = 0;
		GetWorld()->GetTimerManager().ClearTimer(Timer);
	}
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
