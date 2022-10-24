// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Tile.h"
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

	FString file = FPaths::ProjectConfigDir();

	file.Append(TEXT("data.txt"));

	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	FString FileContent;
	TArray<int32> myArr;

	if (FileManager.FileExists(*file))
	{
		// We use the LoadFileToString to load the file into
		if (FFileHelper::LoadFileToString(FileContent, *file, FFileHelper::EHashOptions::None))
		{
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *FileContent);
			myArr = ParseString(&FileContent);
			for (int i = 0; i < myArr.Num(); i++) {
				UE_LOG(LogTemp, Warning, TEXT("%d"), myArr[i]);
			}
			GridHeight = myArr[0];
			GridWidth = myArr[1];			
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
	//UE_LOG(LogTemp, Warning, TEXT("The integer value is: %d"), i);
	TSubclassOf<AActor> actortoSpawn = SimpleCube;
	AActor* newCube = GetWorld()->SpawnActor<AActor>(actortoSpawn, FVector(FIntPoint(i*100, j*100)), FRotator::ZeroRotator);


	TSubclassOf<AActor> wallToSpawn = Wall;
	AActor* firstWall = GetWorld()->SpawnActor<AActor>(wallToSpawn, FVector(FIntPoint(-56.7f, -60)), FRotator::ZeroRotator); // First Wall
	firstWall->SetActorScale3D(FVector(GridWidth, 1, 1));

	TSubclassOf<AActor> wall1ToSpawn = Wall1;
	AActor* secondWall = GetWorld()->SpawnActor<AActor>(wall1ToSpawn, FVector(FIntPoint(-56.7f, -55)), FRotator::ZeroRotator); // second Wall
	secondWall->SetActorScale3D(FVector(1, GridHeight, 1));

	AActor* ThirdWall = GetWorld()->SpawnActor<AActor>(wallToSpawn, FVector(FIntPoint(-56.7f, GridHeight*100 -60)), FRotator::ZeroRotator); // First Wall
	ThirdWall->SetActorScale3D(FVector(GridWidth, 1, 1));

	AActor* fourthWall = GetWorld()->SpawnActor<AActor>(wall1ToSpawn, FVector(FIntPoint(GridWidth*100-56.7f, -55)), FRotator::ZeroRotator); // second Wall
	fourthWall->SetActorScale3D(FVector(1, GridHeight, 1));
}

