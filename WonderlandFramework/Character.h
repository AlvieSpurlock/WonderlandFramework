#pragma once

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

	Character(){} //Obligatory Default Constructor
	
	Character(const char* N, float Health, float D) //Actual Constructor
	{
		size_t Len = std::strlen(N) + 1;
		Name = new char[Len];
		strcpy_s(Name, Len, N);

		MaxHealth = Health;
		CurrHealth = Health;
		UpdateHealthPercentage();
		Damage = D;
	}

	~Character() //Deconstructor
	{ delete[] Name; }

	//-----[Accessors]-----\\
	//Funcitons to Access Private Vars

	float GetMaxHealth() const
	{ return MaxHealth; }

	float GetCurrHealth() const
	{ return CurrHealth; }

	//-----[Mutators]-----\\
	//Funcitons to Alter Private Vars

	void UpdateHealthPercentage()
	{ HealthPercentage = (CurrHealth / MaxHealth); }


	void Hit(Character* Target)
	{ Target->TakeDamage(Damage); }

	void TakeDamage(float DamageTaken)
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

	void Death()
	{ std::cout << Name << " has DIED!\n"; }
};