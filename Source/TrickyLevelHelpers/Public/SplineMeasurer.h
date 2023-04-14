﻿// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineMeasurer.generated.h"

class USplineComponent;
class UTextRenderComponent;

UCLASS(Blueprintable)
class TRICKYLEVELHELPERS_API ASplineMeasurer : public AActor
{
	GENERATED_BODY()

public:
	ASplineMeasurer();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	TObjectPtr<USplineComponent> SplineComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category="Component")
	TObjectPtr<UTextRenderComponent> MeasurementText = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	TObjectPtr<UTextRenderComponent> CustomMeasurementText = nullptr;

	UPROPERTY(EditAnywhere)
	bool bShowCustomMeasurementText = false;

	UPROPERTY(EditAnywhere, meta=(EditCondition="bShowCustomMeasurementText", ClampMin = "1"))
	int32 CustomPointIndex = 1;
};