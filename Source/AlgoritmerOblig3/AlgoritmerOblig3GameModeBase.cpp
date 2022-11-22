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
            
            StartNode = AllNodesList[FMath::RandRange(0, (AllNodesList.Num() - 1))];
            EndNode = AllNodesList[FMath::RandRange(0, (AllNodesList.Num() - 1))];

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
        SearchedNodes.Empty();
        VisitedNodes.Empty();
        PriorityQueue.Empty();
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

void AAlgoritmerOblig3GameModeBase::RunAlgorithm(bool bAStarIsRunning) {
    // First we begin by checking if there are nodes present to search through
    if (!bHasNodesSpawned) {
        return;
    }

    bHasReachedEnd = false;
    

    // Now we set the cost of every node to be the max value, except for the start value
    for (int i = 0; i < AllNodesList.Num(); i++) {
        if (!AllNodesList[i]->bIsStartNode) {
            AllNodesList[i]->Cost = INT_MAX;
        }
        else {
            AllNodesList[i]->Cost = 0;
        }
    }

    // Now we add in the start node to the SearchedNodes array, the cost of the node is 0
    // since it doesn't cost anything to move to the place you're already in
    SearchedNodes.Add(StartNode);
    SearchedNodes[0]->Cost = SearchedNodes[0]->CalculateDistance(SearchedNodes[0]->Position);
    SearchedNodes[0]->PathToGetTo.Add(SearchedNodes[0]);

    ASphereActor* CompareNode;
    ASphereActor* ShortestNode;

    while (SearchedNodes.Num() != AllNodesList.Num() || bHasReachedEnd == false) {
        // We first run through the searched nodes
        for (int i = 0; i < SearchedNodes.Num(); i++) {
            CompareNode = nullptr;
            ShortestNode = nullptr;
            // Then we check their connections and add the one with shortest path to SearchedNodes
            for (int j = 0; j < SearchedNodes[i]->ConnectedNodesList.Num(); j++) {
                
                CompareNode = SearchedNodes[i]->ConnectedNodesList[j];
                CompareNode->Distance = EndNode->Position - CompareNode->Position;

                if (bAStarIsRunning) {
                    CompareNode->Cost += CompareNode->CalculateDistance(CompareNode->Distance);
                }
                
                // A quick check to see if we ShortestNode is a nullptr
                // if it is we assume that the CompareNode is the node with shortest path
                if (ShortestNode == nullptr) {
                    if (SearchedNodes.Find(CompareNode) == INDEX_NONE) {
                        ShortestNode = CompareNode;
                    }
                }         
                // Here we check if the CompareNode is smaller than the ShortestNode
                // If it is we have a new ShortestNode, otherwise we just ignore it for now

                if (ShortestNode != nullptr && CompareNode != nullptr) {
                    if (ShortestNode->Cost > CompareNode->Cost && SearchedNodes.Find(CompareNode) == INDEX_NONE) {
                        ShortestNode = CompareNode;
                    }
                }

                /*else if (FMath::Abs(ShortestNode->Cost) > FMath::Abs(CompareNode->Cost) && SearchedNodes.Find(CompareNode) == INDEX_NONE) {
                    ShortestNode = CompareNode;
                }
                */

                // Here we check if the node is actually the end node, if it is we go straight to it
                if (CompareNode->bIsEndNode) {
                    ShortestNode = CompareNode;
                }
            }

            // Here we fix the pathing for the node and add it to the SearchedNodes array
            if (ShortestNode != nullptr) {
                ShortestNode->PathToGetTo = SearchedNodes[i]->PathToGetTo;
                ShortestNode->PathToGetTo.Add(ShortestNode);
                SearchedNodes.Add(ShortestNode);
                // We also end the while loop if we've found the end node
                if (ShortestNode->bIsEndNode) {
                    bHasReachedEnd = true;
                    break;
                }
            }
        }
    }

    ChangeMaterials(SearchedNodes);
}

void AAlgoritmerOblig3GameModeBase::ResetPath() {
    for (int i = 0; i < SearchedNodes.Num(); i++) {
        if (SearchedNodes[i]->bIsStartNode == false && SearchedNodes[i]->bIsEndNode == false) {
            SearchedNodes[i]->SphereMesh->SetMaterial(0, SearchedNodes[i]->BasicMat);
        }
    }
    for (int i = 0; i < VisitedNodes.Num(); i++) {
        if (VisitedNodes[i]->bIsStartNode == false && VisitedNodes[i]->bIsEndNode == false) {
            VisitedNodes[i]->SphereMesh->SetMaterial(0, VisitedNodes[i]->BasicMat);
        }
    }
    SearchedNodes.Empty();
    VisitedNodes.Empty();
    PriorityQueue.Empty();
}

