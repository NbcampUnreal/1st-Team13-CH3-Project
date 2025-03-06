#include "BPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h" 
#include "BGameInstance.h"
#include "BUIManager.h"
#include "BBaseItem.h"

ABPlayerState::ABPlayerState()
{
	DeathState = EDeathState::NotDead;
	NomalSpeed = 100.f;
	SprintSpeed= NomalSpeed * SprintMultiply;
	SprintMultiply = 6.f;
	CurrentHealth = MaxHealth = 100;
	AttackDamage = 0;
	Level = 1;
	MaxExperience = 0;
	CurrentExperience = 0;
	Coin = 4000;	
}

void ABPlayerState::BeginPlay()
{
	for (uint32 i = 1; i <= 100; ++i)
	{
		LevelTable.Add(i, (uint32)pow(((i + 2 - 1) * 50 / 49.f), 2.5f));
	}	
}

void ABPlayerState::AddCoin(const int32 _Coin)
{
	this->Coin += _Coin;

	if (UBGameInstance* Instance = Cast<UBGameInstance>(GetGameInstance()))
	{
		if (UBUIManager* UI = Instance->GetUIManagerInstance())
		{
			UI->DisplayNotification("Got Money!", FString::Printf(TEXT("Current Gold in possession: %d"), Coin));
		}
	}
}

int32 ABPlayerState::GetCoin() const
{
	return Coin;
}

float ABPlayerState::GetCurrentHealth() const
{
	return CurrentHealth;
}

float ABPlayerState::GetMaxHealth() const
{
	return MaxHealth;
}

float ABPlayerState::GetHealthNormalized() const
{
	return (float)(CurrentHealth) / (float)(MaxHealth);
}

int32 ABPlayerState::GetPlayerLevel() const
{
	return Level;
}

int32 ABPlayerState::GetCurrentExp() const
{
	return CurrentExperience;
}

int32 ABPlayerState::GetMaxExp() const
{
	return MaxExperience;
}



bool ABPlayerState::IsDeadOrDying() const
{
	return (DeathState > EDeathState::NotDead);
}

void ABPlayerState::StartDeath()
{
	if (DeathState != EDeathState::NotDead)
	{
		return;
	}

	DeathState = EDeathState::DeathStarted;

	check(Owner);

	// TODO: �ױ� �����ϸ� �˷�����Ѵ�.
	// Animation ����
}

void ABPlayerState::FinishDeath()
{
	if (DeathState != EDeathState::DeathStarted)
	{
		return;
	}

	DeathState = EDeathState::DeathFinished;
	
	check(Owner);

	// TODO: ���� ���� �˷�����Ѵ�.
	// ��ü ������ ��
}

float ABPlayerState::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	AddCurrentHealth(-DamageAmount);

	if (CurrentHealth <= 0)
	{
		StartDeath();
	}

	return ActualDamage;
}

void ABPlayerState::AddCurrentHealth(int32 Health)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Health, 0, MaxHealth);

	if (UBGameInstance* Instance = Cast<UBGameInstance>(GetGameInstance()))
	{
		if (UBUIManager* UI = Instance->GetUIManagerInstance())
		{
			UI->UpdateHUDHealth(CurrentHealth, MaxHealth);
		}
	}
}

void ABPlayerState::AddExp(int32 Exp)
{
	if (CurrentExperience + Exp < MaxExperience)
	{
		CurrentExperience += Exp;
		return;
	}
	CurrentExperience += Exp;
	while (CurrentExperience > MaxExperience)
	{
		CurrentExperience -= MaxExperience;
		LevelUP();
	}

	if (UBGameInstance* Instance = Cast<UBGameInstance>(GetGameInstance()))
	{
		if (UBUIManager* UI = Instance->GetUIManagerInstance())
		{
			UI->UpdateHUDLevelAndExp(Level, CurrentExperience, MaxExperience);
		}
	}
}

void ABPlayerState::Attack(AActor* Actor)
{
	UGameplayStatics::ApplyDamage(Actor,
		AttackDamage, // TODO : ���Ⱑ �ִٸ� ���� �������� �߰��ؼ� �ø��°� ������
		nullptr,
		this,
		UDamageType::StaticClass());
}

TArray<FItemData> ABPlayerState::GetNearItemArray() const
{	
	TArray<FItemData> Result;

	if (ABCharacter* Chr = Cast<ABCharacter>(GetOwner()))
	{
		for (const auto& Item : Chr->GetNearItemArray())
		{
			Result.Add(Item->GetItemData());
		}
	}
	return Result;
}

