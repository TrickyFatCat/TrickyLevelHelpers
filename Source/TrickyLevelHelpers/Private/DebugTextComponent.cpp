// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "DebugTextComponent.h"

#include "Components/SplineComponent.h"
#include "Engine/Canvas.h"

FDebugSceneProxy::FDebugSceneProxy(const UPrimitiveComponent* InComponent,
                                   FDebugSceneProxyData* ProxyData) : FDebugRenderSceneProxy(InComponent)
{
	DrawType = EDrawType::SolidAndWireMeshes;
	ViewFlagName = "Editor";

	this->ProxyData = *ProxyData;

	for (const auto& Text : ProxyData->DebugLabels)
	{
		this->Texts.Add({
			Text.Text,
			Text.Location,
			FColor::White
		});
	}
}

void FDebugTextDelegateHelper::DrawDebugLabels(UCanvas* Canvas, APlayerController* PlayerController)
{
	if (!Canvas)
	{
		return;
	}

	const FColor OldDrawColor = Canvas->DrawColor;
	Canvas->SetDrawColor(FColor::White);
	const FSceneView* View = Canvas->SceneView;
	const UFont* Font = GEngine->GetSmallFont();
	const FDebugSceneProxyData::FDebugText* DebugText = DebugLabels.GetData();

	for (int32 i = 0; i < DebugLabels.Num(); ++i, ++DebugText)
	{
		if (View->ViewFrustum.IntersectSphere(DebugText->Location, 1.0f))
		{
			const FVector ScreenLoc = Canvas->Project(DebugText->Location);
			Canvas->DrawText(Font, DebugText->Text, ScreenLoc.X, ScreenLoc.Y);
		}
	}

	Canvas->SetDrawColor(OldDrawColor);
}

void FDebugTextDelegateHelper::SetupFromProxy(const FDebugSceneProxy* InSceneProxy)
{
	if (bResetLabels)
	{
		DebugLabels.Reset();
	}

	DebugLabels.Append(InSceneProxy->ProxyData.DebugLabels);
}

UDebugTextComponent::UDebugTextComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

FDebugRenderSceneProxy* UDebugTextComponent::CreateDebugSceneProxy()
{
	if (DebugLabels.Num() == 0)
	{
		return nullptr;
	}
	// USplineComponent* SplineComponent = GetOwner()->FindComponentByClass<USplineComponent>();

	// if (!SplineComponent)
	// {
	// return nullptr;
	// }

	// const int32 PointsNum = SplineComponent->GetNumberOfSplinePoints();
	// const int32 LastPointIndex = SplineComponent->IsClosedLoop() ? PointsNum : PointsNum - 1;

	FDebugSceneProxyData ProxyData;

	// for (int32 i = 0; i <= LastPointIndex; ++i)
	// {
	// const FVector Location = SplineComponent->GetLocationAtSplinePoint(
	// i, ESplineCoordinateSpace::World);
	// const float Distance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(i);
	// const FString InfoText = FString::Printf(
	// TEXT("Units: %d\nMeters: %.2f"), static_cast<int32>(Distance), Distance / 100.f);
	// ProxyData.DebugLabels.Add({Location, InfoText});
	// }

	for (TPair<FString, FVector>& Label : DebugLabels)
	{
		ProxyData.DebugLabels.Add({Label.Value, Label.Key});
	}

	FDebugSceneProxy* DebugSceneProxy = new FDebugSceneProxy(this, &ProxyData);

	if (DebugSceneProxy)
	{
		DebugDrawDelegateManager.SetupFromProxy(DebugSceneProxy);
	}

	return DebugSceneProxy;
}

FBoxSphereBounds UDebugTextComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	return FBoxSphereBounds(FBox(FVector(-1000, -1000, -1000), FVector(1000, 1000, 1000)));
}

void UDebugTextComponent::SetDebugText(const TMap<FString, FVector>& LabelsData, const bool bResetLabels)
{
	DebugLabels.Empty();
	DebugLabels = LabelsData;
	this->MarkRenderStateDirty();
	DebugDrawDelegateManager.bResetLabels = bResetLabels;
}
