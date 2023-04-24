// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Generators/ActorsGeneratorSpline.h"

#include "LevelHelpersLibrary.h"
#include "Components/DebugTextComponent.h"
#include "Components/SplineComponent.h"

AActorsGeneratorSpline::AActorsGeneratorSpline()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SplineComponent->SetComponentTickEnabled(false);
	SetRootComponent(SplineComponent);

#if WITH_EDITORONLY_DATA

	PointsDebug = CreateEditorOnlyDefaultSubobject<UDebugTextComponent>("PointsDebug");

	if (PointsDebug)
	{
		PointsDebug->SetupAttachment(GetRootComponent());
		PointsDebug->SetDrawOneLabel(false);
	}

	SectorsDebug = CreateEditorOnlyDefaultSubobject<UDebugTextComponent>("SectorsDebug");

	if (SectorsDebug)
	{
		SectorsDebug->SetupAttachment(GetRootComponent());
		SectorsDebug->SetDrawOneLabel(false);
	}
#endif
}

void AActorsGeneratorSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA

	if (PointsDebug)
	{
		PointsDebug->SetDrawInGame(bShowDebugInGame);
		PointsDebug->SetDrawDebug(bShowPointsDebug);
		ULevelHelpersLibrary::UpdateSplinePointsDebugDistance(SplineComponent,
		                                                      PointsDebug,
		                                                      PointsDebugColor,
		                                                      1.15f,
		                                                      bShowTravelTime,
		                                                      TravelSpeed);
	}

	if (SectorsDebug)
	{
		SectorsDebug->SetDrawInGame(bShowDebugInGame);
		SectorsDebug->SetDrawDebug(bShowSectorsDebug);
		ULevelHelpersLibrary::UpdateSplineSectorsDebugLength(SplineComponent,
		                                                     SectorsDebug,
		                                                     SectorsDebugColor,
		                                                     1.0f,
		                                                     bShowTravelTime,
		                                                     TravelSpeed);
	}
#endif
}

void AActorsGeneratorSpline::GenerateActors()
{
#if WITH_EDITORONLY_DATA

	switch (GenerationMode)
	{
	case ESplineGenerationMode::Points:
		PointsAmount = SplineComponent->GetNumberOfSplinePoints();
		break;

	case ESplineGenerationMode::Number:
		PointsAmount = ActorsAmount;
		break;

	case ESplineGenerationMode::Spacing:
		PointsAmount = FMath::Floor(SplineComponent->GetSplineLength() / Spacing);
	}

	if (!ActorClass || PointsAmount <= 0)
	{
		return;
	}

	ULevelHelpersLibrary::CalculateSplineTransforms(SplineComponent,
	                                                Transforms,
	                                                PointsAmount,
	                                                LocationOffset,
	                                                GenerationMode == ESplineGenerationMode::Points);

	Locations.Empty();

	for (auto& Transform : Transforms)
	{
		Locations.Emplace(Transform.GetLocation());
	}

	if (Actors.Num() == PointsAmount)
	{
		ChangeActorsTransform();
		return;
	}

	ClearActors();
	SpawnActors();

#endif
}

void AActorsGeneratorSpline::CalculateCustomRotation(const FVector& Location, FRotator& Rotation) const
{
	Super::CalculateCustomRotation(Location, Rotation);
	const FRotator SplineRotation = Transforms[Locations.Find(Location)].GetRotation().Rotator();

	Rotation.Roll = RotateAlongSpline.bX ? SplineRotation.Roll : Rotation.Roll;
	Rotation.Pitch = RotateAlongSpline.bY ? SplineRotation.Pitch : Rotation.Pitch;
	Rotation.Yaw = RotateAlongSpline.bZ ? SplineRotation.Yaw : Rotation.Yaw;
}
