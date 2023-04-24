// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineRuler.generated.h"

class USplineComponent;
class UDebugTextComponent;

UCLASS(Blueprintable, HideCategories=(AssetUserData, Collision, Cooking, HLOD, Lighting, LOD, Mobile, Navigation, Physics, RayTracing, Rendering, Tags, TextureStreaming, Actor, Replication, Input, Networking, WorldPartition, DataLayers))
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

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	TObjectPtr<USplineComponent> SplineComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> PointsDebug = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> SectorsDebug = nullptr;

	UPROPERTY(EditAnywhere, Category="SplineMeasurer", meta=(InlineEditConditionToggle))
	bool bShowDistance = true;

	UPROPERTY(EditAnywhere, Category="SplineMeasurer", meta=(EditCondition="bShowDistance"))
	FLinearColor DistanceColor{FColor::Red};

	UPROPERTY(EditAnywhere, Category="SplineMeasurer", meta=(InlineEditConditionToggle))
	bool bShowSectors = false;
	
	UPROPERTY(EditAnywhere, Category="SplineMeasurer", meta=(EditCondition="bShowSectors"))
	FLinearColor SectorsColor{FColor::Yellow};

	UPROPERTY(EditAnywhere, Category="SplineMeasurer", meta=(InlineEditConditionToggle))
	bool bShowTravelTime = true;

	UPROPERTY(EditAnywhere, Category="SplineMeasurer", meta=(EditCondition="bShowTravelTime"))
	float Speed = 1000;
	
	UPROPERTY(EditAnywhere, Category="SplineMeasurer")
	bool bIsLooped = false;

	UPROPERTY(EditAnywhere, Category="SplineMeasurer")
	FColor SplineColor{FColor::Magenta};
	
	UPROPERTY(EditAnywhere, Category="SplineMeasurer")
	bool bDrawInGame = false;
};
