﻿// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "SplineMeasurer.h"

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

	auto CreateText = [&](TextObjPtr& TextRender, const FName& Name) -> void
	{
		TextRender = CreateDefaultSubobject<UTextRenderComponent>(Name);
		TextRender->TextRenderColor = FColor::Magenta;
		TextRender->SetupAttachment(GetRootComponent());
	};

	CreateText(MeasurementText, "EndText");
	CreateText(CustomMeasurementText, "CustomText");
	CustomMeasurementText->SetVisibility(bShowCustomMeasurementText);

#endif
}

void ASplineMeasurer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA

	const int32 LastSplinePoint = SplineComponent->GetNumberOfSplinePoints();
	const int32 LastPointIndex = SplineComponent->IsClosedLoop() ? LastSplinePoint : LastSplinePoint - 1;

	auto UpdateText = [&](const TextObjPtr& TextComponent, const int32 PointIndex)-> void
	{
		TextComponent->SetRelativeLocation(
			SplineComponent->GetLocationAtSplinePoint(PointIndex, ESplineCoordinateSpace::Local));
		const float Distance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(PointIndex);
		const FString InfoText = FString::Printf(
			TEXT("Units: %d\nMeters: %.2f"), static_cast<int32>(Distance), Distance / 100.f);
		TextComponent->SetText(FText::FromString(InfoText));
	};

	UpdateText(MeasurementText, LastPointIndex);

	CustomPointIndex = CustomPointIndex >= LastPointIndex ? LastPointIndex - 1 : CustomPointIndex;
	UpdateText(CustomMeasurementText, CustomPointIndex);
	CustomMeasurementText->SetVisibility(bShowCustomMeasurementText);
	
#endif
}