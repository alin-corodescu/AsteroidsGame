// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "SimpleHUD.h"
#include "../PlayerAvatar/AsteroidsPlayerState.h"

#define LOCTEXT_NAMESPACE "SimpleHUD"

ASimpleHUD::ASimpleHUD()
{
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	HUDFont = Font.Object;
}

void ASimpleHUD::DrawHUD()
{
	Super::DrawHUD();

	if (CurrentState)
	{
		FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);

		FText ScoreString = FText::Format(LOCTEXT("TestFormat", "Score: {0}"), 
			FText::AsNumber(CurrentState->GetScore()));

		FVector2D TextCentrePos = FVector2D((Canvas->SizeX - 200), 10);
		FCanvasTextItem TextItem(TextCentrePos, ScoreString, 
			HUDFont, FLinearColor::Blue);
		Canvas->DrawItem(TextItem);
	}
	else
	{
		APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		if (Player)
		{
			CurrentState = Cast<AAsteroidsPlayerState>(Player->PlayerState);
		}
	}
}

#undef LOCTEXT_NAMESPACE