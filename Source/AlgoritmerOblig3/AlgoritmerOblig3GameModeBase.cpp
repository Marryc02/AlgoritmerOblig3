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
                if (!newActor) {
                    UE_LOG(LogTemp, Warning, TEXT("actor didn't spawn bruh"));
                }
                AllNodesList.Add(newActor);
            }
            bHasNodesSpawned = true;
           

            for (int i = 0; i < AllNodesList.Num(); i++) {
                if (AllNodesList[i]->ConnectedNodesList.Num() == 0) {
                    AllNodesList[i]->IncreaseCollisionSphereSize();
                }
            }

            ASphereActor* RDMSphere = AllNodesList[FMath::RandRange(0, (AllNodesList.Num() - 1))];
            ChunkArray.Add(RDMSphere);
            while (!CheckConnection(RDMSphere));

            DrawDebugLines();
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
        ChunkArray.Empty();
        bHasNodesSpawned = false;
    }
}

FVector AAlgoritmerOblig3GameModeBase::GetRandomSpawnNumbers() {
    int range = 2000;
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

bool AAlgoritmerOblig3GameModeBase::CheckConnection(ASphereActor* RDM) {
    bHasConstructedChunk = false;

    while (!bHasConstructedChunk) {
        for (int i = 0; i < ChunkArray.Num(); i++) {
            for (int j = 0; j < ChunkArray[i]->ConnectedNodesList.Num(); j++) {
                // if the connected node is not in the chunk array, add it to the chunk array
                if (ChunkArray.Find(ChunkArray[i]->ConnectedNodesList[j]) == INDEX_NONE) {
                    ChunkArray.Add(ChunkArray[i]->ConnectedNodesList[j]);
                }
            }
        }
        UE_LOG(LogTemp, Warning, TEXT("FIRST WHILE"));
        bHasConstructedChunk = true;
    }
    UE_LOG(LogTemp, Warning, TEXT("AllNodesNum value is: %d"), AllNodesList.Num());
    UE_LOG(LogTemp, Warning, TEXT("ChunkArrayNum value is: %d"), ChunkArray.Num());
    if (AllNodesList.Num() != ChunkArray.Num()) {
        bool bFoundDisconnectedNode = false;
        while (!bFoundDisconnectedNode) {
            UE_LOG(LogTemp, Warning, TEXT("SECOND WHILE"));

            for (int j = 0; j < AllNodesList.Num(); j++) {
                // Check if AllNodesList[j] is in the ChunkArray
                if (ChunkArray.Find(AllNodesList[j]) != INDEX_NONE) {
                    // AllNodesList[j] is in ChunkArray
                    bFoundDisconnectedNode = false;
                }
                else {
                    // AllNodesList[j] is not in ChunkArray, yet
                    bFoundDisconnectedNode = true;
                    RDM = AllNodesList[j];
                    break;
                }
            }
        }

        ASphereActor* temp = ChunkArray[FMath::RandRange(0, (ChunkArray.Num() - 1))];
        temp->ConnectedNodesList.Add(RDM);
        RDM->ConnectedNodesList.Add(temp);
        return false;
    }
    else {
        return true;
    }
}
