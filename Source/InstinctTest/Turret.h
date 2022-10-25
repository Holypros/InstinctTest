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

	int timerCount = 0;
	FTimerHandle timerHandle;

	UFUNCTION()
		void UpdateLookAtTarget();

	UFUNCTION()
		void ChangeBeamTarget();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
