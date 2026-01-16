// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "MainMenu.h"
#include "Interaction/InteractionWidget.h"


void APlayerHUD::PlayerHUD()
{
    
}

void APlayerHUD::BeginPlay()
{
    Super::BeginPlay();

    if (MainMenuClass)
    {
        MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
        MainMenuWidget->AddToViewport(5);
        MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (InteractionWidgetClass)
    {
        InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
        InteractionWidget->AddToViewport(-1);
        InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (CrosshairWidgetClass)
    {
        CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
        CrosshairWidget->AddToViewport();
        CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}


void APlayerHUD::DisplayMenu()
{
    if (MainMenuWidget)
    {
        bIsMenuVisible = true;
        MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void APlayerHUD::HideMenu()
{
    if (MainMenuWidget)
    {
        bIsMenuVisible = false;
        MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void APlayerHUD::ToggleMenu()
{
    if (bIsMenuVisible)
    {
        HideMenu();
        
        const FInputModeGameOnly InputMode;
        GetOwningPlayerController()->SetInputMode(InputMode);
        GetOwningPlayerController()->SetShowMouseCursor(false);
    }
    else{
        DisplayMenu();

        const FInputModeGameAndUI InputMode;
        GetOwningPlayerController()->SetInputMode(InputMode);
        GetOwningPlayerController()->SetShowMouseCursor(true);        
    }
}

void APlayerHUD::ShowCrosshair()
{
    if (CrosshairWidget)
    {
        CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void APlayerHUD::HideCrosshair()
{
    if (CrosshairWidget)
    {
        CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void APlayerHUD::ShowInteractionWidget()
{
    if (InteractionWidget)
    {
        InteractionWidget->SetVisibility(ESlateVisibility::Visible);
    }    
}

void APlayerHUD::HideInteractionWidget()
{
    if (InteractionWidget)
    {
        InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
    }    
}

void APlayerHUD::UpdateInteractionWidget(const FInteractableData* InteractableData)
{
    if (InteractionWidget)
    {
        if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
        {
            ShowInteractionWidget();
        }

        InteractionWidget->UpdateWidget(InteractableData);
    }
}
