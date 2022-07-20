// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFPS.h"
#include "FPSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameMode.h"
#include "NiagaraFunctionLibrary.h"
#include "Net/UnrealNetwork.h"


class AFPSGameMode;

//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------


// default //
#pragma region Default

// Sets default values
APlayerFPS::APlayerFPS()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerFPS::BeginPlay()
{
	Super::BeginPlay();

	
	/** Ativa o evento de dano*/
	OnTakeAnyDamage.AddDynamic(this, &APlayerFPS::ClientAnyDamage);
	/** Ativa o time da partida*/
	GetWorld()->GetTimerManager().SetTimer ( TempoDeJogoTimer, this, &APlayerFPS::TempoDeJogoPlayerFunction , 1 , true);
	/** Pega referencia do game mode */
	ServerGameMode();
	/**Informa a referencia do player */
	PlayerInfor.PlayerRef = this;
	PlayerInfor.bPronto = false;
	/** Pega referencia do playerControler*/
	PlayerControllerRef = Cast<AFPSPlayerController>(this->Controller);

	
	
			
}


// Called every frame
void APlayerFPS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



// Called to bind functionality to input
void APlayerFPS::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
#pragma endregion



//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------


 // OnRep AnimMontage //
#pragma region OnRep AnimMontage
void APlayerFPS::OnRep_FireAnimation()
{
	if (Municao > 0 )
	{
		PlayAnimMontage(FireMontage,1);
	}
	
	
}

void APlayerFPS::OnRep_RegarregandoAnim()
{
	if (Municao <= 0 )
	{
		PlayAnimMontage(RecarregarAnimMontage,1);
	}
}


#pragma endregion


//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------


// Client Fire //
#pragma region  Client Fire


void APlayerFPS::FireClient(FTransform TransformSpawnRef, TSubclassOf<AProjectileClass> ProjectileRef)
{
	
	if (Municao > 0)
	{
		FireAnimation();
		SpawnProjectile(TransformSpawnRef,ProjectileRef);
		DecrementMunicao();
		return;
	}
	if (Municao <= 0 && Recarregando == false)
	{
		
		Recarregando = true;
		//GetWorld()->GetTimerManager().SetTimer(RechargeDelay, this, &APlayerFPS::Client_Recarregndo, 2, 0);
		RecarregarAnim();
	}
	
}



#pragma endregion


//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------


// Animation functions//
#pragma region AnimMontage

void APlayerFPS::FireAnimation_Implementation()
{
	PlayAnimMontage(FireMontage,1);
}



//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------


bool APlayerFPS::FireAnimation_Validate()
{
	return true;
}


#pragma endregion




//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------



// Spawn Projectile //
#pragma region projectile



void APlayerFPS::SpawnProjectile_Implementation(FTransform TransformSpawn, TSubclassOf<AProjectileClass>  Projectile)
{
	
	
	if (GetOwner() != nullptr && GetOwner()->GetLocalRole() == ROLE_Authority)
	{
	
		Atirando = !Atirando;
		FireAnimation();
		ActorLocation = TransformSpawn.GetLocation();
		ActorRotation = TransformSpawn.Rotator();
		AProjectileClass* NewProjectile = GetWorld()->SpawnActor<AProjectileClass>(Projectile, ActorLocation, ActorRotation);
		NewProjectile->SetOwner(GetOwner());
		
	}
}


//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

bool APlayerFPS::SpawnProjectile_Validate(FTransform TransformSpawn, TSubclassOf<AProjectileClass>  Projectile)
{
	return true;
}


#pragma endregion



//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------


#pragma region  Decrement Munition



void APlayerFPS::DecrementMunicao_Implementation()
{
	
	if (GetOwner() != nullptr && GetOwner()->GetLocalRole() == ROLE_Authority)
	{
	
		Municao --;
		
	}
	
}




//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------



bool APlayerFPS::DecrementMunicao_Validate()
{
	return true;
}


