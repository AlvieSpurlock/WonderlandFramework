#include "Armor.h"


//-----[Obligatory]-----\\
//Constructor & Deconstructor

Armor::Armor() {}

Armor::Armor(const char* N, float Durability, float D, float F, bool II, bool IB, bool ID, bool TD, bool IE)
{
	size_t Len = std::strlen(N) + 1;
	ArmorName = new char[Len];
	strcpy_s(ArmorName, Len, N);
	MaxDurability = Durability;
	CurrDurability = Durability;
	UpdateDurabilityPercentage();
	BaseDamageDeduction = D;
	EffectiveDamageDeduction = BaseDamageDeduction;
	if (F < 0.1f) { Fragility = 0.1f; }
	else if (F > 2.5f) { Fragility = 2.5f; }
	else { Fragility = F; }
	IsImmortal = II;
	IsBroken = IB;
	TotalDeduction = TD;
	IsDynamic = ID;
	IsEquipped = IE;
}

Armor::~Armor()
{ delete[] ArmorName; }


//-----[Mutators]-----\\
//Funcitons to Alter Private Vars

void Armor::HandleEquip()
{ IsEquipped = !IsEquipped; }

float Armor::ApplyDeduction(float IncomingDamage, float DurabilityDamage)
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
	{ IncomingDamage -= EffectiveDamageDeduction; }
	else
	{
		if (!TotalDeduction)
		{
			static std::mt19937 RNG(std::random_device{}());
			std::uniform_real_distribution<float> Dist(2.0f, 5.0f);
			IncomingDamage -= EffectiveDamageDeduction / 2.0f + Dist(RNG);
		}
		else { std::cout << "Incoming Damage: " << IncomingDamage << "\n"; return IncomingDamage; }
	}  std::cout << "Incoming Damage: " << IncomingDamage << "\n"; return std::max(IncomingDamage, 0.0f);
}
void Armor::UpdateDurabilityPercentage()
{
	DurabilityPercentage = (CurrDurability / MaxDurability);
	if (IsDynamic)
	{
		EffectiveDamageDeduction = BaseDamageDeduction * DurabilityPercentage;
	}
}
