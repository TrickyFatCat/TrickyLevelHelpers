// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpeedRuler.generated.h"

class USceneComponent;
class UBillboardComponent;
class UDebugTextComponent;

/**
 * Visualizes distance calculated using time, speed, acceleration, deceleration.
 */
UCLASS(Blueprintable, HideCategories=(Tick, Events, Actor, HLOD, Replication, Input, Cooking, Collision, Physics, Rendering, WorldPartition, DataLayers,Networking))
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

	UPROPERTY(EditAnywhere, Category="SpeedRuler", meta=(ClampMin="1"))
	float MovementTime = 5.f;

	UPROPERTY(EditAnywhere, Category="SpeedRuler")
	bool bIsUniformSpeed = true;

	UPROPERTY(EditAnywhere,
		Category="SpeedRuler",
		meta=(EditCondition="bIsUniformSpeed", EditConditionHides, ClampMin="1"))
	float Speed = 1000.f;

	UPROPERTY(EditAnywhere,
		Category="SpeedRuler",
		meta=(EditCondition="!bIsUniformSpeed", EditConditionHides, ClampMin="0"))
	float InitialSpeed = 0.f;

	UPROPERTY(EditAnywhere,
		Category="SpeedRuler",
		meta=(EditCondition="!bIsUniformSpeed", EditConditionHides, ClampMin="1"))
	float TargetSpeed = 1000.f;

	UPROPERTY(EditAnywhere,
		Category="SpeedRuler",
		meta=(EditCondition="!bIsUniformSpeed", EditConditionHides, ClampMin="0"))
	float Acceleration = 200.f;

	float AccelTime = 0.f;

	float AccelDist = 0.f;

	UPROPERTY(EditAnywhere,
		Category="SpeedRuler",
		meta=(EditCondition="!bIsUniformSpeed", EditConditionHides, ClampMin="0"))
	float Deceleration = 400.f;

	float DecelTime = 0.f;

	float DecelDist = 0.f;

	float FinalDistance = 0.f;

	float FinalTime = 0.f;

	UPROPERTY(EditAnywhere, Category="SpeedRuler")
	bool bShowCircle = true;

	UPROPERTY(EditAnywhere, Category="SpeedRuler", meta=(HideAlphaChannel))
	FColor Color = FColor::Magenta;

	UPROPERTY(EditAnywhere,
		Category="SpeedRuler",
		meta=(EditCondition="!bIsUniformSpeed", EditConditionHides, HideAlphaChannel))
	FColor AccelerationColor = FColor::Purple;

	UPROPERTY(EditAnywhere,
		Category="SpeedRuler",
		meta=(EditCondition="!bIsUniformSpeed", EditConditionHides, HideAlphaChannel))
	FColor DecelerationColor = FColor::Red;
	
	UPROPERTY(EditAnywhere, Category="SpeedRuler")
	bool bShowInGame = false;

	float Thickness = 5.f;

	void CalculateDistance();

	void DrawUniformSpeedDistance() const;

	void DrawVariableSpeedDistance() const;

	void DrawDistanceLine(const FVector& LineStart, const FVector& LineEnd, const FColor& DrawColor) const;

	void DrawDistanceCircle(const float Radius, const FColor& DrawColor) const;

	static FString PrintUniformSpeedDebug(const float Distance,
	                                      const float Speed,
	                                      const float Time);

	static FString PrintAccelerationDebug(const float Acceleration,
	                                      const float Distance,
	                                      const float InitialSpeed,
	                                      const float CurrentSpeed,
	                                      const float Time);

	static FString PrintTargetSpeedDebug(const float Speed,
	                                     const float Distance,
	                                     const float Time);

	static FString PrintDecelerationDebug(const float Deceleration,
	                                      const float Distance,
	                                      const float InitialSpeed,
	                                      const float Time);
};
