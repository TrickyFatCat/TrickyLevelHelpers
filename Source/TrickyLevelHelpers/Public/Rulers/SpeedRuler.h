// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpeedRuler.generated.h"

class UBillboardComponent;
class UDebugTextComponent;

UCLASS()
class TRICKYLEVELHELPERS_API ASpeedRuler : public AActor
{
	GENERATED_BODY()

public:
	ASpeedRuler();

protected:
	virtual bool ShouldTickIfViewportsOnly() const override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<UBillboardComponent> Billboard = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> DebugText = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> DebugTextNote = nullptr;

	float TravelDistance = 0.f;

	UPROPERTY(EditAnywhere, Category="SpeedRuler", meta=(ClampMin="0"))
	float TravelSpeed = 1000.f;
	
	UPROPERTY(EditAnywhere, Category="SpeedRuler", meta=(ClampMin="0"))
	float TravelTime = 5.f;

	UPROPERTY(EditAnywhere, Category="SpeedRuler")
	FColor Color = FColor::Magenta;

	UPROPERTY(EditAnywhere, Category="SpeedRuler")
	bool bDrawMarks = true;

	UPROPERTY(EditAnywhere, Category="SpeedRuler", meta=(EditCondition="bDrawMarks"))
	float MarksDistance = 100.f;

	UPROPERTY(EditAnywhere, Category="SpeedRuler")
	bool bShowNote = false;

	UPROPERTY(EditAnywhere, Category="SpeedRuler", meta=(EditCondition="bShowNote"))
	FString NoteText = "Create Note";

	UPROPERTY(EditAnywhere, Category="SpeedRuler", meta=(EditCondition="bShowNote"))
	FLinearColor NoteColor = FColor::White;

	UPROPERTY(EditAnywhere, Category="SpeedRuler")
	bool bShowInGame = false;
};
