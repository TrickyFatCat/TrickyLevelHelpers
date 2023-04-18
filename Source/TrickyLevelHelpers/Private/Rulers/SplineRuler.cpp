// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Rulers/SplineRuler.h"

#include "Components/DebugTextComponent.h"
#include "LevelHelpersLibrary.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ASplineRuler::ASplineRuler()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bIsEditorOnlyActor = true;

#if WITH_EDITORONLY_DATA

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SetRootComponent(ToRawPtr(SplineComponent));
	SplineComponent->EditorUnselectedSplineSegmentColor = SplineColor;
	SplineComponent->EditorSelectedSplineSegmentColor = FLinearColor::Yellow;
	SplineComponent->EditorTangentColor = FLinearColor::Green;
	SplineComponent->bIsEditorOnly = true;

	for (int32 i = 0; i < 2; ++i)
	{
		SplineComponent->SetSplinePointType(i, ESplinePointType::Linear, true);
	}

	auto CreateDebugText = [&](TObjectPtr<UDebugTextComponent>& DebugText, const FName& Name) -> void
	{
		DebugText = CreateDefaultSubobject<UDebugTextComponent>(Name);
		DebugText->SetupAttachment(GetRootComponent());
		DebugText->SetDrawOneLabel(false);
		DebugText->bIsEditorOnly = true;
	};

	CreateDebugText(DistanceDebugText, "DistanceDebug");
	CreateDebugText(SectorDebugText, "SectorsDebug");

#endif
}

void ASplineRuler::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA

	bIsEditorOnlyActor = !bDrawInGame;
	
	SplineComponent->SetUnselectedSplineSegmentColor(SplineColor);
	SplineComponent->SetClosedLoop(bIsLooped);
	SplineComponent->bIsEditorOnly = !bDrawInGame;
	SplineComponent->bDrawDebug = bDrawInGame;

	DistanceDebugText->bIsEditorOnly = !bDrawInGame;
	DistanceDebugText->bDrawInGame = bDrawInGame;
	DistanceDebugText->bDrawDebug = bShowDistance;
	ULevelHelpersLibrary::UpdateSplinePointsDebugDistance(SplineComponent,
	                                                      DistanceDebugText,
	                                                      DistanceTextColor,
	                                                      DistanceTextScale,
	                                                      bShowTravelTime,
	                                                      Speed);

	SectorDebugText->bIsEditorOnly = !bDrawInGame;
	SectorDebugText->bDrawInGame = bDrawInGame;
	SectorDebugText->bDrawDebug = bShowSectors;
	ULevelHelpersLibrary::UpdateSplineSectorsDebugLength(SplineComponent,
	                                                     SectorDebugText,
	                                                     SectorsTextColor,
	                                                     SectorsTextScale,
	                                                     bShowTravelTime,
	                                                     Speed);

#endif
}

void ASplineRuler::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITORONLY_DATA
	if (bDrawInGame)
	{
		const FString Command = "show Splines";
		UKismetSystemLibrary::ExecuteConsoleCommand(this, Command);
	}
#endif
}
