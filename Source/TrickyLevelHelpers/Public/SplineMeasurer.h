// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

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
	virtual bool ShouldTickIfViewportsOnly() const override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	TObjectPtr<USplineComponent> SplineComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category="Component")
	TObjectPtr<UTextRenderComponent> MeasurementText = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	TObjectPtr<UTextRenderComponent> CustomMeasurementText = nullptr;

	UPROPERTY(EditAnywhere)
	bool bShowCustomMeasurementText = false;

	UPROPERTY(EditAnywhere, meta=(EditCondition="bShowCustomMeasurementText", ClampMin = "0"))
	int32 CustomPointIndex = 1;
};