// Also known as Best First Search
void AAlgoritmerOblig3GameModeBase::InformedSearch() {
    if (!bHasNodesSpawned) {
        return;
    }

    if (StartNode == nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("StartNode is nullptr"));
        return;
    }

    bHasReachedEnd = false;
    bool bHasFoundEnd = false;
    
    PriorityQueue.Add(StartNode);

    PriorityQueue[0]->PathToGetTo.Add(PriorityQueue[0]);
    ASphereActor* ConnectedNode;

    while (PriorityQueue.Num() != 0 || bHasReachedEnd == false) {
        for (int i = 0; i < PriorityQueue[0]->ConnectedNodesList.Num(); i++) {

            ConnectedNode = PriorityQueue[0]->ConnectedNodesList[i];
            ConnectedNode->CalculatedDistance = DistanceToEnd(ConnectedNode);
            ConnectedNode->Cost += abs(ConnectedNode->CalculatedDistance / 10);

            if (VisitedNodes.Num() != 0) {
                for (int j = 0; j < VisitedNodes.Num(); j++) {
                    if (VisitedNodes.Find(PriorityQueue[0]->ConnectedNodesList[i]) == INDEX_NONE) {
                        PriorityQueue.Add(PriorityQueue[0]->ConnectedNodesList[i]);
                        PriorityQueue[0]->ConnectedNodesList[i]->PathToGetTo = PriorityQueue[0]->PathToGetTo;
                        PriorityQueue[0]->ConnectedNodesList[i]->PathToGetTo.Add(PriorityQueue[0]->ConnectedNodesList[i]);
                    }
                }
            }
            else {
                PriorityQueue.Add(PriorityQueue[0]->ConnectedNodesList[i]);
                PriorityQueue[0]->ConnectedNodesList[i]->PathToGetTo = PriorityQueue[0]->PathToGetTo;
                PriorityQueue[0]->ConnectedNodesList[i]->PathToGetTo.Add(PriorityQueue[0]->ConnectedNodesList[i]);
            }            
        }
        VisitedNodes.Add(PriorityQueue[0]);
        PriorityQueue.RemoveAt(0, 1, true);

        for (int i = 0; i < PriorityQueue.Num(); i++) {
            if (PriorityQueue[i]->bIsEndNode) {
                PriorityQueue.Swap(0, i);
                bHasFoundEnd = true;
                break;
            }
        }

        // Selection sort to sort the spheres based on cost
        // Selection sort is cucked - causes infinite loop
        if (PriorityQueue.Num() != 0) {
            if (!bHasFoundEnd) {
                for (int i = 0; i < PriorityQueue.Num() - 1; i++) {
                    int Smallest = i;

                    for (int j = i + 1; j < PriorityQueue.Num(); j++) {
                        if (PriorityQueue[j]->Cost < PriorityQueue[Smallest]->Cost) {
                            Smallest = j;
                        }
                    }

                    if (Smallest != i) {
                        PriorityQueue.SwapMemory(i, Smallest);
                    }
                }
            }
            
            if (PriorityQueue[0]->bIsEndNode) {
                bHasReachedEnd = true;
                ChangeMaterials(VisitedNodes);
                return;
            }
            
        }
        else {
            bHasReachedEnd = true;
            ChangeMaterials(VisitedNodes);
            return;
        }

    }

    
}

int AAlgoritmerOblig3GameModeBase::DistanceToEnd(ASphereActor* node) {
    int x{};

    x += EndNode->Position.X - node->Position.X;
    x += EndNode->Position.Y - node->Position.Y;
    x += EndNode->Position.Z - node->Position.Z;
    return x;
}


void AAlgoritmerOblig3GameModeBase::ChangeMaterials(TArray<ASphereActor*> &mArray) {
    // Here we change the material of all the nodes
    // i starts at 1 since the first item in SearchedNodes is always the start node
    for (int i = 1; i < mArray.Num(); i++) {
        if (!mArray[i]->bIsEndNode) {
            mArray[i]->SphereMesh->SetMaterial(0, mArray[i]->PathMat);
        }
    }

    // Finally we highlight the path to the end node
    for (int i = 0; i < EndNode->PathToGetTo.Num(); i++) {
        if (EndNode->PathToGetTo[i]->bIsStartNode == false && EndNode->PathToGetTo[i]->bIsEndNode == false) {
            EndNode->PathToGetTo[i]->SphereMesh->SetMaterial(0, EndNode->ShortMat);
        }
    }
}
