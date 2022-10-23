// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Tile.h"
#include "Math/UnrealMathUtility.h" 

// Sets default values
AGridManager::AGridManager()
{
 	
}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();

	Grid2DArray.SetNumZeroed(GridWidth); //initilize the array as zero instead of null or empty
	for (int32 i = 0; i < Grid2DArray.Num(); ++i) 
	{
		Grid2DArray[i].SetNumZeroed(GridHeight);
	}
	
	for (int32 y = 0; y < GridHeight; ++y) 
	{
		for (int32 x = 0; x < GridWidth; ++x)
		{
			const float xPos = x * TileHorizontalOffset;
			const float yPos = y * TileVerticalOffset;


			TSubclassOf<ATile> tileToSpawn = GrassTile;
			if (FMath::RandRange(0.f, 1.f) <= ChanceofWater)
			{
				tileToSpawn = WaterTile;
			}

			ATile* newTile = GetWorld()->SpawnActor<ATile>(tileToSpawn, FVector(FIntPoint(xPos, yPos)), FRotator::ZeroRotator);
			newTile->TileIndex = FIntPoint(x, y);
			newTile->SetActorLabel(FString::Printf(TEXT("Tile_%d-%d"), x, y));
			Grid2DArray[x][y] = newTile;



		}
	}

	//spawning an actor on the grid at random
	int32 i = FMath::RandRange(0, GridWidth-1);
	int32 j = FMath::RandRange(0, GridHeight-1);

	UE_LOG(LogTemp, Warning, TEXT("The integer value is: %d"), i);
	TSubclassOf<AActor> actortoSpawn = SimpleCube;

	AActor* newCube = GetWorld()->SpawnActor<AActor>(actortoSpawn, FVector(FIntPoint(i*100, j*100)), FRotator::ZeroRotator);
}



