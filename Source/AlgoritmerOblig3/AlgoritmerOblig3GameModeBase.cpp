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
    // Check if there already are nodes spawned, if there are no nodes, we can spawn them
    if (!bHasNodesSpawned) {
        UWorld* World = GetWorld();
        if (World)
        {
            for (int i = 0; i < SpawnAmount; i++)
            {
                ASphereActor* newActor = World->SpawnActor<ASphereActor>(SphereActor_BP, GetRandomSpawnNumbers(), FRotator::ZeroRotator);
                // Check for nullptr
                if (!newActor) {
                    UE_LOG(LogTemp, Warning, TEXT("actor didn't spawn bruh"));
                }
                else {
                    AllNodesList.Add(newActor);
                }
            }
            bHasNodesSpawned = true;
           
            // Check now if there are nodes without a connection, if so we increase the size of the collision box
            // until in collides with another node, and adds it to its ConnectedNodesList and then breaks.
            for (int i = 0; i < AllNodesList.Num(); i++) {
                if (AllNodesList[i]->ConnectedNodesList.Num() == 0) {
                    AllNodesList[i]->IncreaseCollisionSphereSize();
                }
            }

            // Here we are going to connect some chunks together, if there are a cluster of nodes with
            // connections, but the clusters are seperate, here we merge them via one connection
            // Now there will always be a path a node can take to get to another node

            ASphereActor* RDMSphere = AllNodesList[FMath::RandRange(0, (AllNodesList.Num() - 1))];
            ChunkArray.Add(RDMSphere);
            while (!CheckConnection(RDMSphere));

            // Here we select which node will be the start node and which will be the end node
            // Choose 1 random node and set it's bIsStartNode value to true
            // Then we choose another random node that is not the first one we chose and change it's
            // bIsEndNode value to true, afterwards we change their materials

            ASphereActor* StartNode = AllNodesList[FMath::RandRange(0, (AllNodesList.Num() - 1))];
            ASphereActor* EndNode = AllNodesList[FMath::RandRange(0, (AllNodesList.Num() - 1))];

            // Check to ensure that the start is also not the end
            while (StartNode == EndNode) {
                EndNode = AllNodesList[FMath::RandRange(0, (AllNodesList.Num() - 1))];
            }

            StartNode->bIsStartNode = true;
            StartNode->SphereMesh->SetMaterial(0, StartNode->StartMat);
            EndNode->bIsEndNode = true;
            EndNode->SphereMesh->SetMaterial(0, EndNode->EndMat);

            DrawDebugLines();
        }        
    } 
}

void AAlgoritmerOblig3GameModeBase::DeleteSpheres() {
    // We first check if there are actually nodes there to delete
    if (bHasNodesSpawned) {
        for (int i = 0; i < AllNodesList.Num(); i++) {
            AllNodesList[i]->Destroy();
            FlushPersistentDebugLines(GetWorld());
        }
        AllNodesList.Empty();
        ChunkArray.Empty();
        bHasNodesSpawned = false;
    }
}

FVector AAlgoritmerOblig3GameModeBase::GetRandomSpawnNumbers() {
    // This is used for where the nodes can spawn
    int range = 2000;
    int x = FMath::RandRange(range * (-1), range);
    int y = FMath::RandRange(range * (-1), range);
    int z = FMath::RandRange(range * (-1), range);
    return FVector(x, y, z);
}

void AAlgoritmerOblig3GameModeBase::DrawDebugLines() {
    // Draws debuglines between connected nodes based on their ConnectedNodesList
    for (int i = 0; i < AllNodesList.Num(); i++) {
        for (int j = 0; j < AllNodesList[i]->ConnectedNodesList.Num(); j++) {
            DrawDebugLine(GetWorld(), AllNodesList[i]->Position, AllNodesList[i]->ConnectedNodesList[j]->Position,
                FColor::White,true, -1.f, 0U,5.f);
        }
        
    }
}

bool AAlgoritmerOblig3GameModeBase::CheckConnection(ASphereActor* RDM) {
    bHasConstructedChunk = false;

    /*
        Step 1: Select a random node from all nodes (done in the SpawnNodes function).
        Step 2: Add all nodes that the random node can reach into the ChunkArray array.
        Step 3: Check if the ChunkArray is smaller than the list of all nodes.
        If so then we know that there are multiple chunks and that they are not connected.
        Otherwise we know every node can reach every other node.
        Step 4 (if we have disconnected chunks): Pick a random node that is not in ChunkArray
        and draw a line between that and a random node in ChunkArray
        Step 5: Rinse and repeat, connected chunks will be checked in step 2.

        Once it is all done, we know that every node can connect to any other node.  
    */

    // Step 2
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

    // Step 3
    if (AllNodesList.Num() != ChunkArray.Num()) {
        // Step 4
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
        // Step 5, it will keep looping as long as it returns false
        return false;
    }
    else {
        return true;
    }
}
