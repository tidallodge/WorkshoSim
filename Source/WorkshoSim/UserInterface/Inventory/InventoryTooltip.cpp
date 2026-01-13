// Fill out your copyright notice in the Description page of Project Settings.
#include "WorkshoSim/UserInterface/Inventory/InventoryTooltip.h"
#include "WorkshoSim/UserInterface/Inventory/InventoryItemSlot.h"
#include "WorkshoSim/Items/ItemBase.h"

#include "Components/TextBlock.h"

void UInventoryTooltip::NativeConstruct()
{
    Super::NativeConstruct();

    if (InventorySlotBeingHovered)
    {
        UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

        switch(ItemBeingHovered->ItemType)
        {
        case EItemType::Armor:
            break;
        case EItemType::Weapon:
            break;
        case EItemType::Shield:
            break;
        case EItemType::Spell:
            break;
        case EItemType::Consumable:
            ItemType->SetText(FText::FromString("Consumable"));
            DamageValue->SetVisibility(ESlateVisibility::Collapsed);
            ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
            break;
        case EItemType::Quest:
            break;
        case EItemType::Mundane:
            ItemType->SetText(FText::FromString("Mundane Item"));
            DamageValue->SetVisibility(ESlateVisibility::Collapsed);
            ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
            UsageText->SetVisibility(ESlateVisibility::Collapsed);
            break;
        }

        ItemName->SetText(ItemBeingHovered->TextData.Name);
        DamageValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStats.DamageValue));
        ArmorRating->SetText(FText::AsNumber(ItemBeingHovered->ItemStats.ArmorRating));
        UsageText->SetText(ItemBeingHovered->TextData.UsageText);
        ItemDescription->SetText(ItemBeingHovered->TextData.Description);
        SellValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStats.SellValue));

        const FString WeightInfo =
            {"Weight: " + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight())};
        StackWeight->SetText(FText::FromString(WeightInfo));


        if (ItemBeingHovered->NumericData.bIsStackable)
        {
            const FString StackInfo =
                {"Max Stack Size: " + FString::FromInt(ItemBeingHovered->NumericData.MaxStackSize)};

            MaxStackSize->SetText(FText::FromString(StackInfo));
        }
        else
        {
            MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}
