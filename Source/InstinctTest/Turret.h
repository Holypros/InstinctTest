// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "Turret.generated.h"


UCLASS()
class INSTINCTTEST_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		USkeletalMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		UStaticMeshComponent* Beam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		USceneComponent* Target1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		USceneComponent* Target2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		USceneComponent* FollowTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		float ChangeTargetDelay = 5.f;

	// Rotation data
	int timerCount = 0;
	FTimerHandle timerHandle;

	FRotator LookAtRotation;
	FRotator TargetRotation;
	FRotator RotationDelta; //Difference between current rotation and rotation we need to have


	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret")
		float BeamLength = 1000.f;

	UFUNCTION()
		void UpdateLookAtTarget(float DeltaTime);

	UFUNCTION()
		void ChangeBeamTarget();

	UFUNCTION(BlueprintCallable)
		void SetBeamLength(float length);

	UFUNCTION()
		void TraceBeam();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
