// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineMeasurer.generated.h"

class USplineComponent;
class UTextRenderComponent;
class UDebugTextComponent;

UCLASS(Blueprintable, HideCategories=(Activation, AssetUserData, Collision, Cooking, HLOD, Lighting, LOD, Mobile, Navigation, Physics, RayTracing, Rendering, Tags, TextureStreaming))
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
	TObjectPtr<UDebugTextComponent> DistanceDebugText = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> SectorDebugText = nullptr;

	UPROPERTY(EditAnywhere, Category="SplineMeasurer")
	bool bDrawInGame = false;

	UPROPERTY(EditAnywhere, Category="SplineMeasurer")
	bool bShowDistance = true;

	UPROPERTY(EditAnywhere, Category="SplineMeasurer", meta=(EditCondition="bShowDistance", EditConditionHides))
	FLinearColor DistanceTextColor{FColor::Red};

	UPROPERTY(EditAnywhere, Category="SplineMeasurer", meta=(EditCondition="bShowDistance", EditConditionHides))
	float DistanceTextScale = 1.15f;

	UPROPERTY(EditAnywhere, Category="SplineMeasurer")
	bool bShowSectors = false;
	
	UPROPERTY(EditAnywhere, Category="SplineMeasurer", meta=(EditCondition="bShowSectors", EditConditionHides))
	FLinearColor SectorsTextColor{FColor::Yellow};

	UPROPERTY(EditAnywhere, Category="SplineMeasurer", meta=(EditCondition="bShowSectors", EditConditionHides))
	float SectorsTextScale = 1.f;
};
