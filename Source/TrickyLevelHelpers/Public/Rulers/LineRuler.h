// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LineRuler.generated.h"

class UBillboardComponent;
class UDebugTextComponent;

USTRUCT(BlueprintType, meta=(HiddenByDefault))
struct FLineRulerAxisData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LineRulerAxisData")
	bool bDrawAxis = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LineRulerAxisData")
	float Length = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LineRulerAxisData")
	bool bShowTravelTime = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LineRulerAxisData", meta=(EditCondition="bShowTravelTime"))
	float TravelSpeed = 1000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LineRulerAxisData")
	FColor Color = FColor::Magenta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LineRulerAxisData")
	bool bDrawMarks = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LineRulerAxisData", meta=(EditCondition="bDrawMarks"))
	float MarksDistance = 100.f;
};

UCLASS(Blueprintable, HideCategories=(AssetUserData, Collision, Cooking, HLOD, Lighting, LOD, Mobile, Navigation, Physics, RayTracing, Rendering, Tags, TextureStreaming, Actor, Replication, Input, Networking, WorldPartition, DataLayers) )
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
	TObjectPtr<UBillboardComponent> Billboard = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> DebugTextX = nullptr;
	
	UPROPERTY()
	TObjectPtr<UDebugTextComponent> DebugTextY = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> DebugTextZ = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> DebugTextNote = nullptr;

	UPROPERTY(EditAnywhere, Category="LineRuler")
	FLineRulerAxisData X;
	
	UPROPERTY(EditAnywhere, Category="LineRuler")
	FLineRulerAxisData Y;
	
	UPROPERTY(EditAnywhere, Category="LineRuler")
	FLineRulerAxisData Z;

	UPROPERTY(EditAnywhere, Category="LineRuler")
	bool bShowNote = false;

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(EditCondition="bShowNote"))
	FString NoteText = "Create Note";

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(EditCondition="bShowNote"))
	FLinearColor NoteColor = FColor::White;

	UPROPERTY(EditAnywhere, Category="LineRuler")
	bool bShowInGame = false;
	
	void DrawLine(const FLineRulerAxisData& AxisData, const FVector& Axis) const;
	
	void DrawMarks(const FLineRulerAxisData& AxisData, const FVector& Axis) const;

	void DrawDebugText(UDebugTextComponent* DebugText, const FLineRulerAxisData& AxisData, const FVector& Axis) const;
};
