// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"


class UStaticMeshComponent;

UENUM()
enum class ETileType : uint8 {
	INVALID,
	GRASS,
	WATER,
	MAX UMETA(Hidden)
};

UCLASS()
class INSTINCTTEST_API ATile : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleInstanceOnly, Category = "Tile")
		FIntPoint TileIndex;




protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile") //exposing to the editor
		ETileType tileType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile") //exposing to the editor
		UStaticMeshComponent* TileMesh;
	
public:	
	// Sets default values for this actor's properties
	ATile();


};
