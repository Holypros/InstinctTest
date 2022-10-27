// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Kismet/KismetMathLibrary.h"
#include "CharacterInterface.h"
#include "TurretAnimInterface.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"

#define OUT
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

	SetBeamLength(BeamLength);

	P_MuzzleFlash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Muzzle Flash"));
	P_MuzzleFlash->SetupAttachment(TurretMesh,TEXT("BeamSocket"));

}

void ATurret::UpdateLookAtTarget(float DeltaTime)
{
	if (LookAtRotation.Equals(TargetRotation,1.f))
	{
		return;
	}

	LookAtRotation += RotationDelta * 0.3f * DeltaTime;

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

	FVector Start = TurretMesh->GetSocketLocation("BeamSocket");
	FVector End = FollowTarget->GetComponentLocation();
	TargetRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

	RotationDelta = TargetRotation - LookAtRotation;
	RotationDelta.Normalize(); //incase we have odd values
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(timerHandle, this, &ATurret::ChangeBeamTarget, ChangeTargetDelay, true,1.f);
	GetWorldTimerManager().SetTimer(TracetimerHandle, this, &ATurret::TraceBeam, .1f, true, .1f);

}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Enemy) 
	{
		FollowEnemy(DeltaTime);
	}
	else 
	{
		UpdateLookAtTarget(DeltaTime);
	}
	

	//TraceBeam();

}

void ATurret::SetBeamLength(float length)
{
	Beam->SetRelativeScale3D(FVector(length / 100, Beam->GetRelativeScale3D().Y, Beam->GetRelativeScale3D().Z));
	Beam->SetRelativeLocation(FVector(length / (-2.0f), 0, 0));
}

void ATurret::TraceBeam()
{
	
	FHitResult HitResult;
	FVector Start = TurretMesh->GetSocketLocation("BeamSocket");
	FVector End = Start + Beam->GetForwardVector() * BeamLength;

	FCollisionQueryParams CollQueryParams;
	CollQueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel
	(
		OUT HitResult,
		Start,
		End,
		ECollisionChannel::ECC_Camera,
		CollQueryParams
	);

	if (bHit)
	{
		SetBeamLength(HitResult.Distance);
		CheckEnemy(HitResult.GetActor());
	}
	else
	{		
		SetBeamLength(BeamLength);		
	}
}

void ATurret::CheckEnemy(AActor* hitActor)
{
	if (hitActor->Implements<UCharacterInterface>())
	{
		bool bEnemy = ICharacterInterface::Execute_IsEnemy(hitActor);
		if (bEnemy) 
		{
			if (!Enemy) //just to execute once at the first time
			{
				Enemy = hitActor;	
				GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ATurret::Shoot, 1.f, true, 0.4f);
			}		
		}
	}
	else
	{
		if (Enemy) {
			Enemy = nullptr;
			GetWorldTimerManager().ClearTimer(ShootTimerHandle);
		}
		
	}
}

void ATurret::FollowEnemy(float DeltaTime)
{
	FVector Start = TurretMesh->GetSocketLocation("BeamSocket");
	FVector End = Enemy->GetActorLocation();
	//UE_LOG(LogTemp, Log, TEXT("Actor location: %s"), *End.ToString());
	FRotator RotationtoEnemy = UKismetMathLibrary::FindLookAtRotation(Start, End);



	LookAtRotation = FMath::RInterpTo(LookAtRotation, RotationtoEnemy, DeltaTime,10);

	if (TurretMesh->GetAnimInstance()->Implements<UTurretAnimInterface>())
	{
		ITurretAnimInterface::Execute_UpdateLookAtRotation(TurretMesh->GetAnimInstance(), LookAtRotation);
	}
}

void ATurret::Shoot()
{
	UGameplayStatics::PlaySoundAtLocation(this, ShootSound, P_MuzzleFlash->GetComponentLocation());
	P_MuzzleFlash->Activate(true);

	FHitResult HitResult;
	FVector Start = TurretMesh->GetSocketLocation("BeamSocket");
	FVector End = Start + Beam->GetForwardVector() * BeamLength;

	FCollisionQueryParams CollQueryParams;
	CollQueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel
	(
		OUT HitResult,
		Start,
		End,
		ECollisionChannel::ECC_Camera,
		CollQueryParams
	);

	if (bHit) 
	{
		FPointDamageEvent DamageEvent(damageToPlayer, HitResult, Beam->GetForwardVector(), nullptr);
		HitResult.GetActor()->TakeDamage(damageToPlayer, DamageEvent, GetInstigatorController(), this);
	}
}
