// Copyright BanMing

#include "Grid/GridPathfinding.h"

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

TArray<FIntPoint> AGridPathfinding::GetValidTileNeihbors(FIntPoint Index, bool bIncludeDiagonals) const
{
	check(Grid);
	FTileData InputData = Grid->GetTileDatafromGridIndex(Index);
	TArray<FIntPoint> Neihbors = GetTileNeihbors(Index, bIncludeDiagonals);
	TArray<FIntPoint> Res;
	for (const FIntPoint& I : Neihbors)
	{
		const FTileData CurData = Grid->GetTileDatafromGridIndex(Index);
		const float ZOffset = FMath::Abs(CurData.Transform.GetLocation().Z - InputData.Transform.GetLocation().Z);
		if (Grid->IsWalkableByIndex(I) && ZOffset <= Grid->TileSize.Z)
		{
			Res.Add(I);
		}
	}
	return Res;
}

TArray<FIntPoint> AGridPathfinding::GetTileNeihbors(FIntPoint Index, bool bIncludeDiagonals) const
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
