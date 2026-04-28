#pragma once

#include "Weapon.h"
#include "Armor.h"

#include <cstring>
#include <iostream>

class Character
{
private:
	char* Name = nullptr;
	float MaxHealth = 100;
	float CurrHealth = 100;
	float HealthPercentage = 1;
	float Damage = 10;
public:
	//-----[Obligatory]-----\\
	//Constructor & Deconstructor

	Character();
	Character(const char* N, float Health, float D);
	~Character();


	//-----[Accessors]-----\\
	//Funcitons to Access Private Vars

	float GetMaxHealth() const;
	float GetCurrHealth() const;
	float GetDamage() const { return Damage; }
	char* GetName() const { return Name; }
	float GetHealthPercentage() const { return HealthPercentage; }


	//-----[Mutators]-----\\
	//Funcitons to Alter Private Vars

	void UpdateHealthPercentage();
	void SetCurrHealth(float newHealth) { CurrHealth = newHealth; }
	virtual void Hit(Character* Target);
	virtual void TakeDamage(float DamageTaken);
	void Death();
};
class CombatCharacter : public Character
{
private:
	Weapon* W = nullptr;
	Armor* A = nullptr;
public:
	CombatCharacter(const char* N, float Health, float D, Weapon& cW, Armor* cA = nullptr)
		: Character(N, Health, D)
	{
		W = &cW;
		A = cA;
	}
	~CombatCharacter() {}
	
	Weapon* GetWeapon() const { return W; } 
	Armor* GetArmor() const { return A; }
	void Hit(Character* Target) override;
	void TakeDamage(float DamageTaken) override
	{
		if (A != nullptr)
		{
			static std::mt19937 RNG(std::random_device{}());
			std::uniform_int_distribution<int> Dist(10, 20);
			DamageTaken = A->ApplyDeduction(DamageTaken, (A->GetMaxDurability() / 2 + Dist(RNG)) * A->GetFragility());
			if (GetCurrHealth() - DamageTaken > 0)
			{
				SetCurrHealth(GetCurrHealth() - DamageTaken);
				UpdateHealthPercentage();
			}
			else
			{
				SetCurrHealth(0);
				UpdateHealthPercentage();
				Death();
			}
			std::cout << GetName() << ": " << GetCurrHealth() << "(" << GetHealthPercentage() << ")" << "\n";
		}
		else
		{
			if (GetCurrHealth() - DamageTaken > 0)
			{
				SetCurrHealth(GetCurrHealth() - DamageTaken);
				UpdateHealthPercentage();
			}
			else
			{
				SetCurrHealth(0);
				UpdateHealthPercentage();
				Death();
			}
			std::cout << GetName() << ": " << GetCurrHealth() << "(" << GetHealthPercentage() << ")" << "\n";
		}
	}
};