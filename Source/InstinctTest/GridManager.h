// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

class ATile;


UCLASS()
class INSTINCTTEST_API AGridManager : public AActor
{
	GENERATED_BODY()

protected:
	TArray<TArray<ATile*>> Grid2DArray;
	UPROPERTY(BlueprintReadOnly, Category = "Grid|Layout")
		int32 GridWidth;

	UPROPERTY(BlueprintReadOnly, Category = "Grid|Layout")
		int32 GridHeight;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
		float TileHorizontalOffset;
	
	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
		float TileVerticalOffset;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
		TSubclassOf<ATile> GrassTile;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
		TSubclassOf<ATile> WaterTile;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
		TSubclassOf<AActor> SimpleCube;

	UPROPERTY(EditAnywhere, Category = "Grid|Layout")
		float ChanceofWater;


public:	
	// Sets default values for this actor's properties
	AGridManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
