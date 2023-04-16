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

	UPROPERTY(EditAnywhere, Category="SplineMeasurer")
	FLinearColor TextColor{FColor::Red};

	UPROPERTY(EditAnywhere, Category="SplineMeasurer")
	float TextScale = 1.f;
};
