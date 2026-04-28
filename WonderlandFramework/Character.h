#pragma once
#include "Weapon.h"
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
	//-----[Mutators]-----\\
	//Funcitons to Alter Private Vars
	void UpdateHealthPercentage();
	virtual void Hit(Character* Target);
	void TakeDamage(float DamageTaken);
	void Death();
};
class CombatCharacter : public Character
{
private:
	Weapon* W = nullptr;
public:
	CombatCharacter(const char* N, float Health, float D, Weapon& cW)
		: Character(N, Health, D)
	{
		W = &cW;
	}
	~CombatCharacter() {}
	
	Weapon* GetWeapon() const { return W; } 
	void Hit(Character* Target) override;
};