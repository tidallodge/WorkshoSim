// Fill out your copyright notice in the Description page of Project Settings.
#include "WorkshoSim/UserInterface/Inventory/InventoryPanel.h"
#include "WorkshoSim/UserInterface/Inventory/InventoryItemSlot.h"
#include "WorkshoSim/Components/InventoryComponent.h"
#include "WorkshoSim/PlayerCharacter.h"

#include "Components/TextBlock.h"
#include "Components/WrapBox.h"

void UInventoryPanel::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
    if (PlayerCharacter)
    {
        InventoryReference = PlayerCharacter->GetInventory();
        if (InventoryReference)
        {
            InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory);
            SetInfoText();
        }
    }   
}

void UInventoryPanel::SetInfoText() const
{
    const FString WeightInfoValue{
        FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight()) + "/" 
        + FString::SanitizeFloat(InventoryReference->GetWeightCapactiy())
    };
    const FString CapacityInfoValue{
        FString::SanitizeFloat(InventoryReference->GetInventoryContents().Num()) + "/" 
        + FString::SanitizeFloat(InventoryReference->GetSlotsCapacity())
    };

    WeightInfo->SetText(FText::FromString(WeightInfoValue));
    CapacityInfo->SetText(FText::FromString(CapacityInfoValue));
}

void UInventoryPanel::RefreshInventory()
{
    if (InventoryReference && InventorySlotClass)
    {
        InventoryWrapBox->ClearChildren();
        
        for (UItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
        {
            UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
            ItemSlot->SetItemReference(InventoryItem);
            
            InventoryWrapBox->AddChildToWrapBox(ItemSlot);
        }

        SetInfoText();
    }
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
