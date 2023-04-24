﻿// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Rulers/SplineRuler.h"

#include "Components/DebugTextComponent.h"
#include "LevelHelpersLibrary.h"
#include "Components/BillboardComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ASplineRuler::ASplineRuler()
{
	bIsEditorOnlyActor = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SetRootComponent(ToRawPtr(SplineComponent));
	SplineComponent->EditorUnselectedSplineSegmentColor = SplineColor;
	SplineComponent->EditorSelectedSplineSegmentColor = FLinearColor::Yellow;
	SplineComponent->EditorTangentColor = FLinearColor::Green;

	for (int32 i = 0; i < 2; ++i)
	{
		SplineComponent->SetSplinePointType(i, ESplinePointType::Linear, true);
	}


#if WITH_EDITORONLY_DATA
	PrimaryActorTick.bCanEverTick = true;
	
	PointsDebug = CreateEditorOnlyDefaultSubobject<UDebugTextComponent>("PointsDebug");
	PointsDebug->SetupAttachment(GetRootComponent());
	PointsDebug->SetDrawOneLabel(false);

	SectorsDebug = CreateEditorOnlyDefaultSubobject<UDebugTextComponent>("SectorsDebug");
	SectorsDebug->SetupAttachment(GetRootComponent());
	SectorsDebug->SetDrawOneLabel(false);
#else
	PrimaryActorTick.bCanEverTick = false;
#endif
}

bool ASplineRuler::ShouldTickIfViewportsOnly() const
{
	return true;
}

void ASplineRuler::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if WITH_EDITORONLY_DATA

	const int32 LastSplinePoint = SplineComponent->GetNumberOfSplinePoints();
	const int32 LastPointIndex = SplineComponent->IsClosedLoop() ? LastSplinePoint : LastSplinePoint - 1;

	for (int32 i = 0; i < LastPointIndex; ++i)
	{
		const FVector Location = SplineComponent->GetLocationAtSplineInputKey(i + 0.5f, ESplineCoordinateSpace::World);
		const FRotator Rotation = SplineComponent->GetRotationAtSplineInputKey(i + 0.5f, ESplineCoordinateSpace::World);

		DrawDebugCrosshairs(GetWorld(),
		                    Location,
		                    Rotation,
		                    40.f,
		                    SplineColor,
		                    false,
		                    -1,
		                    0);
	}

#endif
}

void ASplineRuler::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA

	bIsEditorOnlyActor = !bDrawInGame;

	SplineComponent->SetUnselectedSplineSegmentColor(SplineColor);
	SplineComponent->SetClosedLoop(bIsLooped);

	if (PointsDebug)
	{
		PointsDebug->SetDrawInGame(bDrawInGame);
		PointsDebug->SetDrawDebug(bShowDistance);
		ULevelHelpersLibrary::UpdateSplinePointsDebugDistance(SplineComponent,
		                                                      PointsDebug,
		                                                      DistanceColor,
		                                                      1.15f,
		                                                      bShowTravelTime,
		                                                      Speed);
	}

	if (SectorsDebug)
	{
		SectorsDebug->SetDrawInGame(bDrawInGame);
		SectorsDebug->SetDrawDebug(bShowSectors);
		ULevelHelpersLibrary::UpdateSplineSectorsDebugLength(SplineComponent,
		                                                     SectorsDebug,
		                                                     SectorsColor,
		                                                     1.15f,
		                                                     bShowTravelTime,
		                                                     Speed);
	}

#endif
}

void ASplineRuler::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITORONLY_DATA
	if (bDrawInGame)
	{
		SplineComponent->bDrawDebug = bDrawInGame;
		const FString Command = "show Splines";
		UKismetSystemLibrary::ExecuteConsoleCommand(this, Command);
	}
#endif
}
