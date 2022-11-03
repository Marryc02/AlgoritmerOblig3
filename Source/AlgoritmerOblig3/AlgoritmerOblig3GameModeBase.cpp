// Copyright Epic Games, Inc. All Rights Reserved.


#include "AlgoritmerOblig3GameModeBase.h"
#include "Kismet/GameplayStatics.h"

void AAlgoritmerOblig3GameModeBase::BeginPlay()
{
	Super::BeginPlay();

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
