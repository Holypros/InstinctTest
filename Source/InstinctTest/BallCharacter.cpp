// Fill out your copyright notice in the Description page of Project Settings.


#include "BallCharacter.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABallCharacter::ABallCharacter()
{
	//SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Comp"));
	SetRootComponent(StaticMesh);
	
	StaticMesh->SetupAttachment(SceneComp);
	SpringArm->SetupAttachment(StaticMesh);
	CameraComp->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	
}





// Called to bind functionality to input
void ABallCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForwardOrBackword", this, &ABallCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRightOrLeft", this, &ABallCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABallCharacter::TurnFunction);
	PlayerInputComponent->BindAxis("LookUp", this, &ABallCharacter::LookUpFunction);
}

void ABallCharacter::MoveForward(float value)
{
	FVector forceVector = CameraComp->GetForwardVector();
	FName BoneName = TEXT("None");
	forceVector.X = forceVector.X * value * ballSpeed;
	forceVector.Y = forceVector.Y * value * ballSpeed;
	StaticMesh->AddForce(forceVector, BoneName, true);
}

void ABallCharacter::MoveRight(float value)
{
	FVector forceVector = CameraComp->GetRightVector();
	FName BoneName = TEXT("None");
	forceVector.X = forceVector.X * value * ballSpeed;
	forceVector.Y = forceVector.Y * value * ballSpeed;
	StaticMesh->AddForce(forceVector, BoneName, true);
}

void ABallCharacter::TurnFunction(float value)
{
	AddControllerYawInput(value);
}

void ABallCharacter::LookUpFunction(float value)
{
	AddControllerPitchInput(value);
}

bool ABallCharacter::IsEnemy_Implementation()
{
	return true;
}

