// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "LevelHelpersLibrary.h"

void ULevelHelpersLibrary::CalculateGridLocations(TArray<FVector>& Locations,
                                                  const FGridSize& GridSize,
                                                  const FVector2D& SectorSize,
                                                  const FVector& GridCentre)
{
	if (Locations.Num() > 0)
	{
		Locations.Empty();
	}

	FVector Location{GridCentre};

	for (int32 x = 0; x < GridSize.X; ++x)
	{
		for (int32 y = 0; y < GridSize.Y; ++y)
		{
			Location.X = x * SectorSize.X + GridCentre.X;
			Location.Y = y * SectorSize.Y + GridCentre.Y;
			Locations.Emplace(Location);
		}
	}
}

void ULevelHelpersLibrary::CalculateCubeLocations(TArray<FVector>& Locations,
                                                  const FCubeSize& CubeSize,
                                                  const FVector& SectorSize,
                                                  const FVector& CubeCentre)
{
	if (Locations.Num() > 0)
	{
		Locations.Empty();
	}

	FVector Location{FVector::ZeroVector};

	for (int32 x = 0; x < CubeSize.X; ++x)
	{
		for (int32 y = 0; y < CubeSize.Y; ++y)
		{
			for (int32 z = 0; z < CubeSize.Z; ++z)
			{
				Location.X = x * SectorSize.X;
				Location.Y = y * SectorSize.Y;
				Location.Z = z * SectorSize.Z;
				Location += CubeCentre;
				Locations.Emplace(Location);
			}
		}
	}
}
