// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "SplineMeasurer.h"

#include "Components/DebugTextComponent.h"
#include "LevelHelpersLibrary.h"
#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"

using TextObjPtr = TObjectPtr<UTextRenderComponent>;

ASplineMeasurer::ASplineMeasurer()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bIsEditorOnlyActor = true;

#if WITH_EDITORONLY_DATA

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SetRootComponent(ToRawPtr(SplineComponent));
	SplineComponent->EditorUnselectedSplineSegmentColor = FLinearColor::Red;
	SplineComponent->EditorSelectedSplineSegmentColor = FLinearColor::Yellow;
	SplineComponent->EditorTangentColor = FLinearColor::Green;

	auto CreateDebugText = [&](TObjectPtr<UDebugTextComponent>& DebugText, const FName& Name) -> void
	{
		DebugText = CreateDefaultSubobject<UDebugTextComponent>(Name);
		DebugText->SetupAttachment(GetRootComponent());
		DebugText->SetDrawOneLabel(false);
	};

	CreateDebugText(DistanceDebugText, "DistanceDebug");
	CreateDebugText(SectorDebugText, "SectorsDebug");

#endif
}

void ASplineMeasurer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA

	DistanceDebugText->bDrawDebug = bShowDistance;
	ULevelHelpersLibrary::UpdateSplinePointsDebugDistance(SplineComponent,
	                                                      DistanceDebugText,
	                                                      DistanceTextColor,
	                                                      DistanceTextScale);

	SectorDebugText->bDrawDebug = bShowSectors;
	ULevelHelpersLibrary::UpdateSplineSectorsDebugLength(SplineComponent,
	                                                     SectorDebugText,
	                                                     SectorsTextColor,
	                                                     SectorsTextScale);

#endif
}
