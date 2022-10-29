// Fill out your copyright notice in the Description page of Project Settings.


#include "BallCharacter.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"

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

	P_Explosion = CreateDefaultSubobject<UParticleSystem>(TEXT("Explosion"));

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.5f, 96.f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

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
	return (health>0);
}

void ABallCharacter::UpdateScore()
{
	score++;
}

int ABallCharacter::getScore()
{
	return score;
}




float ABallCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (health <= 0) 
	{
		return 0;
	}
	float DamageCaused = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageCaused = FMath::Min(health, DamageCaused); //if the current health value is less than the damage caused then the max value damagecaused should be is the current health value 
	health -= DamageCaused;

	if (health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Dead"));
		DisableInput(GetWorld()->GetFirstPlayerController());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), P_Explosion, GetActorLocation(), GetActorRotation());
		StaticMesh->DestroyComponent();
	}
	return DamageCaused;
}