#pragma once
#include "Weapon.h"
#include "Armor.h"
#include "Consumable.h"
#include "Tick.h"
#include "World_2D_Grid.h"

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
    Tick* T = nullptr;
    float HealLikelihood = 0;
    bool WasHitLastTick = false;

public:
    CombatCharacter(const char* N, float Health, float D, float HL, Weapon& cW, Armor* cA = nullptr, Consumable* cC = nullptr, Tick* cT = nullptr)
        : Character(N, Health, D)
    {
        HealLikelihood = HL;
        W = &cW;
        A = cA;
        C = cC;
        T = cT;
    }
    ~CombatCharacter() {}
    Weapon* GetWeapon() const { return W; }
    Armor* GetArmor() const { return A; }
    Consumable* GetConsumable() const { return C; }
    Tick* GetTick() const { return T; }
    float GetHealLikelihood() const { return HealLikelihood; }
    bool GetWasHitLastTick() const { return WasHitLastTick; }
    void SetWasHitLastTick(bool Value) { WasHitLastTick = Value; }
    virtual void OnTick(int CurrentTick, Character* Target) = 0;
    void Hit(Character* Target) override;
    void TakeDamage(float DamageTaken) override;
    void DecideToHeal();
    void DecideToAttack(Character* Target, int CurrentTick);
};


class AdvancedCombatCharacter : public CombatCharacter
{
private:
    std::vector<int> Location;

    int BaseRange = 0;
    int MoveSpeed = 0;

    Map* M = nullptr;

    std::string Genre = "fantasy";
    std::string TimePeriod = "medieval";
    std::string Vibe = "dark and gritty";

public:
    //-----[Obligatory]-----\\
    //Constructor & Deconstructor
    AdvancedCombatCharacter(const char* N, float Health, float D, float HL, int X, int Y, int BR, int MS, Weapon& cW, Armor* cA = nullptr, Consumable* cC = nullptr, Tick* cT = nullptr, Map* cM = nullptr)
        : CombatCharacter(N, Health, D, HL, cW, cA, cC, cT)
    {
        Location.push_back(X);
        Location.push_back(Y);
        BaseRange = BR;
        MoveSpeed = MS;
        M = cM;
    }
    ~AdvancedCombatCharacter() {}
    
    
    //-----[Accessors]-----\\
    //Funcitons to Access Private Vars
    
    std::vector<int> GetLocation() const { return Location; }
    int GetBaseRange() const { return BaseRange; }
    int GetMoveSpeed() const { return MoveSpeed; }
    Map* GetMap() const { return M; }
    std::string GetGenre() const { return Genre; }
    std::string GetTimePeriod() const { return TimePeriod; }
    std::string GetVibe() const { return Vibe; }
    
    
    //-----[Mutators]-----\\
    //Funcitons to Alter Private Vars
    
    void SetGenre(const std::string& G) { Genre = G; }
    void SetTimePeriod(const std::string& TP) { TimePeriod = TP; }
    void SetVibe(const std::string& V) { Vibe = V; }
    void Move(int X, int Y)
    {
        if (M != nullptr && !M->CanMoveTo(X, Y)) { return; }
        
        Location.clear();
        Location.push_back(X);
        Location.push_back(Y);

        if (M != nullptr)
        { M->DescribeSurroundings(X, Y); }
    }
    void MoveTo(std::vector<int> Target)
    {
        std::vector<int> Curr = GetLocation();

        int StepsX = MoveSpeed / 2;
        int StepsY = MoveSpeed - StepsX;
        int CurrX = Curr[0]; int CurrY = Curr[1];

        bool IsFrontX = Curr[0] < Target[0];
        bool IsFrontY = Curr[1] < Target[1];

        int DistanceMovedX = 0;
        int DistanceMovedY = 0;

        if (abs(CurrX - Target[0]) > BaseRange)
        {
            while (DistanceMovedX < StepsX)
            {
                int NextX = IsFrontX ? CurrX + 1 : CurrX - 1;
                if (M != nullptr && !M->CanMoveTo(NextX, CurrY)) { break; }
                DistanceMovedX++;
                CurrX = NextX;
            }
        }
        if (abs(CurrY - Target[1]) > BaseRange)
        {
            while (DistanceMovedY < StepsY)
            {
                int NextY = IsFrontY ? CurrY + 1 : CurrY - 1;
                if (M != nullptr && !M->CanMoveTo(CurrX, NextY)) { break; }
                DistanceMovedY++;
                CurrY = NextY;
            }
        }
        Move(CurrX, CurrY);
        if (abs(CurrX - Target[0]) <= BaseRange && abs(CurrY - Target[1]) <= BaseRange)
        { std::cout << GetName() << " In Range\n"; }
    }


    //-----[World Description]-----\\
    //Queries Map directly for surroundings and Ollama description

    void DescribeSurroundings()
    {
        if (M != nullptr)
        { M->DescribeSurroundings(Location[0], Location[1]); }
    }

    void GetOllamaDescription()
    {
        if (M == nullptr) { return; }
        if (GetTick() != nullptr) { GetTick()->Stop(); }
        std::string Description = M->BuildOllamaContext(Location[0], Location[1], Genre, TimePeriod, Vibe);
        std::cout << "\n" << Description << "\n";
        if (GetTick() != nullptr) { GetTick()->Start(); }
    }
};



//-----[AI Character]-----\\
//Autonomous AI driven OnTick

class AICharacter : public AdvancedCombatCharacter
{
public:
    //-----[Obligatory]-----\\
    //Constructor & Deconstructor

    AICharacter(const char* N, float Health, float D, float HL, int X, int Y, int BR, int MS, Weapon& cW, Armor* cA = nullptr, Consumable* cC = nullptr, Tick* cT = nullptr, Map* cM = nullptr)
        : AdvancedCombatCharacter(N, Health, D, HL, X, Y, BR, MS, cW, cA, cC, cT, cM) { }
   
    ~AICharacter() {}

    void OnTick(int CurrentTick, Character* Target) override;
};



//-----[Player Character]-----\\
//Player Input driven OnTick

class PlayerCharacter : public AdvancedCombatCharacter
{
public:
    //-----[Obligatory]-----\\
    //Constructor & Deconstructor

    PlayerCharacter(const char* N, float Health, float D, float HL, int X, int Y, int BR, int MS, Weapon& cW, Armor* cA = nullptr, Consumable* cC = nullptr, Tick* cT = nullptr, Map* cM = nullptr)
        : AdvancedCombatCharacter(N, Health, D, HL, X, Y, BR, MS, cW, cA, cC, cT, cM) {}
    
    ~PlayerCharacter() {}
    
    void OnTick(int CurrentTick, Character* Target) override;
};