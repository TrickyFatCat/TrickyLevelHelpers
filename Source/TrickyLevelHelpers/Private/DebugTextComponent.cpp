// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "DebugTextComponent.h"

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
			Text.Color
		});
	}
}

void FDebugTextDelegateHelper::DrawDebugLabels(UCanvas* Canvas, APlayerController* PlayerController)
{
	if (!Canvas || !bDrawDebug)
	{
		return;
	}

	const FDebugSceneProxyData::FDebugText* DebugText = DebugLabels.GetData();
	const FColor OldDrawColor = Canvas->DrawColor;
	Canvas->SetDrawColor(DebugText->Color);
	const FSceneView* View = Canvas->SceneView;
	const UFont* Font = GEngine->GetSmallFont();

	FFontRenderInfo FontRenderInfo;
	FontRenderInfo.bEnableShadow = true;

	for (int32 i = 0; i < DebugLabels.Num(); ++i, ++DebugText)
	{
		if (View->ViewFrustum.IntersectSphere(DebugText->Location, 1.0f))
		{
			const FVector ScreenLoc = Canvas->Project(DebugText->Location);
			Canvas->DrawText(Font, DebugText->Text, ScreenLoc.X, ScreenLoc.Y, 1, 1, FontRenderInfo);
		}
	}

	Canvas->SetDrawColor(OldDrawColor);
}

void FDebugTextDelegateHelper::SetupFromProxy(const FDebugSceneProxy* InSceneProxy)
{
	DebugLabels.Reset();
	DebugLabels.Append(InSceneProxy->ProxyData.DebugLabels);
}

UDebugTextComponent::UDebugTextComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MinDrawDistance = 1000;
}

FDebugRenderSceneProxy* UDebugTextComponent::CreateDebugSceneProxy()
{
	FDebugSceneProxyData ProxyData;

	auto AddLabelData = [&](const FDebugLabelData& Label)-> void
	{
		FVector Location = Label.bUseCustomLocation ? Label.Location : GetComponentLocation() + Label.Offset;
		ProxyData.DebugLabels.Add({Label.Text, Location, Label.Color.ToFColor(false)});
	};

	if (bDrawOneLabel)
	{
		AddLabelData(DebugLabel);
	}
	else
	{
		if (DebugLabels.Num() == 0)
		{
			return nullptr;
		}

		for (const FDebugLabelData& Label : DebugLabels)
		{
			AddLabelData(Label);
		}
	}

	FDebugSceneProxy* DebugSceneProxy = new FDebugSceneProxy(this, &ProxyData);

	DebugDrawDelegateManager.bDrawDebug = bDrawDebug;

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

void UDebugTextComponent::SetDebugLabel(const FDebugLabelData& LabelData)
{
	DebugLabel = LabelData;
	this->MarkRenderStateDirty();
}

void UDebugTextComponent::SetDebugLabels(const TArray<FDebugLabelData>& LabelsData)
{
	DebugLabels.Empty();
	DebugLabels = LabelsData;
	this->MarkRenderStateDirty();
}
