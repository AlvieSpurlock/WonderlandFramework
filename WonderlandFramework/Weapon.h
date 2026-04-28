#pragma once
#include <cstring>
#include <iostream>
#include <random>
class Weapon
{
private:
	char* WeaponName = nullptr; //char* for better Memory Management
	
	float MaxDurability = 0; // Max Durability Allowed
	float CurrDurability = 0; // Current Durability
	float DurabilityPercentage = 0; // Durability Percentage
	
	float BaseDamageMulti = 0; // Base Damage Multi for Dynamic Multi
	float EffectiveDamageMulti = 0; // Effect Damage Multi that CAN be Dynamic
	
	float Fragility = 0; // For Applying Damage to Durability
	
	bool IsEquipped = true; // Toggle for switching Weapons
	bool IsImmortal = false; // Toggle for Simulation(RPG Style) and Arcade, made decision while coding
	bool IsBroken = false; // Check for Broken Functionality if NOT Immortal
	bool TotalDeduction = false; // Toggle for Simulation(RPG Style) and Arcade, made decision while coding
	bool IsDynamic = false; // Toggle for Simulation(RPG Style) and Arcade, made decision while coding

public:
	//-----[Obligatory]-----\\
	//Constructor & Deconstructor

	Weapon();
	Weapon(const char* N, float Durability, float D, float F, bool II, bool IB, bool ID, bool TD, bool IE);
	~Weapon();


	//-----[Accessors]-----\\
	//Funcitons to Access Private Vars

	float GetMaxDurability() const { return MaxDurability; }
	float GetCurrDurability() const { return CurrDurability; }
	float GetFragility() const { return Fragility; }


	//-----[Mutators]-----\\
	//Funcitons to Alter Private Vars

	void HandleEquip();
	float ApplyMulti(float BaseDamage, float DurabilityDamage);
	void UpdateDurabilityPercentage();
};