// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LineRuler.generated.h"

class UBillboardComponent;

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

	UPROPERTY(EditAnywhere, Category="LineRuler")
	FVector Distance{100.0};

	UPROPERTY(EditAnywhere, Category="LineRuler")
	float LineThickness = 2.f;

	UPROPERTY(EditAnywhere, Category="LineRuler")
	bool bDrawMarks = false;

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(EditCondition="bDrawMarks"))
	FVector MarksDistance{100.0};

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(EditCondition="bDrawMarks"))
	float MarksScale = 50.f;

	void DrawLine(const double Length, const FVector& Axis, const FColor& Color) const;
	
	void DrawMarks(const double Length, double MarkDistance, const FVector& Axis, const FColor& Color) const;
};
