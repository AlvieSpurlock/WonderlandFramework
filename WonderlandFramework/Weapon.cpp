#include "Weapon.h"


//-----[Obligatory]-----\\
//Constructor & Deconstructor

Weapon::Weapon() {}

Weapon::Weapon(const char* N, float Durability, float D, float F, bool II, bool IB, bool ID, bool TD, bool IE)
{
	size_t Len = std::strlen(N) + 1;
	WeaponName = new char[Len];
	strcpy_s(WeaponName, Len, N);
	MaxDurability = Durability;
	CurrDurability = Durability;
	UpdateDurabilityPercentage();
	BaseDamageMulti = D;
	EffectiveDamageMulti = BaseDamageMulti;
	if (F < 0.1f) { Fragility = 0.1f; }
	else if (F > 2.5f) { Fragility = 2.5f; }
	else { Fragility = F; }
	IsImmortal = II;
	IsBroken = IB;
	TotalDeduction = TD;
	IsDynamic = ID;
	IsEquipped = IE;
}

Weapon::~Weapon()
{
	delete[] WeaponName;
}


//-----[Mutators]-----\\
//Funcitons to Alter Private Vars

void Weapon::HandleEquip() 
{ 
	IsEquipped = !IsEquipped; 
	if(IsEquipped){ std::cout << WeaponName << " Equipped!"; }
	else { std::cout << WeaponName << " Unequipped!"; }
}

float Weapon::ApplyMulti(float BaseDamage, float DurabilityDamage)
{
	if (!IsImmortal)
	{
		if (!IsBroken)
		{
			if (CurrDurability - DurabilityDamage > 0)
			{
				CurrDurability -= DurabilityDamage;
				UpdateDurabilityPercentage();
			}
			else
			{
				CurrDurability = 0;
				UpdateDurabilityPercentage();
				IsBroken = true;
			}
		}
	}
	if (!IsBroken)
	{ BaseDamage *= EffectiveDamageMulti; }
	else
	{
		if (!TotalDeduction)
		{
			static std::mt19937 RNG(std::random_device{}());
			std::uniform_int_distribution<int> Dist(2, 5);
			BaseDamage *= EffectiveDamageMulti / 2 + Dist(RNG);
		}
		else 
		{ 
			std::cout << "DamageMulti: " << BaseDamage << "\n"; 
			return BaseDamage; 
		}
	} 
	std::cout << "Damage: " << BaseDamage << "\n"
		<< "Durability: " << DurabilityPercentage * 100 << "%\n";
	return BaseDamage;
}

void Weapon::UpdateDurabilityPercentage()
{
	DurabilityPercentage = (CurrDurability / MaxDurability);
	if (IsDynamic)
	{ EffectiveDamageMulti = BaseDamageMulti * DurabilityPercentage; }
}