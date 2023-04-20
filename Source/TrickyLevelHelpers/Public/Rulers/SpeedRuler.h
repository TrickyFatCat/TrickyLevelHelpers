// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpeedRuler.generated.h"

class USceneComponent;
class UBillboardComponent;
class UDebugTextComponent;

UCLASS(Blueprintable, HideCategories=(Actor, HLOD, Replication, Input, Cooking, Collision, Physics, Rendering, WorldPartition, DataLayers, Networking))
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
	TObjectPtr<USceneComponent> Root = nullptr;
	UPROPERTY()
	TObjectPtr<UBillboardComponent> Billboard = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> DebugText = nullptr;

	float TravelDistance = 0.f;

	UPROPERTY(EditAnywhere, Category="SpeedRuler", meta=(ClampMin="0"))
	float TravelSpeed = 1000.f;
	
	UPROPERTY(EditAnywhere, Category="SpeedRuler", meta=(ClampMin="0"))
	float TravelTime = 5.f;

	UPROPERTY(EditAnywhere, Category="SpeedRuler")
	FColor Color = FColor::Magenta;

	UPROPERTY(EditAnywhere, Category="SpeedRuler", meta=(MultiLine))
	FString NoteText = "Speed Ruler";

	UPROPERTY(EditAnywhere, Category="SpeedRuler")
	bool bShowCircle = true;

	UPROPERTY(EditAnywhere, Category="SpeedRuler")
	bool bShowInGame = false;
};
