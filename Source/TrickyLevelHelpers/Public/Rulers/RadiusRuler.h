// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RadiusRuler.generated.h"

class USceneComponent;
class UBillboardComponent;
class UDebugTextComponent;

UENUM()
enum class ERadiusRulerShape : uint8
{
	Circle,
	Sphere,
	Cylinder
};

/**
 * Visualizes given radius.
 */
UCLASS(Blueprintable, HideCategories=(Tick, Events, Actor, HLOD, Replication, Input, Cooking, Collision, Physics, Rendering, WorldPartition, DataLayers,Networking))
class TRICKYLEVELHELPERS_API ARadiusRuler : public AActor
{
	GENERATED_BODY()

public:
	ARadiusRuler();

protected:
	virtual bool ShouldTickIfViewportsOnly() const override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<USceneComponent> Root = nullptr;

	UPROPERTY()
	TObjectPtr<UBillboardComponent> Billboard = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> DebugText = nullptr;

	UPROPERTY(EditAnywhere, Category="RadiusRuler", meta=(MultiLine))
	FString NoteText = "Radius Ruler";

	UPROPERTY(EditAnywhere, Category="RadiusRuler", meta=(ClampMin="0"))
	float Radius = 100.f;

	UPROPERTY(EditAnywhere, Category="RadiusRuler")
	ERadiusRulerShape Shape = ERadiusRulerShape::Sphere;
	
	UPROPERTY(EditAnywhere,
		Category="RadiusRuler",
		meta=(EditCondition="Shape==ERadiusRulerShape::Cylinder", EditConditionHides, ClampMin="0"))
	float Height = 500.f;
	
	UPROPERTY(EditAnywhere, Category="RadiusRuler", meta=(InlineEditConditionToggle))
	bool bShowTravelTime = false;
	
	UPROPERTY(EditAnywhere, Category="RadiusRuler", meta=(EditCondition="bShowTravelTime", ClampMin="1"))
	float Speed = 1000.f;
	
	UPROPERTY(EditAnywhere, Category="RadiusRuler", meta=(HideAlphaChannel))
	FColor Color = FColor::Magenta;

	UPROPERTY(EditAnywhere, Category="RadiusRuler")
	bool bShowInGame = false;

	const float Thickness = 4.0f;
};
