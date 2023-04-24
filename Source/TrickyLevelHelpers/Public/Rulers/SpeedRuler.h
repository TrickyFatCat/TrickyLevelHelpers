// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpeedRuler.generated.h"

class USceneComponent;
class UBillboardComponent;
class UDebugTextComponent;

UCLASS(Blueprintable,
	HideCategories=(Actor, HLOD, Replication, Input, Cooking, Collision, Physics, Rendering, WorldPartition, DataLayers,
		Networking))
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

	UPROPERTY(EditAnywhere, Category="SpeedRuler", meta=(MultiLine))
	FString NoteText = "Speed Ruler";

	float Distance = 0.f;

	UPROPERTY(EditAnywhere, Category="SpeedRuler", meta=(ClampMin="1"))
	float Speed = 1000.f;

	UPROPERTY(EditAnywhere, Category="SpeedRuler", meta=(ClampMin="1"))
	float Time = 5.f;

	UPROPERTY(EditAnywhere, Category="SpeedRuler")
	bool bShowCircle = true;

	UPROPERTY(EditAnywhere, Category="SpeedRuler", meta=(HideAlphaChannel))
	FColor Color = FColor::Magenta;

	UPROPERTY(EditAnywhere, Category="SpeedRuler")
	bool bShowInGame = false;

	float Thickness = 3.f;
};
