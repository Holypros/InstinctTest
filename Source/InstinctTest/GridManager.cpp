// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Tile.h"
#include "INSTINCTTEST.h"
#include "DataSave.h"
#include "Math/UnrealMathUtility.h" 



// Sets default values
AGridManager::AGridManager()
{
 	
}



// Function to parse Text file
TArray<int32> ParseString(FString* string) {
	TArray<int32> arr1;
	FString temp = "";
	int32 newInt;
	for (int i = 0; i < string->Len(); i++) {
		if ((*string)[i] == ',') {
			newInt = FCString::Atoi(*temp);
			temp = "";
			arr1.Add(newInt);
		}
		else
		{
			temp += (*string)[i];
		}
	}
	if (temp != "") {
		newInt = FCString::Atoi(*temp);
		arr1.Add(newInt);
	}
	return arr1;
}


// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();

#pragma region ReadFromFile

	
	FString file = FPaths::ProjectContentDir();

	file.Append(TEXT("Files/data.txt"));

	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	FString FileContent;


	if (FileManager.FileExists(*file))
	{
		// We use the LoadFileToString to load the file into
		if (FFileHelper::LoadFileToString(FileContent, *file, FFileHelper::EHashOptions::None))
		{
			UE_LOG(LogTemp, Warning, TEXT("Actually Read the file from content"));
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *FileContent);
			myArr = ParseString(&FileContent);
			GridHeight = myArr[0];
			GridWidth = myArr[1];
			numofturrets = myArr[2];
			if (numofturrets >= (GridHeight * GridWidth)) //safety check to make sure the number of turrets written in file does not exceed the tile size by half
			{
				numofturrets = (int32)((GridHeight * GridWidth)/2);
			}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did not load text from file"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: ERROR: Can not read the file because it was not found."));
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"), *file);
	}
#pragma endregion



	Grid2DArray.SetNumZeroed(GridWidth); //initilize the array as zero instead of null or empty
	flagArray.SetNumZeroed(GridWidth);
	for (int32 i = 0; i < Grid2DArray.Num(); ++i) 
	{
		Grid2DArray[i].SetNumZeroed(GridHeight);
		flagArray[i].SetNumZeroed(GridHeight);
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
			//newTile->SetActorLabel(FString::Printf(TEXT("Tile_%d-%d"), x, y));
			Grid2DArray[x][y] = newTile; //The 2D array now holds each actor
		//UE_LOG(LogTemp, Warning, TEXT("Bool value is: %s"), flagArray[x][y] ? TEXT("true") : TEXT("false"));
		}
	}

//spawning turrets randomly on grid
#pragma region Spawning Turrets
	int32 i;
	int32 j;
	TSubclassOf<AActor> turrettoSpawn = Turret;
	for (int a = 0; a < numofturrets; ++a) {
		i = FMath::RandRange(0, GridWidth - 1);
		j = FMath::RandRange(0, GridHeight - 1);
		while (flagArray[i][j] == true) {
			i = FMath::RandRange(0, GridWidth - 1);
			j = FMath::RandRange(0, GridHeight - 1);
		}
		AActor* newTurret = GetWorld()->SpawnActor<AActor>(turrettoSpawn, FVector(FIntPoint(i * 100, j * 100)), FRotator::ZeroRotator);
		flagArray[i][j] = true;
	}
#pragma endregion


//spawning coins
#pragma region SpawningCoins
	TSubclassOf<AActor> cointoSpawn = Coin;

	for (int x = 0; x < (int32)(GridHeight * GridWidth)/2; ++x) 
	{
		i = FMath::RandRange(0, GridWidth - 1);
		j = FMath::RandRange(0, GridHeight - 1);
		while (flagArray[i][j] == true) {
			i = FMath::RandRange(0, GridWidth - 1);
			j = FMath::RandRange(0, GridHeight - 1);
		}
		AActor* newCoin = GetWorld()->SpawnActor<AActor>(cointoSpawn, FVector(FIntPoint(i * 100, j * 100)), FRotator::ZeroRotator);
		flagArray[i][j] = true;

	}

	


#pragma endregion


#pragma region SpawningWallsAroundTheGrid
	TSubclassOf<AActor> wallToSpawn = Wall;
	AActor* firstWall = GetWorld()->SpawnActor<AActor>(wallToSpawn, FVector(FIntPoint(-56.7f, -60)), FRotator::ZeroRotator); // First Wall
	firstWall->SetActorScale3D(FVector(GridWidth, 1, 1));

	TSubclassOf<AActor> wall1ToSpawn = Wall1;
	AActor* secondWall = GetWorld()->SpawnActor<AActor>(wall1ToSpawn, FVector(FIntPoint(-56.7f, -55)), FRotator::ZeroRotator); // second Wall
	secondWall->SetActorScale3D(FVector(1, GridHeight, 1));

	AActor* ThirdWall = GetWorld()->SpawnActor<AActor>(wallToSpawn, FVector(FIntPoint(-56.7f, GridHeight * 100 - 60)), FRotator::ZeroRotator); // First Wall
	ThirdWall->SetActorScale3D(FVector(GridWidth, 1, 1));

	AActor* fourthWall = GetWorld()->SpawnActor<AActor>(wall1ToSpawn, FVector(FIntPoint(GridWidth * 100 - 56.7f, -55)), FRotator::ZeroRotator); // second Wall
	fourthWall->SetActorScale3D(FVector(1, GridHeight, 1));
#pragma endregion

}

