// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineMeasurer.generated.h"

class USplineComponent;
class UTextRenderComponent;
class UDebugTextComponent;

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

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> DebugText = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category="Component")
	TObjectPtr<UTextRenderComponent> MeasurementText = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	TObjectPtr<UTextRenderComponent> CustomMeasurementText = nullptr;

	UPROPERTY(EditAnywhere, Category="SplineMeasurer")
	bool bShowCustomMeasurementText = false;

	UPROPERTY(EditAnywhere, Category="SplineMeasurer", meta=(EditCondition="bShowCustomMeasurementText", ClampMin = "1"))
	int32 CustomPointIndex = 1;
};