TArray<FItemData> ABPlayerState::GetAllInventoryItem() const
{
	TArray<FItemData> Items;

	for (const auto& Arr : Inventory)
	{
		for (const auto& Item : Arr.Value)
		{
			Items.Add(Item);
		}
	}
	return Items;
}

TArray<FItemData> ABPlayerState::GetInventoryTypeItem(const FName& ItemName) const
{
	TArray<FItemData> Items;

	if (const TArray<FItemData>* InventoryItems = Inventory.Find(ItemName))
	{
		for (const auto& Item : *InventoryItems)
		{
			Items.Add(Item);
		}
	}

	return Items;
}
TArray<FItemData> ABPlayerState::GetInventoryClassItem(const UClass* ItemClass) const
{
	TArray<FItemData> Items;

	if (!ItemClass) // null 체크
	{
		UE_LOG(LogTemp, Warning, TEXT("GetInventoryClassItem: ItemClass is null!"));
		return Items;
	}

	for (const auto& Pair : Inventory)
	{
		for (const auto& Item : Pair.Value)
		{
			if (Item.ItemClass && Item.ItemClass->IsChildOf(ItemClass))
			{
				Items.Add(Item);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %d items of class %s in inventory."), Items.Num(), *ItemClass->GetName());
	return Items;
}

void ABPlayerState::InventoryRemoveItem(const FItemData& Item)
{
	// 1️⃣ 해당 아이템이 Inventory에 있는지 확인
	TArray<FItemData>* Items = Inventory.Find(Item.ItemName);
	if (!Items || Items->IsEmpty())
	{
		return;
	}

	// 2️⃣ 마지막 아이템을 안전하게 삭제
	int32 LastIdx = Items->Num() - 1;
	Items->RemoveAt(LastIdx);


	// 3️⃣ 만약 아이템 배열이 비어 있으면, 맵에서 제거
	if (Items->IsEmpty())
	{
		Inventory.Remove(Item.ItemName);
		UpdateQuickSlot(Item.ItemName, 0);
	}
	else
	{
		// 4️⃣ 퀵슬롯 업데이트 (남아 있는 경우에만)
		UpdateQuickSlot(Item.ItemName, LastIdx);
	}
}


void ABPlayerState::InventoryAddItem(const FItemData& Item)
{
	Inventory.FindOrAdd(Item.ItemName).Add(Item);
	UpdateQuickSlot(Item.ItemName, Inventory[Item.ItemName].Num());

	if (UBGameInstance* Instance = Cast<UBGameInstance>(GetGameInstance()))
	{
		if (UBUIManager* UI = Instance->GetUIManagerInstance())
		{
			UI->DisplayItemNotification(Item.ItemName);
		}
	}
}

void ABPlayerState::UpdateQuickSlot(const FName& ItemName, int32 Count)
{
	if (UBGameInstance* Instance = Cast<UBGameInstance>(GetGameInstance()))
	{
		if (UBUIManager* UI = Instance->GetUIManagerInstance())
		{
			UI->UpdateHUDQuickSlot(ItemName, Count);
		}
	}
}

void ABPlayerState::UseItem(const FName& ItemName)
{
	if (Inventory.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템이 없다."));
		return;
	}
	if (Inventory[ItemName].IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템이 없다."));
		return;
	}

	if (ABPlayerController* Con = Cast<ABPlayerController>(GetOwner()))
	{
		if (ABCharacter* Chr = Cast<ABCharacter>(Con->GetCharacter()))
		{
			
			ABBaseItem* Base = GetWorld()->SpawnActor<ABBaseItem>(Inventory[ItemName].Last().ItemClass,FVector::ZeroVector,FRotator::ZeroRotator);
			if (Base)
			{
				Base->UseItem(Chr);
				Base->DestroyItem();
				InventoryRemoveItem(Inventory[ItemName].Last());
			}
		}
	}
}

void ABPlayerState::LevelUP()
{
	++Level;

	if (LevelTable[Level] != 0)
		MaxExperience = LevelTable[Level];

	MaxHealth = Level * 20;
	CurrentHealth = MaxHealth;

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("레벨업"));

	if (UBGameInstance* Instance = Cast<UBGameInstance>(GetGameInstance()))
	{
		if (UBUIManager* UI = Instance->GetUIManagerInstance())
		{
			UI->DisplayNotification("Level Up!", FString::Printf(TEXT("Current Level is %d"), Level));
		}
	}
}
