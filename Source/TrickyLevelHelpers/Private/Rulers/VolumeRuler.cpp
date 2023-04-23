// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Rulers/VolumeRuler.h"

#include "Components/BillboardComponent.h"
#include "Components/DebugTextComponent.h"


AVolumeRuler::AVolumeRuler()
{
	
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

#if WITH_EDITORONLY_DATA
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	Billboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>("Billboard");
	Billboard->SetupAttachment(GetRootComponent());

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
	Billboard->SetSprite(ConstructorStatics.SpriteTexture.Object);
	SpriteScale = 0.5;

	DebugText = CreateEditorOnlyDefaultSubobject<UDebugTextComponent>("DebugText");
	DebugText->SetupAttachment(GetRootComponent());
	DebugText->SetDrawInGame(false);
	DebugText->SetDrawOneLabel(true);
#else
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
#endif
	bIsEditorOnlyActor = true;
}

bool AVolumeRuler::ShouldTickIfViewportsOnly() const
{
	return true;
}

void AVolumeRuler::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA
	bIsEditorOnlyActor = !bShowInGame;
	DebugText->SetDrawInGame(bShowInGame);
	
	Color.A = 255 * 0.15;
	Extent = Size * 0.5;
	Center = bCenterOrigin ? GetActorLocation() : GetActorLocation() + Extent;

	FDebugLabelData DebugLabelData;
	FColor TextColor = Color;
	TextColor.A = 255;
	DebugLabelData.Color = TextColor;
	DebugLabelData.TextScale = 1.15;
	DebugLabelData.bUseCustomLocation = true;
	DebugLabelData.Location = GetActorLocation();

	const FString HeaderText = FString::Printf(TEXT("%s\n---------\n"), *NoteText);
	const FString UnitsText = FString::Printf(
		TEXT("Units: X = %d | Y = %d | Z = %d"),
		static_cast<int32>(Size.X),
		static_cast<int32>(Size.Y),
		static_cast<int32>(Size.Z));
	const FString MetersText = FString::Printf(
    		TEXT("Meters: X = %.2f | Y = %.2f | Z = %.2f"),
    		Size.X / 100.0,
    		Size.Y / 100.0,
    		Size.Z / 100.0);
	const FString LengthData = FString::Printf(TEXT("%s\n%s\n%s"), *HeaderText, *UnitsText, *MetersText);
	DebugLabelData.Text = LengthData;
	DebugText->SetDebugLabel(DebugLabelData);

#endif
}

void AVolumeRuler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITORONLY_DATA

	Center = bCenterOrigin ? GetActorLocation() : GetActorLocation() + Extent;
	
	DrawDebugBox(GetWorld(),
	             Center,
	             Extent,
	             GetActorRotation().Quaternion(),
	             Color,
	             false,
	             -1,
	             0,
	             Thickness);

	if (bDrawSolidBox)
	{
		DrawDebugSolidBox(GetWorld(),
		                  Center,
		                  Extent - 0.1,
		                  GetActorRotation().Quaternion(),
		                  Color,
		                  false,
		                  -1,
		                  0);
	}
#endif
}
