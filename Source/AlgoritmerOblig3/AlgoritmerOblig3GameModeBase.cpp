// Copyright Epic Games, Inc. All Rights Reserved.

#include "Kismet/GameplayStatics.h"
#include "AlgoritmerOblig3GameModeBase.h"

void AAlgoritmerOblig3GameModeBase::BeginPlay()
{
	Super::BeginPlay();

    SpawnSpheres(0);
}

void AAlgoritmerOblig3GameModeBase::SpawnSpheres(int SpawnAmount)
{
    UWorld* World = GetWorld();
    if (World)
    {
        for (int i = 0; i < SpawnAmount; i++)
        {
            int x = FMath::RandRange(-500, 500);
            int y = FMath::RandRange(-500, 500);
            int z = FMath::RandRange(-500, 500);

            World->SpawnActor<ASphereActor>(SphereActor_BP, FVector(x, y , z), FRotator::ZeroRotator);
        }
    }
}
