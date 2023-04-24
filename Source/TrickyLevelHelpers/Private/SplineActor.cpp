// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "SplineActor.h"

#include "Components/DebugTextComponent.h"
#include "Components/SplineComponent.h"
#include "LevelHelpersLibrary.h"

ASplineActor::ASplineActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SetRootComponent(ToRawPtr(SplineComponent));

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

void ASplineActor::OnConstruction(const FTransform& Transform)
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
		                                                     1.15f,
		                                                     bShowTravelTime,
		                                                     TravelSpeed);
	}
#endif
}
