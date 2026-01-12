// Copyright BanMing

#include "Grid/Pathfinding/GridPathfinding.h"

#include "FunctionLibrary/GridShapeLibrary.h"
#include "Grid/GridBase.h"
AGridPathfinding::AGridPathfinding()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGridPathfinding::InitializeGridPathfinding(AGridBase* InGrid)
{
	Grid = InGrid;
}

void AGridPathfinding::FinalizeGridPathfinding()
{
	Grid = nullptr;
}

TArray<FPathfindingData> AGridPathfinding::GetValidTileNeighbors(FIntPoint Index, bool bIncludeDiagonals) const
{
	check(Grid);
	FTileData InputData = Grid->GetTileDatafromGridIndex(Index);
	TArray<FIntPoint> Neighbors = GetTileNeighbors(Index, bIncludeDiagonals);
	TArray<FPathfindingData> Res;
	for (const FIntPoint& I : Neighbors)
	{
		const FTileData CurData = Grid->GetTileDatafromGridIndex(Index);
		const float ZOffset = FMath::Abs(CurData.Transform.GetLocation().Z - InputData.Transform.GetLocation().Z);
		if (Grid->IsWalkableByIndex(I) && ZOffset <= Grid->TileSize.Z)
		{
			FPathfindingData Data;
			Data.Index = I;
			Data.PrevIndex = Index;
			Res.Add(Data);
		}
	}
	return Res;
}

TArray<FIntPoint> AGridPathfinding::GetTileNeighbors(FIntPoint Index, bool bIncludeDiagonals) const
{
	check(Grid);
	switch (Grid->Shape)
	{
		case EGridShape::Square:
			return GetNeighborIndexesForSquare(Index, bIncludeDiagonals);
		case EGridShape::Triangle:
			return GetNeighborIndexesForTriangle(Index, bIncludeDiagonals);
		case EGridShape::Hexagon:
			return GetNeighborIndexesForHexagon(Index);
	}
	return TArray<FIntPoint>();
}

bool AGridPathfinding::IsDiagonals(FIntPoint Index1, FIntPoint Index2) const
{
	TArray<FIntPoint> Neighbors = GetTileNeighbors(Index1, false);

	return !Neighbors.Contains(Index2);
}

TArray<FIntPoint> AGridPathfinding::FindPath(FIntPoint Start, FIntPoint Target, bool bIncludeDiagonals)
{
	ResetPathfindingInfo();

	StartIndex = Start;
	TargetIndex = Target;
	bFindingIncludeDiagonals = bIncludeDiagonals;

	TArray<FIntPoint> Res;

	if (!IsInputDataValid())
	{
		return Res;
	}

	FPathfindingData TilePathData;
	TilePathData.Index = Start;
	TilePathData.CostToEnterTile = 1;
	TilePathData.CostFromStart = 0;
	TilePathData.MinCostToTarget = GetMinCostBetweenTwoTiles(Start, Target, bIncludeDiagonals);
	DiscoverTile(TilePathData);

	while (DiscoveredTileIndexes.Num() > 0)
	{
		if (AnalyseNextDiscoveredTile())
		{
			return GeneratePath();
		}
	}

	return Res;
}

TArray<FIntPoint> AGridPathfinding::GetNeighborIndexesForSquare(FIntPoint Index, bool bIncludeDiagonals) const
{
	check(Grid);
	TArray<FIntPoint> Res;
	Res.Add({Index.X, Index.Y - 1});
	Res.Add({Index.X, Index.Y + 1});
	Res.Add({Index.X - 1, Index.Y});
	Res.Add({Index.X + 1, Index.Y});
	if (bIncludeDiagonals)
	{
		Res.Add({Index.X + 1, Index.Y + 1});
		Res.Add({Index.X + 1, Index.Y - 1});
		Res.Add({Index.X - 1, Index.Y + 1});
		Res.Add({Index.X - 1, Index.Y - 1});
	}
	return Res;
}

TArray<FIntPoint> AGridPathfinding::GetNeighborIndexesForHexagon(FIntPoint Index) const
{
	TArray<FIntPoint> Res;

	Res.Add({Index.X, Index.Y - 2});
	Res.Add({Index.X, Index.Y + 2});
	Res.Add({Index.X + 1, Index.Y + 1});
	Res.Add({Index.X - 1, Index.Y - 1});
	Res.Add({Index.X + 1, Index.Y - 1});
	Res.Add({Index.X - 1, Index.Y + 1});

	return Res;
}

TArray<FIntPoint> AGridPathfinding::GetNeighborIndexesForTriangle(FIntPoint Index, bool bIncludeDiagonals) const
{
	TArray<FIntPoint> Res;
	Res.Add({Index.X, Index.Y - 1});
	Res.Add({Index.X, Index.Y + 1});
	if (Index.X % 2 && Index.Y % 2)
	{
		// Down
		Res.Add({Index.X - 1, Index.Y});
	}
	else
	{
		// Up
		Res.Add({Index.X + 1, Index.Y});
	}

	if (bIncludeDiagonals)
	{
		if (Index.X % 2 && Index.Y % 2)
		{
			Res.Add({Index.X + 1, Index.Y});
			Res.Add({Index.X - 1, Index.Y - 2});
			Res.Add({Index.X - 1, Index.Y + 2});
		}
		else
		{
			Res.Add({Index.X - 1, Index.Y});
			Res.Add({Index.X + 1, Index.Y - 2});
			Res.Add({Index.X + 1, Index.Y + 2});
		}
	}
	return Res;
}

bool AGridPathfinding::IsInputDataValid() const
{
	return StartIndex != TargetIndex && Grid->IsWalkableByIndex(StartIndex) && Grid->IsWalkableByIndex(TargetIndex);
}

