// Copyright Epic Games, Inc. All Rights Reserved.


#include "AlgoritmerOblig3GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
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
            DrawDebugLines();

            /*for (int i = 0; i < AllNodesList.Num(); i++) {
                if (AllNodesList[i]->ConnectedNodesList.Num() == 0) {
                    AllNodesList[i]->IncreaseCollisionSphereSize();
                }
            }*/
        }        
    } 
}

void AAlgoritmerOblig3GameModeBase::DeleteSpheres() {
    if (bHasNodesSpawned) {
        for (int i = 0; i < AllNodesList.Num(); i++) {
            // Not quite sure if this will work the way I want it to
            AllNodesList[i]->Destroy();
            FlushPersistentDebugLines(GetWorld());
        }
        AllNodesList.Empty();
        bHasNodesSpawned = false;
    }
}

FVector AAlgoritmerOblig3GameModeBase::GetRandomSpawnNumbers() {
    int range = 1000;
    int x = FMath::RandRange(range * (-1), range);
    int y = FMath::RandRange(range * (-1), range);
    int z = FMath::RandRange(range * (-1), range);
    return FVector(x, y, z);
}

void AAlgoritmerOblig3GameModeBase::DrawDebugLines() {
    for (int i = 0; i < AllNodesList.Num(); i++) {
        for (int j = 0; j < AllNodesList[i]->ConnectedNodesList.Num(); j++) {
            DrawDebugLine(GetWorld(), AllNodesList[i]->Position, AllNodesList[i]->ConnectedNodesList[j]->Position,
                FColor::White,true, -1.f, 0U,5.f);
        }
    }
}
