// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Rulers/SplineRuler.h"

#include "Components/DebugTextComponent.h"
#include "LevelHelpersLibrary.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ASplineRuler::ASplineRuler()
{
	bIsEditorOnlyActor = true;

#if WITH_EDITORONLY_DATA
	PrimaryActorTick.bCanEverTick = true;
	
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
		DebugText = CreateEditorOnlyDefaultSubobject<UDebugTextComponent>(Name);
		DebugText->SetupAttachment(GetRootComponent());
		DebugText->SetDrawOneLabel(false);
		DebugText->bIsEditorOnly = true;
	};

	CreateDebugText(DistanceDebug, "DistanceDebug");
	CreateDebugText(SectorsDebug, "SectorsDebug");
#else
	PrimaryActorTick.bCanEverTick = false;
#endif
}

bool ASplineRuler::ShouldTickIfViewportsOnly() const
{
#if WITH_EDITORONLY_DATA
	return true;
#else
	return Super::ShouldTickIfViewportsOnly();
#endif
}

void ASplineRuler::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if WITH_EDITORONLY_DATA
	const int32 LastSplinePoint = SplineComponent->GetNumberOfSplinePoints();
	const int32 LastPointIndex = SplineComponent->IsClosedLoop() ? LastSplinePoint : LastSplinePoint - 1;
	const FColor Color = SplineColor.ToFColor(true);

	for (int32 i = 0; i < LastPointIndex; ++i)
	{
		const FVector Location = SplineComponent->GetLocationAtSplineInputKey(i + 0.5f, ESplineCoordinateSpace::World);
		const FRotator Rotation = SplineComponent->GetRotationAtSplineInputKey(i + 0.5f, ESplineCoordinateSpace::World);

		DrawDebugCrosshairs(SplineComponent->GetWorld(),
		                    Location,
		                    Rotation,
		                    40.f,
		                    Color,
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
	SplineComponent->bIsEditorOnly = !bDrawInGame;

	DistanceDebug->SetDrawInGame(bDrawInGame);
	DistanceDebug->SetDrawDebug(bShowDistance);
	ULevelHelpersLibrary::UpdateSplinePointsDebugDistance(SplineComponent,
	                                                      DistanceDebug,
	                                                      DistanceTextColor,
	                                                      1.15f,
	                                                      bShowTravelTime,
	                                                      Speed);

	SectorsDebug->SetDrawInGame(bDrawInGame);
	SectorsDebug->SetDrawDebug(bShowSectors);
	ULevelHelpersLibrary::UpdateSplineSectorsDebugLength(SplineComponent,
	                                                     SectorsDebug,
	                                                     SectorsTextColor,
	                                                     1.0f,
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
		SplineComponent->bDrawDebug = bDrawInGame;
		const FString Command = "show Splines";
		UKismetSystemLibrary::ExecuteConsoleCommand(this, Command);
	}
#endif
}
