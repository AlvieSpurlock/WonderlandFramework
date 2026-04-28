#pragma once
#include "Weapon.h"
#include "Armor.h"
#include "Consumable.h"
#include <cstring>
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
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
    float GetCurrHealth();
    float GetDamage() const;
    char* GetName() const;
    float GetHealthPercentage() const;


    //-----[Mutators]-----\\
    //Funcitons to Alter Private Vars

    void UpdateHealthPercentage();
    void SetCurrHealth(float newHealth);
    virtual void Hit(Character* Target);
    virtual void TakeDamage(float DamageTaken);
    void Death();
};
class CombatCharacter : public Character
{
private:
    Weapon* W = nullptr;
    Armor* A = nullptr;
    Consumable* C = nullptr;
    
    float HealLikelihood = 0;

public:
    CombatCharacter(const char* N, float Health, float D, float HL, Weapon& cW, Armor* cA = nullptr, Consumable* cC = nullptr)
        : Character(N, Health, D)
    {
        HealLikelihood = HL;
        W = &cW;
        A = cA;
        C = cC;
    }

    ~CombatCharacter() {}


    Weapon* GetWeapon() const { return W; }

    Armor* GetArmor() const { return A; }


    void Hit(Character* Target) override;

    void TakeDamage(float DamageTaken) override;


    void DecideToHeal();
};