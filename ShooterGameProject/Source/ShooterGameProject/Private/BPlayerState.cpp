#include "BPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h" 

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
	Coin = 0;	
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
}

void ABPlayerState::Attack(AActor* Actor)
{
	UGameplayStatics::ApplyDamage(Actor,
		AttackDamage, // TODO : ���Ⱑ �ִٸ� ���� �������� �߰��ؼ� �ø��°� ������
		nullptr,
		this,
		UDamageType::StaticClass());
}

void ABPlayerState::LevelUP()
{
	++Level;

	if (LevelTable[Level] != 0)
		MaxExperience = LevelTable[Level];

	MaxHealth = Level * 20;
	CurrentHealth = MaxHealth;
}
