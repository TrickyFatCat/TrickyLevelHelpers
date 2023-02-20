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

void ULevelHelpersLibrary::CalculateCylinderLocations(TArray<FVector>& Locations,
                                                      const int32 PointsNumber,
                                                      const int32 RingsAmount,
                                                      const float Radius,
                                                      const float Angle,
                                                      const FVector& RingOffset,
                                                      const FVector& Centre)
{
	if (PointsNumber <= 0 || RingsAmount <= 0)
	{
		return;
	}

	Locations.Empty();

	for (int32 i = 0; i < RingsAmount; ++i)
	{
		TArray<FVector> RingLocations;

		CalculateRingLocations(RingLocations,
		                       PointsNumber,
		                       Radius,
		                       Angle,
		                       Centre + RingOffset * i);

		Locations.Append(RingLocations);
	}
}

void ULevelHelpersLibrary::CalculateSphereLocations(TArray<FVector>& Locations,
                                                    const int32 PointsNumber,
                                                    const float Radius,
                                                    const float MinLatitude,
                                                    const float MaxLatitude,
                                                    const float MinLongitude,
                                                    const float MaxLongitude,
                                                    const FVector& Centre)
{
	if (PointsNumber <= 0)
	{
		return;
	}

	Locations.Empty();

	const float Phi = PI * (3.f - FMath::Sqrt(5.f));
	const float Tau = PI * 2.f;

	const float MinLongitudeAngle = MinLongitude * 360.f;
	const float MaxLongitudeAngle = MaxLongitude * 360.f;
	const float MinLongitudeRad = FMath::DegreesToRadians(MinLongitudeAngle);
	const float MaxLongitudeRad = FMath::DegreesToRadians(MaxLongitudeAngle);

	for (int32 i = 0; i < PointsNumber; ++i)
	{
		const float Z = ((i / (static_cast<float>(PointsNumber) - 1.f)) * (MaxLatitude - MinLatitude) + MinLatitude) *
			2.f - 1.f;
		const float RadiusZ = FMath::Sqrt(1.f - Z * Z);
		float Theta = Phi * static_cast<float>(i);

		if (MinLongitudeAngle != 0.f || MaxLongitudeAngle != 360.f)
		{
			Theta = FMath::Fmod(Theta, Tau);
			Theta = Theta < 0 ? Theta + Tau : Theta;
			Theta = Theta * MaxLongitudeRad / Tau + MinLongitudeRad;
		}

		const float X = FMath::Sin(Theta) * RadiusZ;
		const float Y = FMath::Cos(Theta) * RadiusZ;

		FVector Location{X, Y, Z};
		Location *= Radius;
		Location += Centre;
		Locations.Emplace(Location);
	}
}

void ULevelHelpersLibrary::CalculateSplineLocations(const USplineComponent* SplineComponent,
                                                    TArray<FVector>& Locations,
                                                    const int32 PointsAmount,
                                                    const FVector& LocationOffset)
{
	if (!SplineComponent || PointsAmount <= 0)
	{
		return;
	}

	if (Locations.Num() > 0)
	{
		Locations.Empty();
	}

	FVector Location{FVector::ZeroVector};
	const float Offset = SplineComponent->GetSplineLength() / static_cast<float>(PointsAmount);

	for (int32 i = 0; i < PointsAmount; ++i)
	{
		const float Distance = Offset * i + Offset * 0.5f * !SplineComponent->IsClosedLoop();
		Location = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
		Location += LocationOffset;
		Locations.Emplace(Location);
	}
}

void ULevelHelpersLibrary::CalculateSplineTransforms(const USplineComponent* SplineComponent,
                                                     TArray<FTransform>& Transforms,
                                                     const int32 PointsAmount,
                                                     const FVector& LocationOffset,
                                                     const bool bGenerateAtPoints)
{
	if (!SplineComponent || PointsAmount <= 0)
	{
		return;
	}

	if (Transforms.Num() > 0)
	{
		Transforms.Empty();
	}

	const float Offset = SplineComponent->GetSplineLength() / static_cast<float>(PointsAmount);
	FVector Location{FVector::ZeroVector};
	FRotator Rotation{FRotator::ZeroRotator};
	FVector Scale{FVector::OneVector};

	for (int32 i = 0; i < PointsAmount; ++i)
	{
		if (bGenerateAtPoints)
		{
			Location = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			Rotation = SplineComponent->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			Scale = SplineComponent->GetScaleAtSplinePoint(i);
		}
		else
		{
			const float Distance = Offset * i + Offset * 0.5f * !SplineComponent->IsClosedLoop();
			Location = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
			Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
			Scale = SplineComponent->GetScaleAtDistanceAlongSpline(Distance);
		}

		Location += LocationOffset;
		Transforms.Emplace(FTransform{Rotation.Quaternion(), Location, Scale});
	}
}

void ULevelHelpersLibrary::GetRotatorFromMode(FRotator& Rotation, const ERotationMode RotationMode)
{
	switch (RotationMode)
	{
	case ERotationMode::Forward:
		Rotation = FVector::ForwardVector.Rotation();
		break;

	case ERotationMode::Backward:
		Rotation = (FVector::ForwardVector * -1).Rotation();
		break;

	case ERotationMode::Up:
		Rotation = FVector::UpVector.Rotation();
		break;

	case ERotationMode::Down:
		Rotation = (FVector::UpVector * -1).Rotation();
		break;

	case ERotationMode::Left:
		Rotation = (FVector::RightVector * -1).Rotation();
		break;

	case ERotationMode::Right:
		Rotation = FVector::RightVector.Rotation();
		break;
	}
}
