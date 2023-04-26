// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineRuler.generated.h"

class USplineComponent;
class UDebugTextComponent;

/**
 * Measures distance from first point to others and length of sectors between points.
 */
UCLASS(Blueprintable, HideCategories=(Tick, Events, Actor, HLOD, Replication, Input, Cooking, Collision, Physics, Rendering, WorldPartition, DataLayers,Networking))
class TRICKYLEVELHELPERS_API ASplineRuler : public AActor
{
	GENERATED_BODY()

public:
	ASplineRuler();

protected:
	virtual bool ShouldTickIfViewportsOnly() const override;
	
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	TObjectPtr<USplineComponent> SplineComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> PointsDebug = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> SectorsDebug = nullptr;

	UPROPERTY(EditAnywhere, Category="SplineMeasurer", meta=(InlineEditConditionToggle))
	bool bShowDistance = true;

	UPROPERTY(EditAnywhere, Category="SplineMeasurer", meta=(EditCondition="bShowDistance", HideAlphaChannel, DisplayName="Points"))
	FLinearColor DistanceColor{FColor::Red};

	UPROPERTY(EditAnywhere, Category="SplineMeasurer", meta=(InlineEditConditionToggle))
	bool bShowSectors = false;
	
	UPROPERTY(EditAnywhere, Category="SplineMeasurer", meta=(EditCondition="bShowSectors", HideAlphaChannel, DisplayName="Sectors"))
	FLinearColor SectorsColor{FColor::Yellow};

	UPROPERTY(EditAnywhere, Category="SplineMeasurer", meta=(InlineEditConditionToggle))
	bool bShowTravelTime = true;

	UPROPERTY(EditAnywhere, Category="SplineMeasurer", meta=(EditCondition="bShowTravelTime", ClampMin="1"))
	float Speed = 1000;
	
	UPROPERTY(EditAnywhere, Category="SplineMeasurer")
	bool bIsLooped = false;

	UPROPERTY(EditAnywhere, Category="SplineMeasurer")
	FColor SplineColor{FColor::Magenta};
	
	UPROPERTY(EditAnywhere, Category="SplineMeasurer")
	bool bDrawInGame = false;
};
