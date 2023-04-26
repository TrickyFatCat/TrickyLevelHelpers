// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VolumeRuler.generated.h"

class USceneComponent;
class UBillboardComponent;
class UDebugTextComponent;

/**
 * Visualizes given distance as a cube.
 */
UCLASS(Blueprintable, HideCategories=(Tick, Events, Actor, HLOD, Replication, Input, Cooking, Collision, Physics, Rendering, WorldPartition, DataLayers,Networking))
class TRICKYLEVELHELPERS_API AVolumeRuler : public AActor
{
	GENERATED_BODY()

public:
	AVolumeRuler();

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

	UPROPERTY(EditAnywhere, Category="VolumeRuler", meta=(MultiLine))
	FString NoteText = "Volume Ruler";

	UPROPERTY(EditAnywhere, Category="VolumeRuler", meta=(AllowPreserveRatio))
	FVector Size{100.f};

	UPROPERTY(EditAnywhere, Category="VolumeRuler")
	bool bCenterOrigin = false;

	FVector Extent = Size * 0.5;

	FVector Center{FVector::Zero()};

	UPROPERTY(EditAnywhere, Category="VolumeRuler")
	bool bDrawSolidBox = false;

	UPROPERTY(EditAnywhere, Category="VolumeRuler", meta=(HideAlphaChannel))
	FColor Color{255, 0, 0};

	float Thickness = 3.f;

	UPROPERTY(EditAnywhere, Category="VolumeRuler")
	bool bShowInGame = false;

	static FString GetDimension(const double Length, const FString& AxisName);
};
