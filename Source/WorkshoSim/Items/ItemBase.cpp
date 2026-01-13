// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "WorkshoSim/Components/InventoryComponent.h"


UItemBase::UItemBase() : bIsCopy(false), bIsPickup(false)
{

}

void UItemBase::ResetItemFlags()
{
    bIsCopy = false;
    bIsPickup = false;
}

UItemBase* UItemBase::CreateItemCopy() const
{
    UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

    ItemCopy->ID = this->ID;
    ItemCopy->Quantity = this->Quantity;
    ItemCopy->ItemQuality = this->ItemQuality;
    ItemCopy->ItemType = this->ItemType;
    ItemCopy->TextData = this->TextData;
    ItemCopy->NumericData = this->NumericData;
    ItemCopy->ItemStats = this->ItemStats;
    ItemCopy->AssetData = this->AssetData;
    ItemCopy->bIsCopy = true;

    return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
    UE_LOG(LogTemp, Warning, TEXT("SetQuantity called."));
    if (NewQuantity != Quantity)
    {
        Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackable ? NumericData.MaxStackSize : 1);
    
        if (OwningInventory)
        {
            if (Quantity <= 0)
            {
                OwningInventory->RemoveSingleInstanceOfItem(this);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ItemBase OwningInventory is null"));
        }
    }   
}

void UItemBase::Use(APlayerCharacter* character)
{
    
}
