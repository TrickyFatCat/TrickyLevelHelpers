// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LineRuler.generated.h"

class UBillboardComponent;
class UDebugTextComponent;

UCLASS()
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
	TObjectPtr<UBillboardComponent> Root = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> DebugTextX = nullptr;
	
	UPROPERTY()
	TObjectPtr<UDebugTextComponent> DebugTextY = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> DebugTextZ = nullptr;
	
	UPROPERTY(EditAnywhere, Category="LineRuler")
	FVector Length{100.0};

	UPROPERTY(EditAnywhere, Category="LineRuler")
	float LineThickness = 2.f;

	UPROPERTY(EditAnywhere, Category="LineRuler")
	bool bDrawMarks = false;

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(EditCondition="bDrawMarks", EditConditionHides))
	FVector MarksDistance{100.0};

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(EditCondition="bDrawMarks", EditConditionHides))
	float MarksScale = 50.f;

	UPROPERTY(EditAnywhere, Category="LineRuler")
	FVector TravelSpeed{1000.0};

	void DrawLine(const double LineLength, const FVector& Axis, const FColor& Color) const;
	
	void DrawMarks(const double LineLength, double MarkDistance, const FVector& Axis, const FColor& Color) const;

	void DrawDebugText(UDebugTextComponent* DebugText, const double LineLength, const FVector& Axis, const FColor& Color);
};
