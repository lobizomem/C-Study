// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ActorDamageClass.h"
#include "FPSEnunsAndStructs.h"
//#include "FPSGameMode.h"
#include "FPSPlayerController.h"
#include "ProjectileClass.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"

#include "PlayerFPS.generated.h"


class AFPSGameMode;


UCLASS()
class PLAYERMODULE_API APlayerFPS : public ACharacter
{

	GENERATED_BODY()

public:



#pragma region PragmaCopy


#pragma endregion

	
//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------


// Variables //
#pragma region Variables

	/** HeadShotSound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables c++")
	TSubclassOf<AActor> HeadShotSound;
	
	/** DamageParticle */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables c++")
    UNiagaraSystem* DamageParticle;
	
	/** Time a qual pertence */
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category="Variables c++")
	int32 Time;

	/** Indice do KDInfo */
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category="Variables c++")
	int32 Indice;

	/** Referencia ao player controller */
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category="Variables c++")
	AFPSPlayerController* PlayerControllerRef;

	/** Infos do time 1 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Variables c++")
	AFPSGameMode* GameModeRef;

	/** Infos do time 1 */
	UPROPERTY(EditDefaultsOnly,Replicated, BlueprintReadWrite, Category="Variables c++")
	TArray<FKDInfo> InfoTime1;

	/** Infos do time 2 */
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category="Variables c++")
	TArray<FKDInfo> InfoTime2;
	
	/** Tempo de jogo em Minutos */
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category="Variables c++")
	float TempoDeJogoPlayer = 10;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category="Variables c++")
	FKDInfo PlayerInfor;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category="Variables c++")
	TArray<FKDInfo> PartidaInfor;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category="Variables c++")
	float Health = 100;
	
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category="Variables c++")
	UAnimMontage* RecarregarAnimMontage;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category="Variables c++")
	FVector ActorLocation;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category="Variables c++")
	FRotator ActorRotation;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category="Variables c++")
	UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category="Variables c++")
	UAnimMontage* DeadMontage;
	
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category="Variables c++")
	int32 Municao = 30;

	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadWrite, Category="Variables c++")
	float Damage = 10;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_RegarregandoAnim, BlueprintReadWrite, Category="Variables c++")
	bool Recarregando = false;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_RegarregandoAnim, BlueprintReadWrite, Category="Variables c++")
	bool Morto = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Variables")
	FTimerHandle RechargeDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Variables")
	FTimerHandle TempoDeJogoTimer;

	/** Timer para reviver*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Variables")
	FTimerHandle ReviveTimer;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_FireAnimation, BlueprintReadWrite, Category="Variables c++" )
	bool Atirando = false;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_FireAnimation, BlueprintReadWrite, Category="Variables c++" )
	bool bDebugTrace = true;
	
	
#pragma endregion

	
//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------


// default //
#pragma region Default

	
	// Sets default values for this character's properties
	APlayerFPS();

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
#pragma endregion



//-----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------


// Functions //
#pragma region Functions

	//OnRep Animation
	UFUNCTION()
	virtual void OnRep_FireAnimation();

	//OnRep Animation
	UFUNCTION()
	virtual void OnRep_RegarregandoAnim();
	
	//Função para a Animação de tiro
	UFUNCTION(BlueprintCallable, Category=" c++ Functions")
	void Client_Recarregndo();

	//Função para a Animação de tiro
	UFUNCTION(BlueprintCallable, Category=" c++ Functions")
	void FireClient(FTransform TransformSpawnRef, TSubclassOf<AProjectileClass>  ProjectileRef );

	//Função para a Animação de tiro
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, BlueprintAuthorityOnly, Category=" c++ Functions")
	void FireAnimation();
	void FireAnimation_Implementation();
	bool FireAnimation_Validate();
	
	
	//Função Spawn Projectile
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation,BlueprintAuthorityOnly, Category=" c++ Functions")
	void SpawnProjectile( FTransform TransformSpawn, TSubclassOf<AProjectileClass>  Projectile);
	void SpawnProjectile_Implementation(FTransform TransformSpawn, TSubclassOf<AProjectileClass>  Projectile);
	bool SpawnProjectile_Validate(FTransform TransformSpawn, TSubclassOf<AProjectileClass>  Projectile);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation,BlueprintAuthorityOnly, Category=" c++ Functions")
	void DecrementMunicao();
	void DecrementMunicao_Implementation();
	bool DecrementMunicao_Validate();
	

	UFUNCTION(BlueprintCallable,Server, Reliable, BlueprintAuthorityOnly, Category=" c++ Functions")
	void IncrementMunicao();
	void IncrementMunicao_Implementation();
	bool IncrementMunicao_Validate();

	// Apply Damage
	UFUNCTION(BlueprintCallable, Category=" c++ Functions")
	void Client_ApplyDamageFunction( float DamageFire,AActor* OtherActor );
	
	// Apply Damage
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, BlueprintAuthorityOnly, Category=" c++ Functions")
	void ApplyDamageFunction( float DamageFire,AActor* OtherActor );
	void ApplyDamageFunction_Implementation(float DamageFire, AActor* OtherActor);
	bool ApplyDamageFunction_Validate(float DamageFire, AActor* OtherActor);

	/** line Trace */
	UFUNCTION(BlueprintCallable,Server, Reliable, BlueprintAuthorityOnly, Category=" c++ Functions")
	void LineTraceF(FVector StartPoint, FVector EndPoint);
	void LineTraceF_Implementation(FVector StartPoint, FVector EndPoint);
	bool LineTraceF_Validate(FVector StartPoint, FVector EndPoint);

	/** LineTrace do lado do client */
	UFUNCTION(BlueprintCallable, Category=" c++ Functions")
	void Client_LineTraceF( FVector StartPoint, FVector EndPoint );

	/** Animação de Recarregar */
	UFUNCTION(BlueprintCallable,Server, Reliable, BlueprintAuthorityOnly, Category=" c++ Functions")
	void RecarregarAnim();
	void RecarregarAnim_Implementation();
	bool RecarregarAnim_Validate();

	/** Recarregar Server*/
	UFUNCTION(BlueprintCallable,Server, Reliable, BlueprintAuthorityOnly, Category=" c++ Functions")
	void Server_Recarregar();
	void Server_Recarregar_Implementation();
	bool Server_Recarregar_Validate();

	/** AnyDamage Server*/
	UFUNCTION(BlueprintCallable, Category=" c++ Functions")
	void AnyDamage(AActor* DamagedActor, float DamageHit, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	//void AnyDamage_Implementation(AActor* DamagedActor, float DamageHit, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	bool AnyDamage_Validate(AActor* DamagedActor, float DamageHit, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	/** AnyDamage Server*/
	UFUNCTION(BlueprintCallable, Category=" c++ Functions")
	void ClientAnyDamage(AActor* DamagedActor, float DamageHit, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	/** Time do tempo de jogo */
	UFUNCTION(BlueprintCallable, Category=" c++ Functions")
	void TempoDeJogoPlayerFunction();

	/** Time do tempo de jogo */
	UFUNCTION(BlueprintCallable, Client,Reliable, Category=" c++ Functions")
	virtual void ClientDisableInput();

	/** Informa o game mode inicialmente */
	UFUNCTION(BlueprintCallable,Server,Reliable, Category=" c++ Functions")
	virtual void InicialInfoGameMode();

	/** Informa o game mode inicialmente */
	UFUNCTION(BlueprintCallable,Server,Reliable, Category=" c++ Functions")
	virtual void Reviver();

	/** Informa o game mode inicialmente */
	UFUNCTION(BlueprintCallable, Client,Reliable, Category=" c++ Functions")
	virtual void ClientReviver();


	/** Informa o game mode inicialmente */
	UFUNCTION(BlueprintCallable,Server,Reliable, Category=" c++ Functions")
	virtual void AttTimesInfo();

	/** Informa o game mode inicialmente */
	UFUNCTION(BlueprintCallable,Client,Reliable, Category=" c++ Functions")
	virtual void ClientGameMode(AFPSGameMode* ClientGameModeRef);

	/** Informa o game mode inicialmente */
	UFUNCTION(BlueprintCallable,Server,Reliable, Category=" c++ Functions")
	virtual void ServerGameMode();

	/** Informa o game mode inicialmente */
	UFUNCTION(BlueprintCallable,Server,Reliable, Category=" c++ Functions")
	virtual void SepararTime();

	/** Informa o game mode inicialmente */
	UFUNCTION(BlueprintCallable,Server,Reliable, Category=" c++ Functions")
	virtual void AttInfoGameMOde(FKDInfo PlayerInforF);

	/** Informa o game mode inicialmente */
	UFUNCTION(BlueprintCallable,Server,Reliable, Category=" c++ Functions")
	virtual void GameModePronto();

	/** Informa o game mode inicialmente */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category=" c++ Functions")
	void SpawnDamageEffect(FVector HitLocation, AActor* OtherActor);
	
	
	
#pragma endregion



};






