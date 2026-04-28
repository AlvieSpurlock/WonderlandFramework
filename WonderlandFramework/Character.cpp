#include "Character.h"

//====={Character Base}=====\\

//-----[Obligatory]-----\\
//Constructor & Deconstructor

Character::Character() {}

Character::Character(const char* N, float Health, float D)
{
	size_t Len = std::strlen(N) + 1;
	Name = new char[Len];
	strcpy_s(Name, Len, N);
	MaxHealth = Health;
	CurrHealth = Health;
	UpdateHealthPercentage();
	Damage = D;
}

Character::~Character()
{
	delete[] Name;
}


//-----[Accessors]-----\\
//Funcitons to Access Private Vars

float Character::GetMaxHealth() const
{ return MaxHealth; }

float Character::GetCurrHealth() const
{ return CurrHealth; }


//-----[Mutators]-----\\
//Funcitons to Alter Private Vars

void Character::UpdateHealthPercentage()
{ HealthPercentage = (CurrHealth / MaxHealth); }

void Character::Hit(Character* Target)
{ Target->TakeDamage(Damage); }

void Character::TakeDamage(float DamageTaken)
{
	if (CurrHealth - DamageTaken > 0)
	{
		CurrHealth = CurrHealth - DamageTaken;
		UpdateHealthPercentage();
	}
	else
	{
		CurrHealth = 0;
		UpdateHealthPercentage();
		Death();
	}
	std::cout << Name << ": " << CurrHealth << "(" << HealthPercentage << ")" << "\n";
}

void Character::Death()
{ std::cout << Name << " has DIED!\n"; }




//====={Combat Character}=====\\

void CombatCharacter::Hit(Character* Target)
{
	static std::mt19937 RNG(std::random_device{}());
	std::uniform_int_distribution<int> Dist(10, 20);
	Target->TakeDamage(W->ApplyMulti(GetDamage(), (W->GetMaxDurability() / 2 + Dist(RNG)) * W->GetFragility()));
}