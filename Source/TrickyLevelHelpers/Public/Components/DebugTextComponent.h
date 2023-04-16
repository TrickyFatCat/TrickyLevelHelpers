// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Debug/DebugDrawComponent.h"
#include "DebugTextComponent.generated.h"

USTRUCT(BlueprintType)
struct FDebugLabelData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DebugText")
	FString Text = "DebugText";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DebugText")
	bool bUseCustomLocation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DebugText",
		meta=(EditCondition="!bUseCustomLocation", EditConditionHides))
	FVector Offset{FVector::Zero()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DebugText",
		meta=(EditCondition="bUseCustomLocation", EditConditionHides))
	FVector Location{FVector::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DebugText")
	FLinearColor Color{FColor::Magenta};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DebugText")
	float TextScale = 1.f;
};

struct FDebugSceneProxyData
{
	struct FDebugText
	{
		FString Text;
		FVector Location;
		FColor Color;
		float Scale;

		FDebugText()
		{
		}

		FDebugText(const FDebugLabelData& Label, const FVector& InLocation)
			: Text(Label.Text), Location(InLocation), Color(Label.Color.ToFColor(false)), Scale(Label.TextScale)
		{
		}
	};

	TArray<FDebugText> DebugLabels;
};

class FDebugSceneProxy : public FDebugRenderSceneProxy
{
public:
	FDebugSceneProxy(const UPrimitiveComponent* InComponent, FDebugSceneProxyData* ProxyData);

	FDebugSceneProxyData ProxyData;
};

class FDebugTextDelegateHelper : public FDebugDrawDelegateHelper
{
public:
	virtual void DrawDebugLabels(UCanvas* Canvas, APlayerController*) override;

	void SetupFromProxy(const FDebugSceneProxy* InSceneProxy);

	bool bDrawDebug = true;

	TArray<FDebugSceneProxyData::FDebugText> DebugLabels;
};


UCLASS(ClassGroup=(TrickyLevelHelpers), meta=(BlueprintSpawnableComponent))
class TRICKYLEVELHELPERS_API UDebugTextComponent : public UDebugDrawComponent
{
	GENERATED_BODY()

public:
	UDebugTextComponent();
	
	UPROPERTY(EditAnywhere, Category="DebugText")
	bool bDrawDebug = true;

protected:
	FDebugTextDelegateHelper DebugDrawDelegateManager;

	UPROPERTY(EditAnywhere, Category="DebugText")
	bool bDrawOneLabel = true;

	UPROPERTY(EditAnywhere, Category="DebugText", meta=(EditCondition = "bDrawOneLabel", EditConditionHides))
	FDebugLabelData DebugLabel;

	UPROPERTY(EditAnywhere, Category="DebugText", meta=(EditCondition = "!bDrawOneLabel", EditConditionHides))
	TArray<FDebugLabelData> DebugLabels;

	virtual FDebugRenderSceneProxy* CreateDebugSceneProxy() override;

	virtual FDebugDrawDelegateHelper& GetDebugDrawDelegateHelper() override { return DebugDrawDelegateManager; }

	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

public:
	UFUNCTION(BlueprintCallable, Category="DebugText")
	void SetDebugLabel(const FDebugLabelData& LabelData);

	UFUNCTION(BlueprintCallable, Category="DebugText")
	void SetDebugLabels(const TArray<FDebugLabelData>& LabelsData);

	void SetDrawOneLabel(const bool Value) { bDrawOneLabel = Value; }
};