#pragma endregion 



//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------

/** Apply Damage*/
#pragma region Apply Damage

void APlayerFPS::Client_ApplyDamageFunction(float DamageFire, AActor* OtherActor)
{
	ApplyDamageFunction(DamageFire,OtherActor);
}


//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------


void APlayerFPS::ApplyDamageFunction_Implementation(float DamageFire, AActor* OtherActor)
{
	if (GetOwner() != nullptr && GetOwner()->GetLocalRole() == ROLE_Authority && GetOwner() != OtherActor)
	{
		APlayerFPS* DamageActor = Cast<APlayerFPS>(OtherActor);
		if (DamageActor->Time != Time)
		{
			UGameplayStatics::ApplyDamage(OtherActor,DamageFire,GetInstigatorController(),this,UDamageType::StaticClass());
	
		}
	}
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

bool APlayerFPS::ApplyDamageFunction_Validate(float DamageFire, AActor* OtherActor)
{
	return true;
}

#pragma endregion 

//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------


/** line Trace*/
#pragma region LineTrace

void APlayerFPS::LineTraceF_Implementation(FVector StartPoint, FVector EndPoint)
{
	
		bool boolbHasHit;		
		FHitResult ResultTrace;
		TArray <AActor*> IgnoredActors;
		IgnoredActors.Add(this);
		FireAnimation();
		Atirando = !Atirando;
	
		boolbHasHit=UKismetSystemLibrary::LineTraceSingle(GetOwner(),StartPoint,EndPoint,UEngineTypes::ConvertToTraceType(ECC_Visibility),true,IgnoredActors,bDebugTrace? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,ResultTrace,true,FLinearColor::Red,FLinearColor::Green,5);

		if (ResultTrace.GetComponent()->ComponentTags.Contains(TEXT("Head")))
		{
			//
			if (boolbHasHit && Cast<ACharacter>(ResultTrace.GetActor()))
			{
				APlayerFPS* OtherCharacter = Cast<APlayerFPS>(ResultTrace.GetActor());
				if (OtherCharacter->Health - (Damage*3) <=0)
				{
					AActor* HeadShotActor = GetWorld()->SpawnActor<AActor>(HeadShotSound, GetActorLocation(), GetActorRotation());
				}
				
				ApplyDamageFunction(Damage*3, OtherCharacter);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message 9999999999999!"));
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DamageParticle,ResultTrace.Location,ResultTrace.GetActor()->GetActorRotation());
				SpawnDamageEffect(ResultTrace.Location,ResultTrace.GetActor());
				
				return;
			}
			
		
		}
	
		if (boolbHasHit && Cast<ACharacter>(ResultTrace.GetActor()))
		{
		
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug"));	
			if (ResultTrace.GetComponent()->GetCollisionObjectType() == ECC_GameTraceChannel1)
			{
				
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message 9999999999999!"));	
			}
			ACharacter* OtherCharacter = Cast<ACharacter>(ResultTrace.GetActor());
			ApplyDamageFunction(Damage, OtherCharacter);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DamageParticle,ResultTrace.Location,ResultTrace.GetActor()->GetActorRotation());
			SpawnDamageEffect(ResultTrace.Location,ResultTrace.GetActor());
		}
	
		
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

