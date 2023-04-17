// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Rulers/LineRuler.h"

#include "Components/BillboardComponent.h"
#include "Components/DebugTextComponent.h"


ALineRuler::ALineRuler()
{
	Root = CreateDefaultSubobject<UBillboardComponent>("Root");
	SetRootComponent(ToRawPtr(Root));

#if WITH_EDITORONLY_DATA
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinder<UTexture2D> SpriteTexture;
		FName ID_Misc;
		FText NAME_Misc;

		FConstructorStatics()
			: SpriteTexture(TEXT("/Engine/EditorResources/S_Note"))
			  , ID_Misc(TEXT("Misc"))
			  , NAME_Misc(NSLOCTEXT("SpriteCategory", "Misc", "Misc"))
		{
		}
	};

	static FConstructorStatics ConstructorStatics;
	Root->SetSprite(ConstructorStatics.SpriteTexture.Object);

	auto CreateDebugText = [&](TObjectPtr<UDebugTextComponent>& DebugText, const FName& Name) -> void
	{
		DebugText = CreateDefaultSubobject<UDebugTextComponent>(Name);
		DebugText->SetupAttachment(GetRootComponent());
		DebugText->SetDrawOneLabel(false);
	};

	CreateDebugText(DebugTextX, "DebugTextX");
	CreateDebugText(DebugTextY, "DebugTextY");
	CreateDebugText(DebugTextZ, "DebugTextZ");
#else
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
#endif

	bIsEditorOnlyActor = true;
}

bool ALineRuler::ShouldTickIfViewportsOnly() const
{
	return true;
}

void ALineRuler::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA

#endif
}

void ALineRuler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITORONLY_DATA
	DrawLine(Length.X, GetActorForwardVector(), FColor::Red);
	DrawLine(Length.Y, GetActorRightVector(), FColor::Green);
	DrawLine(Length.Z, GetActorUpVector(), FColor{65, 105, 255});

	if (bDrawMarks)
	{
		DrawMarks(Length.X, MarksDistance.X, GetActorForwardVector(), FColor::Red);
		DrawMarks(Length.Y, MarksDistance.Y, GetActorRightVector(), FColor::Green);
		DrawMarks(Length.Z, MarksDistance.Z, GetActorUpVector(), FColor{65, 105, 255});
	}

	DrawDebugText(DebugTextX, Length.X, GetActorForwardVector(), FColor::Red);
	DrawDebugText(DebugTextY, Length.Y, GetActorRightVector(), FColor::Green);
	DrawDebugText(DebugTextZ, Length.Z, GetActorUpVector(), FColor{137, 207, 240});

#endif
}

void ALineRuler::DrawLine(const double LineLength, const FVector& Axis, const FColor& Color) const
{
	if (LineLength == 0.f)
	{
		return;
	}

	const FVector LineStart = GetActorLocation();
	const FVector LineEnd = LineStart + Axis * LineLength;

	DrawDebugLine(GetWorld(),
	              LineStart,
	              LineEnd,
	              Color,
	              false,
	              -1,
	              0,
	              LineThickness);
}

void ALineRuler::DrawMarks(const double LineLength,
                           const double MarkDistance,
                           const FVector& Axis,
                           const FColor& Color) const
{
	if (LineLength == 0.0 || MarkDistance == 0.0)
	{
		return;
	}

	const int32 Amount = FMath::FloorToInt32(FMath::Abs(LineLength) / MarkDistance);


	for (int32 i = 0; i < Amount; ++i)
	{
		const FVector Location = GetActorLocation() + Axis * MarkDistance * (i + 1) * FMath::Sign(LineLength);
		DrawDebugCrosshairs(GetWorld(),
		                    Location,
		                    GetActorRotation(),
		                    MarksScale,
		                    Color);
	}
}

void ALineRuler::DrawDebugText(UDebugTextComponent* DebugText,
                               const double LineLength,
                               const FVector& Axis,
                               const FColor& Color)
{
	if (!IsValid(DebugText))
	{
		return;
	}

	DebugText->bDrawDebug = LineLength != 0.0;

	if (!DebugText->bDrawDebug)
	{
		return;
	}

	TArray<FDebugLabelData> DebugLabels;

	FDebugLabelData DebugLabelData;
	DebugLabelData.bUseCustomLocation = true;
	DebugLabelData.Color = Color;
	DebugLabelData.TextScale = 1.15;

	FVector TextLocation = GetActorLocation() + Axis * Length;
	TextLocation.Z += 50.0;
	DebugLabelData.Location = TextLocation;

	const FString LengthData = FString::Printf(
		TEXT("Units: %d\nMeters: %.2f"), static_cast<int32>(LineLength), LineLength / 100.0);
	DebugLabelData.Text = LengthData;

	DebugLabels.Add(DebugLabelData);
	DebugText->SetDebugLabels(DebugLabels);
}
