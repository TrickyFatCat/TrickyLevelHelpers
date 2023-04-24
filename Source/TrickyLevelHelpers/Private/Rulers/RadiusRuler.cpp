// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Rulers/RadiusRuler.h"

#include "Components/BillboardComponent.h"
#include "Components/DebugTextComponent.h"


ARadiusRuler::ARadiusRuler()
{
	bIsEditorOnlyActor = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

#if WITH_EDITORONLY_DATA
	PrimaryActorTick.bCanEverTick = true;

	Billboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>("Billboard");

	if (Billboard)
	{
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
	}

	DebugText = CreateEditorOnlyDefaultSubobject<UDebugTextComponent>("Name");

	if (DebugText)
	{
		DebugText->SetupAttachment(GetRootComponent());
		DebugText->SetDrawOneLabel(true);
	}
#else
	PrimaryActorTick.bCanEverTick = false;
#endif
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

	if (DebugText)
	{
		DebugText->SetDrawInGame(bShowInGame);
		DebugText->SetRelativeLocation(GetActorForwardVector() * Radius);

		FDebugLabelData DebugLabelData;
		DebugLabelData.Color = Color;
		DebugLabelData.TextScale = 1.15;
		const FString HeightText = Shape != ERadiusRulerShape::Cylinder
			                           ? ""
			                           : FString::Printf(TEXT("Height : %d | %.2f m"),
			                                             static_cast<int32>(Height),
			                                             Height / 100.0);

		const float Time = Speed <= 0.f ? 0.0 : Radius / Speed;
		const FString TravelText = !bShowTravelTime
			                           ? ""
			                           : FString::Printf(TEXT("Speed : %.2f m/s\nTime: %.2f sec"), Speed / 100.f, Time);
		const FString Text = FString::Printf(TEXT("%s\n---------\nRadius : %d | %.2f m\n%s\n%s"),
		                                     *NoteText,
		                                     static_cast<int32>(Radius),
		                                     Radius / 100.f,
		                                     *HeightText,
		                                     *TravelText);
		DebugLabelData.Text = Text;
		DebugText->SetDebugLabel(DebugLabelData);
	}
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
