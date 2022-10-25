// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Kismet/KismetMathLibrary.h"
#include "TurretAnimInterface.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(Root);
	TurretMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(Root);
	Beam = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Beam"));
	Beam->SetupAttachment(TurretMesh, TEXT("BeamSocket"));

	Target1 = CreateDefaultSubobject<USceneComponent>(TEXT("Target1"));
	Target1->SetupAttachment(Root);
	Target2 = CreateDefaultSubobject<USceneComponent>(TEXT("Target2"));
	Target2->SetupAttachment(Root);

	FollowTarget = CreateDefaultSubobject<USceneComponent>(TEXT("FollowTarget"));
	FollowTarget->SetupAttachment(Root);

}

void ATurret::UpdateLookAtTarget()
{
	FVector Start = TurretMesh->GetSocketLocation("BeamSocket");
	FVector End = FollowTarget->GetComponentLocation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

	if (TurretMesh->GetAnimInstance()->Implements<UTurretAnimInterface>()) 
	{
		ITurretAnimInterface::Execute_UpdateLookAtRotation(TurretMesh->GetAnimInstance(), LookAtRotation);
	}

}

void ATurret::ChangeBeamTarget()
{
	timerCount++;
	if (timerCount % 2 == 0) 
	{
		FollowTarget->SetWorldLocation(Target1->GetComponentLocation());
	}
	else 
	{
		FollowTarget->SetWorldLocation(Target2->GetComponentLocation());
	}
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(timerHandle, this, &ATurret::ChangeBeamTarget, 5.f, true,1.f);
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateLookAtTarget();

}
