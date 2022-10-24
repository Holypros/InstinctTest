// Fill out your copyright notice in the Description page of Project Settings.


#include "BallCharacter.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABallCharacter::ABallCharacter()
{
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Comp"));
	RootComponent = SceneComp;
	StaticMesh->SetupAttachment(SceneComp);
	SpringArm->SetupAttachment(StaticMesh);
	CameraComp->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	
}





// Called to bind functionality to input
void ABallCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

