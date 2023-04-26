// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LineRuler.generated.h"

class USceneComponent;
class UBillboardComponent;
class UDebugTextComponent;

/**
 * Visualizes a given distance along X, Y and Z axis
 */
UCLASS(Blueprintable, HideCategories=(Tick, Events, Actor, HLOD, Replication, Input, Cooking, Collision, Physics, Rendering, WorldPartition, DataLayers,Networking))
class TRICKYLEVELHELPERS_API ALineRuler : public AActor
{
	GENERATED_BODY()

public:
	ALineRuler();

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

	UPROPERTY(EditAnywhere, Category="LineRuler")
	FString NoteText = "Line Ruler";

	UPROPERTY(EditAnywhere, Category="LineRuler")
	FVector Length{100.f};

	UPROPERTY(EditAnywhere, Category="LineRuler")
	FVector MarksSpacing{100.f};

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(InlineEditConditionToggle))
	bool bShowTravelTime = false;

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(EditCondition="bShowTravelTime"))
	float Speed = 1000.f;	

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(InlineEditConditionToggle))
	bool bShowX = true;

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(InlineEditConditionToggle))
	bool bShowY = true;

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(InlineEditConditionToggle))
	bool bShowZ = true;

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(EditCondition="bShowX", HideAlphaChannel, DisplayName="X"))
	FColor ColorX{230, 57, 0};

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(EditCondition="bShowY", HideAlphaChannel, DisplayName="Y"))
	FColor ColorY{65, 188, 65};

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(EditCondition="bShowZ", HideAlphaChannel, DisplayName="Z"))
	FColor ColorZ{0, 149, 230};

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(HideAlphaChannel))
	FLinearColor TextColor = FColor::Magenta;
	
	UPROPERTY(EditAnywhere, Category="LineRuler")
	bool bShowInGame = false;

	const float Thickness = 3.f;

	void DrawLine(const bool bShow, const double Distance, const double Spacing, const FVector& Axis,
	              const FColor& Color) const;

	static FString PrintAxisDebug(const bool bShow, const double Length, const FString& AxisName);

	static FString PrintTravelDebug(const bool bShow,
	                                const double Length,
	                                const double Speed,
	                                const FString& AxisName);
};
