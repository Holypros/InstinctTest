// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterInterface.h"
#include "BallCharacter.generated.h"

UCLASS()
class INSTINCTTEST_API ABallCharacter : public APawn, public ICharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABallCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ball") //exposing to the editor
		class USceneComponent* SceneComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ball")
		class UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ball")
		class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ball")
		class UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ball")
		float ballSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ball")
		float health = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ball")
		UParticleSystem* P_Explosion;
	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
		class UCapsuleComponent* TriggerCapsule;


	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;
	


public:	


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveForward(float value);
	void MoveRight(float value);
	void TurnFunction(float value);
	void LookUpFunction(float value);

	bool IsEnemy_Implementation() override;


};
