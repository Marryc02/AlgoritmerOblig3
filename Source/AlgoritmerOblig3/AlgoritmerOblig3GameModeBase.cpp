// Copyright Epic Games, Inc. All Rights Reserved.


#include "AlgoritmerOblig3GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

void AAlgoritmerOblig3GameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

void AAlgoritmerOblig3GameModeBase::SpawnSpheres(int SpawnAmount)
{
    if (!bHasNodesSpawned) {
        UWorld* World = GetWorld();
        if (World)
        {
            for (int i = 0; i < SpawnAmount; i++)
            {
                ASphereActor* newActor = World->SpawnActor<ASphereActor>(SphereActor_BP, GetRandomSpawnNumbers(), FRotator::ZeroRotator);
                AllNodesList.Add(newActor);
            }
            bHasNodesSpawned = true;
        }
    } 
   
}

void AAlgoritmerOblig3GameModeBase::DeleteSpheres() {
    if (bHasNodesSpawned) {
        for (int i = 0; i < AllNodesList.Num(); i++) {
            // Not qutie sure if this will work the way I want it to
            AllNodesList[i]->Destroy();
        }
        AllNodesList.Empty();
        bHasNodesSpawned = false;
    }
}

FVector AAlgoritmerOblig3GameModeBase::GetRandomSpawnNumbers() {
    int x = FMath::RandRange(-500, 500);
    int y = FMath::RandRange(-500, 500);
    int z = FMath::RandRange(-500, 500);
    return FVector(x, y, z);
}
