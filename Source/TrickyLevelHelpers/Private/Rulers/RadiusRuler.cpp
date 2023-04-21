// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Rulers/RadiusRuler.h"

#include "Components/BillboardComponent.h"
#include "Components/DebugTextComponent.h"


ARadiusRuler::ARadiusRuler()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

#if WITH_EDITORONLY_DATA
	Billboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>("Billboard");
	Billboard->SetupAttachment(GetRootComponent());

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
	Billboard->SetSprite(ConstructorStatics.SpriteTexture.Object);
	Billboard->bUseInEditorScaling = false;
	Billboard->bIsScreenSizeScaled = true;
	Billboard->ScreenSize = 0.001;

	DebugText = CreateEditorOnlyDefaultSubobject<UDebugTextComponent>("Name");
	DebugText->SetupAttachment(GetRootComponent());
	DebugText->SetDrawInGame(false);
	DebugText->SetDrawOneLabel(true);
#else
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
#endif

	bIsEditorOnlyActor = true;
}

bool ARadiusRuler::ShouldTickIfViewportsOnly() const
{
	return true;
}

void ARadiusRuler::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if WITH_EDITORONLY_DATA
	bIsEditorOnlyActor = !bShowInGame;
	DebugText->SetDrawInGame(bShowInGame);

	FDebugLabelData DebugLabelData;
	DebugLabelData.Color = Color;
	DebugLabelData.TextScale = 1.15;
	DebugLabelData.bUseCustomLocation = true;

	const float Theta = FMath::DegreesToRadians(TextAngleOffset);
	const FVector Centre = GetActorLocation();
	FVector Location = Centre;
	Location.X = Radius * FMath::Cos(Theta) + Centre.X;
	Location.Y = Radius * FMath::Sin(Theta) + Centre.Y;
	DebugLabelData.Location = Location;

	const FString HeaderText = FString::Printf(TEXT("%s\n---------\n"), *NoteText);
	const FString LengthData = FString::Printf(
		TEXT("%sUnits: %d\nMeters: %.2f"), *HeaderText,
		static_cast<int32>(Radius),
		Radius / 100.f);
	DebugLabelData.Text = LengthData;
	DebugText->SetDebugLabel(DebugLabelData);
#endif
}


void ARadiusRuler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
#if WITH_EDITORONLY_DATA
	switch (Shape)
	{
	case ERadiusRulerShape::Circle:
		DrawCircle(GetWorld(),
		           GetActorLocation(),
		           GetActorForwardVector(),
		           GetActorRightVector(),
		           Color,
		           Radius,
		           32,
		           false,
		           -1,
		           0,
		           Thickness);
		break;

	case ERadiusRulerShape::Sphere:
		DrawDebugSphere(GetWorld(),
		                GetActorLocation(),
		                Radius,
		                24,
		                Color,
		                false,
		                -1,
		                0,
		                Thickness);
		break;

	case ERadiusRulerShape::Cylinder:
		DrawDebugCylinder(GetWorld(),
		                  GetActorLocation(),
		                  GetActorLocation() + GetActorUpVector() * Height,
		                  Radius,
		                  24,
		                  Color,
		                  false,
		                  -1,
		                  0,
		                  Thickness);
		break;
	}
#endif
}