void AGridPathfinding::DiscoverTile(FPathfindingData TilePathData)
{
	PathfindingData.Add(TilePathData.Index, TilePathData);
	InsertTileInDiscoveredArray(TilePathData);
	OnPathfindingDataUpdated.Broadcast(TilePathData.Index);
}

int32 AGridPathfinding::GetMinCostBetweenTwoTiles(FIntPoint Index1, FIntPoint Index2, bool bIncludeDiagonals) const
{
	FIntPoint Index = Index1 - Index2;
	if (bIncludeDiagonals)
	{
		return FMath::Max(FMath::Abs(Index.X), FMath::Abs(Index.Y));
	}

	return FMath::Abs(Index.X) + FMath::Abs(Index.Y);
}

int32 AGridPathfinding::GetTileSortingCost(FPathfindingData TilePathData) const
{
	int32 SortingCost = TilePathData.CostFromStart + TilePathData.MinCostToTarget;
	const bool bIsDiagonals = IsDiagonals(TilePathData.Index, TilePathData.PrevIndex);
	return SortingCost * 2 + bIsDiagonals ? 1 : 0;
}

bool AGridPathfinding::AnalyseNextDiscoveredTile()
{
	CurrentDiscoveredTile = PullCheapestTileOutOfDiscoveredList();
	CurrentNeighbors = GetValidTileNeighbors(CurrentDiscoveredTile.Index, bFindingIncludeDiagonals);

	while (CurrentNeighbors.Num() > 0)
	{
		if (DiscoverNextNeighbor())
		{
			return true;
		}
	}
	return false;
}

TArray<FIntPoint> AGridPathfinding::GeneratePath()
{
	TArray<FIntPoint> Res;
	// 从后向前找
	FIntPoint Current = TargetIndex;
	while (Current != StartIndex)
	{
		if (PathfindingData.Contains(Current))
		{
			Res.Add(PathfindingData[Current].Index);
			Current = PathfindingData[Current].PrevIndex;
		}
	}
	Algo::Reverse(Res);
	return Res;
}

FPathfindingData AGridPathfinding::PullCheapestTileOutOfDiscoveredList()
{
	if (DiscoveredTileIndexes.Num() < 1 || DiscoveredTileSortingCosts.Num() < 1)
	{
		return FPathfindingData();
	}

	// 花费最小的一直是第一个
	FIntPoint TileIndex = DiscoveredTileIndexes[0];
	DiscoveredTileIndexes.RemoveAt(0);
	DiscoveredTileSortingCosts.RemoveAt(0);

	AnalysedTileIndexes.Add(TileIndex);
	return PathfindingData[TileIndex];
}

bool AGridPathfinding::DiscoverNextNeighbor()
{
	CurrentNeighbor = CurrentNeighbors[0];
	CurrentNeighbors.RemoveAt(0);
	if (!AnalysedTileIndexes.Contains(CurrentNeighbor.Index))
	{
		int32 CostFromStart = CurrentDiscoveredTile.CostFromStart + CurrentNeighbor.CostToEnterTile;
		int32 IndexInDiscovered = DiscoveredTileIndexes.Find(CurrentNeighbor.Index);
		// 已经发现了该路径
		if (IndexInDiscovered != -1)
		{
			CurrentNeighbor = PathfindingData[CurrentNeighbor.Index];
			// 找到最佳路径
			if (CostFromStart < CurrentNeighbor.CostFromStart)
			{
				DiscoveredTileIndexes.RemoveAt(IndexInDiscovered);
				DiscoveredTileSortingCosts.RemoveAt(IndexInDiscovered);
			}
			else
			{
				return false;
			}
		}

		FPathfindingData Data;
		Data.Index = CurrentNeighbor.Index;
		Data.CostToEnterTile = CurrentNeighbor.CostToEnterTile;
		Data.CostFromStart = CostFromStart;
		Data.MinCostToTarget = GetMinCostBetweenTwoTiles(CurrentNeighbor.Index, TargetIndex, bFindingIncludeDiagonals);
		Data.PrevIndex = CurrentNeighbor.PrevIndex;
		DiscoverTile(Data);

		return CurrentNeighbor.Index == TargetIndex;
	}
	return false;
}

void AGridPathfinding::InsertTileInDiscoveredArray(FPathfindingData TileData)
{
	// Cost越大靠前
	int32 SortingCost = GetTileSortingCost(TileData);
	if (DiscoveredTileSortingCosts.Num() == 0)
	{
		DiscoveredTileSortingCosts.Add(SortingCost);
		DiscoveredTileIndexes.Add(TileData.Index);
	}
	else
	{
		if (SortingCost >= DiscoveredTileSortingCosts[DiscoveredTileSortingCosts.Num() - 1])
		{
			DiscoveredTileSortingCosts.Add(SortingCost);
			DiscoveredTileIndexes.Add(TileData.Index);
		}
		else
		{
			for (int32 I = 0; I < DiscoveredTileSortingCosts.Num(); I++)
			{
				int32 Cost = DiscoveredTileSortingCosts[I];
				if (Cost >= SortingCost)
				{
					DiscoveredTileSortingCosts.Insert(SortingCost, I);
					DiscoveredTileIndexes.Insert(TileData.Index, I);
					break;
				}
			}
		}
	}
}

void AGridPathfinding::ResetPathfindingInfo()
{
	PathfindingData.Empty();
	DiscoveredTileIndexes.Empty();
	DiscoveredTileSortingCosts.Empty();
	AnalysedTileIndexes.Empty();
	OnPathfindingDataCleared.Broadcast();
}
