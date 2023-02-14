// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "LevelHelpersLibrary.h"

void ULevelHelpersLibrary::CalculateGridLocations(TArray<FVector>& Locations,
                                                  const FGridSize& GridSize,
                                                  const FVector2D& SectorSize,
                                                  const FVector& Centre)
{
	if (Locations.Num() > 0)
	{
		Locations.Empty();
	}

	FVector Location{Centre};

	for (int32 x = 0; x < GridSize.X; ++x)
	{
		for (int32 y = 0; y < GridSize.Y; ++y)
		{
			Location.X = x * SectorSize.X + Centre.X;
			Location.Y = y * SectorSize.Y + Centre.Y;
			Locations.Emplace(Location);
		}
	}
}

void ULevelHelpersLibrary::CalculateCubeLocations(TArray<FVector>& Locations,
                                                  const FCubeSize& CubeSize,
                                                  const FVector& SectorSize,
                                                  const FVector& Centre)
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
				Location += Centre;
				Locations.Emplace(Location);
			}
		}
	}
}

void ULevelHelpersLibrary::CalculateRingLocations(TArray<FVector>& Locations,
                                                 const int32 PointsNumber,
                                                 const float Radius,
                                                 const float Angle,
                                                 const FVector& Centre)
{
	if (PointsNumber <= 0)
	{
		return;
	}
	
	if (Locations.Num() > 0)
	{
		Locations.Empty();
	}

	FVector Location{Centre};
	const float Theta = FMath::DegreesToRadians(Angle / PointsNumber);

	for (int32 i = 0; i < PointsNumber; ++i)
	{
		Location.X = Radius * FMath::Cos(Theta * i) + Centre.X;
		Location.Y = Radius * FMath::Sin(Theta * i) + Centre.Y;
		Locations.Emplace(Location);
	}
}