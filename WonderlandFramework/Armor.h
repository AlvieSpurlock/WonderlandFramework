#pragma once
#include <cstring>
#include <iostream>
#include <random>
#include <algorithm>
class Armor
{
private:
	char* ArmorName = nullptr; //char* for better Memory Management
	
	float MaxDurability = 0; // Max Durability Allowed
	float CurrDurability = 0; // Current Durability
	float DurabilityPercentage = 0; // Durability Percentage
	
	float BaseDamageDeduction = 0; // Base Damage Deduction for Dynamic Deduction
	float EffectiveDamageDeduction = 0; // Effective Damage Deduction that CAN be Dynamic
	
	float Fragility = 0; // For Applying Damage to Durability
	
	bool IsEquipped = true; // Toggle for switching Armor
	bool IsImmortal = false; // Toggle for Simulation(RPG Style) and Arcade, made decision while coding
	bool IsBroken = false; // Check for Broken Functionality if NOT Immortal
	bool TotalDeduction = false; // Toggle for Simulation(RPG Style) and Arcade, made decision while coding
	bool IsDynamic = false; // Toggle for Simulation(RPG Style) and Arcade, made decision while coding

public:
	//-----[Obligatory]-----\\
	//Constructor & Deconstructor

	Armor();
	Armor(const char* N, float Durability, float D, float F, bool II, bool IB, bool ID, bool TD, bool IE);
	~Armor();


	//-----[Accessors]-----\\
	//Funcitons to Access Private Vars
	
	float GetMaxDurability() const { return MaxDurability; }
	float GetCurrDurability() const { return CurrDurability; }
	float GetFragility() const { return Fragility; }
	
	
	//-----[Mutators]-----\\
	//Funcitons to Alter Private Vars

	void HandleEquip();
	float ApplyDeduction(float IncomingDamage, float DurabilityDamage);
	void UpdateDurabilityPercentage();
};