bool APlayerFPS::LineTraceF_Validate(FVector StartPoint, FVector EndPoint)
{
	return true;
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

void APlayerFPS::Client_LineTraceF(FVector StartPoint, FVector EndPoint)
{
	if (Municao > 0 && Health > 0)
	{
		
		LineTraceF(StartPoint,EndPoint);
		DecrementMunicao();
		FireAnimation();
		Atirando = !Atirando;
		
		if (GetOwner()->GetLocalRole() == ROLE_Authority)
		{
			Atirando = !Atirando;
		}
		if (GetOwner()->GetLocalRole() != ROLE_Authority)
		{
			Atirando = !Atirando;
		}

		
		return;
	}
	if (Municao == 0)
	{
		Municao = -1;
		Server_Recarregar();
		RecarregarAnim();
		Recarregando = !Recarregando;
		
		if (GetOwner()->GetLocalRole() == ROLE_Authority)
		{
			Recarregando = !Recarregando;
		}
		if (GetOwner()->GetLocalRole() != ROLE_Authority)
		{
			Recarregando = !Recarregando;
		}	
	}
	
	
}

#pragma endregion 


//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------


/** Recarregando */
#pragma region Recarregando

void APlayerFPS::Client_Recarregndo()
{

	Municao = -1;
	Server_Recarregar();
	RecarregarAnim();
	Recarregando = !Recarregando;
		
	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		Recarregando = !Recarregando;
	}
	if (GetOwner()->GetLocalRole() != ROLE_Authority)
	{
		Recarregando = !Recarregando;
	}	
	
}


//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------


void APlayerFPS::IncrementMunicao_Implementation()
{
		
	if (Municao <= 0 && GetOwner() != nullptr && GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		Municao = 30;	
	}
	
}


//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------


bool APlayerFPS::IncrementMunicao_Validate()
{
	return true;
}


void APlayerFPS::RecarregarAnim_Implementation()
{
	PlayAnimMontage(RecarregarAnimMontage, 1);
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

bool APlayerFPS::RecarregarAnim_Validate()
{
	return  true;
}

void APlayerFPS::Server_Recarregar_Implementation()
{
	Municao = -1;
	Recarregando = !Recarregando;
	RecarregarAnim();	
			
}

bool APlayerFPS::Server_Recarregar_Validate()
{
	return true;
}


#pragma endregion 


//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------


/** AnyDamage */
#pragma region AnyDamage

void APlayerFPS::AnyDamage(AActor* DamagedActor, float DamageHit, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (Health > 0)
		{
			Health -= DamageHit;
			
			if (Health <= 0)
			{
				AnyDamage(DamagedActor, DamageHit, DamageType, InstigatedBy, DamageCauser);
			}
		}
		else
		{
			APlayerFPS* CurrentPlayer =  Cast<APlayerFPS>(DamageCauser);
			int32 CurrentIndice = CurrentPlayer->Indice;
			
			
			if (Time == 1 && Morto == false && GameModeRef!= nullptr)
			{
				
				GameModeRef->InfoTime1[Indice].Death += 1;
				GameModeRef->InfoTime2[CurrentIndice].Kills += 1;
				
			
			}
			if (Time == 2 && Morto == false && GameModeRef!= nullptr)
			{
				GameModeRef = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
				GameModeRef->InfoTime2[Indice].Death += 1;
				GameModeRef->InfoTime1[CurrentIndice].Kills += 1;
			}
			if (this->Controller != nullptr)
			{
				DisableInput(PlayerControllerRef);
				ClientDisableInput();
				PlayAnimMontage(DeadMontage,1);
				/** Inicia o reviver */
				GetWorld()->GetTimerManager().SetTimer ( ReviveTimer, this, &APlayerFPS::ClientReviver, 2 , true);
				Morto = true;
			}
		
		}
	}
	
}

bool APlayerFPS::AnyDamage_Validate(AActor* DamagedActor, float DamageHit, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	return true;
}

void APlayerFPS::ClientAnyDamage(AActor* DamagedActor, float DamageHit, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	AnyDamage(DamagedActor, DamageHit, DamageType, InstigatedBy, DamageCauser);
}

#pragma endregion


//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------



/** GameMode */
#pragma region  GameMode

void APlayerFPS::TempoDeJogoPlayerFunction()
{
	
	if (GameModeRef!= nullptr)
	{
		TempoDeJogoPlayer = GameModeRef->TempoDeJogo;
	}
}

void APlayerFPS::SpawnDamageEffect_Implementation(FVector HitLocation, AActor* OtherActor)
{
}

