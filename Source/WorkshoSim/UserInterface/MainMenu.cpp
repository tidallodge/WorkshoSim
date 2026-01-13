// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "WorkshoSim/PlayerCharacter.h"
#include "WorkshoSim/UserInterface/Inventory/ItemDragDropOperation.h"


void UMainMenu::NativeOnInitialized()
{
    Super::NativeOnInitialized();
}

void UMainMenu::NativeConstruct()
{
    Super::NativeConstruct();

    PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
}

bool UMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    // return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

    const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

    if (PlayerCharacter && ItemDragDrop->SourceItem)
    {
        PlayerCharacter->DropItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity);
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("failed to drop item"));
        return false;
    }
}