void APlayerFPS::SepararTime_Implementation()
{
	PlayerInfor.PlayerRef = this;
	GameModeRef->SepararTime(PlayerInfor);
}

void APlayerFPS::GameModePronto_Implementation()
{
	PlayerInfor.bPronto = !PlayerInfor.bPronto;
}

void APlayerFPS::AttInfoGameMOde_Implementation(FKDInfo PlayerInforF)
{
	if (Time == 1)
	{
		GameModeRef->InfoTime1[Indice].bPronto = PlayerInforF.bPronto;	
	}
	if (Time == 2)
	{
		GameModeRef->InfoTime2[Indice].bPronto = PlayerInforF.bPronto;	
	}
	
}



void APlayerFPS::ServerGameMode_Implementation()
{
	GameModeRef = GetWorld()->GetAuthGameMode<AFPSGameMode>();
	ClientGameMode(GameModeRef);
}

void APlayerFPS::ClientGameMode_Implementation(AFPSGameMode* ClientGameModeRef)
{	
	GameModeRef = ClientGameModeRef;
}

void APlayerFPS::InicialInfoGameMode_Implementation()
{
	
	GameModeRef = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GameModeRef != nullptr)
	{
		GameModeRef->SepararTime(PlayerInfor);
	}

}

void APlayerFPS::AttTimesInfo_Implementation()
{
	if (GameModeRef != nullptr)
	{	
		InfoTime1 = GameModeRef->InfoTime1;
		InfoTime2 = GameModeRef->InfoTime2;
	}
	
}


#pragma endregion



//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------


/** Reviver */
#pragma region Reviver

void APlayerFPS::ClientReviver_Implementation()
{
	EnableInput(PlayerControllerRef);
	Health = 100;
	SetActorLocation(PlayerInfor.SpawnLocation);
	Morto = false;
	Reviver();
	GetWorld()->GetTimerManager().ClearTimer (ReviveTimer);
}

void APlayerFPS::Reviver_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer (ReviveTimer);
	EnableInput(PlayerControllerRef);
	Health = 100;
	Morto = false;
	SetActorLocation(PlayerInfor.SpawnLocation);
}

#pragma endregion 



//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------



/** Input Disable Enable*/
#pragma region Input


void APlayerFPS::ClientDisableInput_Implementation()
{
	PlayerControllerRef = Cast<AFPSPlayerController>(GetController());		
	DisableInput(PlayerControllerRef);
}


#pragma endregion



//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region Replicação das variaveis

void APlayerFPS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	
	//Replicated
	DOREPLIFETIME( APlayerFPS, ActorLocation);
	DOREPLIFETIME( APlayerFPS, ActorRotation);
	DOREPLIFETIME( APlayerFPS, FireMontage);
	DOREPLIFETIME( APlayerFPS, Municao);
	DOREPLIFETIME( APlayerFPS, Atirando);
	DOREPLIFETIME( APlayerFPS, Recarregando);
	DOREPLIFETIME( APlayerFPS, RecarregarAnimMontage);
	DOREPLIFETIME( APlayerFPS, Health);
	DOREPLIFETIME( APlayerFPS, TempoDeJogoPlayer);
	DOREPLIFETIME( APlayerFPS, PlayerControllerRef);
	DOREPLIFETIME( APlayerFPS, DeadMontage);
	DOREPLIFETIME( APlayerFPS, PartidaInfor);
	DOREPLIFETIME( APlayerFPS, PlayerInfor);
	DOREPLIFETIME( APlayerFPS, Time);
	DOREPLIFETIME( APlayerFPS, Indice);
	DOREPLIFETIME( APlayerFPS, InfoTime1);
	DOREPLIFETIME( APlayerFPS, InfoTime2);
	DOREPLIFETIME( APlayerFPS, Morto);

	

	//Condition RepNotify
	//DOREPLIFETIME_CONDITION( UUInventoryComponent, Nome da Variavel, COND_SkipReplay );
}

#pragma endregion
