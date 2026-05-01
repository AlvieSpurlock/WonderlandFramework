#pragma once
#include "Weapon.h"
#include "Armor.h"
#include "Consumable.h"

#include "Tick.h"
#include "Test_Grid.h"

#include <cstring>
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <algorithm>


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
    bool WasHitLastTick = false;

public:
    //-----[Obligatory]-----\\
    //Constructor & Deconstructor

    CombatCharacter(const char* N, float Health, float D, float HL, Weapon& cW, Armor* cA = nullptr, Consumable* cC = nullptr)
        : Character(N, Health, D)
    {
        HealLikelihood = HL;
        W = &cW;
        A = cA;
        C = cC;
    }
    ~CombatCharacter() {}

    //-----[Accessors]-----\\
    //Funcitons to Access Private Vars

    Weapon* GetWeapon() const { return W; }
    Armor* GetArmor() const { return A; }
    Consumable* GetConsumable() const { return C; }
    float GetHealLikelihood() const { return HealLikelihood; }
    bool GetWasHitLastTick() const { return WasHitLastTick; }


    //-----[Mutators]-----\\
    //Funcitons to Alter Private Vars

    void SetWasHitLastTick(bool Value) { WasHitLastTick = Value; }
    virtual void OnTick(int CurrentTick, Character* Target) {}
    void Hit(Character* Target) override;
    void TakeDamage(float DamageTaken) override;
    virtual void DecideToHeal();
    void DecideToAttack(Character* Target, int CurrentTick);
    float GetConsumableDanger()
    {
        float DangerScore = 0;
        if (GetConsumable() == nullptr) { return 0.0f; }
        if (GetConsumable()->GetCanOD()) { DangerScore += .5f; }
        if (GetConsumable()->GetIsDeadly()) { DangerScore += .5f; }
        return DangerScore;
    }
};


class AdvancedCombatCharacter : public CombatCharacter
{
private:
    std::vector<int> Location;
    int BaseRange = 0;
    int MoveSpeed = 0;

    bool IsBlocking = false;

public:
    //-----[Obligatory]-----\\
    //Constructor & Deconstructor
    AdvancedCombatCharacter(const char* N, float Health, float D, float HL, int X, int Y, int BR, int MS, Weapon& cW, Armor* cA = nullptr, Consumable* cC = nullptr, Tick* cT = nullptr)
        : CombatCharacter(N, Health, D, HL, cW, cA, cC)
    {
        Location.push_back(X);
        Location.push_back(Y);
        BaseRange = BR;
        MoveSpeed = MS;
    }
    ~AdvancedCombatCharacter() {}


    //-----[Accessors]-----\\
    //Funcitons to Access Private Vars

    std::vector<int> GetLocation() const { return Location; }
    int GetBaseRange() const { return BaseRange; }
    int GetMoveSpeed() const { return MoveSpeed; }


    //-----[Mutators]-----\\
    //Funcitons to Alter Private Vars

    void SetMoveSpeed(int NS) { MoveSpeed = NS; }

    void Move(int X, int Y)
    {
        Location.clear();
        Location.push_back(X);
        Location.push_back(Y);
        std::cout << GetName() << " moved to (" << X << ", " << Y << ")\n";
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
                DistanceMovedX++;
                CurrX = IsFrontX ? CurrX + 1 : CurrX - 1;
            }
        }
        if (abs(CurrY - Target[1]) > BaseRange)
        {
            while (DistanceMovedY < StepsY)
            {
                DistanceMovedY++;
                CurrY = IsFrontY ? CurrY + 1 : CurrY - 1;
            }
        }
        Move(CurrX, CurrY);
        if (abs(CurrX - Target[0]) <= BaseRange && abs(CurrY - Target[1]) <= BaseRange)
        { std::cout << GetName() << " In Range\n"; }
    }

    void Block()
    {
        IsBlocking = !IsBlocking;
        if (IsBlocking) { std::cout << GetName() << " Blocking!\n"; }
        else { std::cout << GetName() << " Canceling Block!\n"; }
    }

    void TakeDamage(float DamageTaken) override
    {
        if (!IsBlocking)
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
            Block();
        }
    }

    //-----[Tick Event]-----\\
    //Handles things every Tick

    virtual void OnTick(int CurrentTick, AdvancedCombatCharacter* Target);
};



//-----[AI Character]-----\\
//Autonomous AI driven OnTick

enum class PanicResponse
{
    Aggressive,          // 0.0  - 0.2  — low panic, full offense
    ReservedAggressive,  // 0.2  - 0.4  — mild panic, cautious offense
    DefensiveAggressive, // 0.4  - 0.6  — moderate panic, offense with retreat
    FullDefensive,       // 0.6  - 0.8  — high panic, pure survival
    Flee                 // 0.8  - 1.0  — full panic, runs
};

class AICharacter : public AdvancedCombatCharacter
{
private:
    float Panic = 0.0f;
    PanicResponse PR = PanicResponse::Aggressive;

public:
    //-----[Obligatory]-----\\
    //Constructor & Deconstructor

    AICharacter(const char* N, float Health, float D, float HL, int X, int Y, int BR, int MS, Weapon& cW, Armor* cA = nullptr, Consumable* cC = nullptr, Tick* cT = nullptr)
        : AdvancedCombatCharacter(N, Health, D, HL, X, Y, BR, MS, cW, cA, cC, cT) {
    }
    ~AICharacter() {}

    //-----[Accessors]-----\\
    //Funcitons to Access Private Vars
    float GetPanic() const { return Panic; }
    std::string GetPanicResponseString() const
    {
        switch (PR)
        {
        case PanicResponse::Aggressive:          return "Aggressive";
        case PanicResponse::ReservedAggressive:  return "Reserved Aggressive";
        case PanicResponse::DefensiveAggressive: return "Defensive Aggressive";
        case PanicResponse::FullDefensive:       return "Full Defensive";
        case PanicResponse::Flee:                return "Flee";
        default:                                 return "Unknown";
        }
    }

    //-----[Tick Event]-----\\
    //Handles things every Tick

    void OnTick(int CurrentTick, AdvancedCombatCharacter* Target) override;

    void EvaluatePanic()
    {
        Panic = 0.0f;
        std::mt19937 RNG(std::random_device{}());


        //-----[Health Panic]-----\\
        //Low health increases panic

        float HealthPanic = 1.0f - GetHealthPercentage();


        //-----[Supply Panic]-----\\
        //Low supplies increases panic

        float SupplyPanic = 0.0f;
        if (GetConsumable() != nullptr)
        {
            float Danger = GetConsumableDanger();
            float QuantityRisk = 1.0f - GetConsumable()->QuantityPercentage();
            float Threshold = 0.0f;
            if (Danger >= 1.0f)
            {
                std::uniform_real_distribution<float> LV(.15f, 0.4f);
                Threshold = LV(RNG);
            }
            else if (Danger >= 0.5f)
            {
                std::uniform_real_distribution<float> DV(.4f, 0.6f);
                Threshold = DV(RNG);
            }
            else
            {
                std::uniform_real_distribution<float> SV(.1f, 0.8f);
                Threshold = SV(RNG);
            }

            // High danger medicine running low = more panic
            SupplyPanic = std::min(1.0f, QuantityRisk * (1.0f + Danger));

            // Being below threshold adds urgency
            if (GetHealthPercentage() < Threshold)
            { SupplyPanic = std::min(1.0f, SupplyPanic + 0.2f); }
        }
        else
        { SupplyPanic = 1.0f; } // No consumable at all = always supply panicked

        //-----[Weapon Panic]-----\\
        //Broken or degraded weapon increases panic
        float WeaponPanic = 0.0f;
        if (GetWeapon() != nullptr)
        {
            float DurabilityRisk = 1.0f - GetWeapon()->GetCurrDurability() / GetWeapon()->GetMaxDurability();
            WeaponPanic = DurabilityRisk * 0.5f;
        }
        else
        { WeaponPanic = 1.0f; } // No weapon = highly panicked


        //-----[Armor Panic]-----\\
        //Broken or degraded armor increases panic

        float ArmorPanic = 0.0f;
        if (GetArmor() != nullptr)
        {
            float DurabilityRisk = 1.0f - GetArmor()->GetCurrDurability() / GetArmor()->GetMaxDurability();
            ArmorPanic = DurabilityRisk * 0.5f;
        }


        //-----[Combine]-----\\
        //Weight each factor by importance

        Panic = (HealthPanic * 0.4f) + (SupplyPanic * 0.3f) + (WeaponPanic * 0.2f) + (ArmorPanic * 0.1f);
        Panic = std::min(1.0f, std::max(0.0f, Panic));
    }

    void EvaluatePanicResponse()
    {
        EvaluatePanic();
        if (Panic < 0.2f) { PR = PanicResponse::Aggressive; }
        else if (Panic < 0.4f) { PR = PanicResponse::ReservedAggressive; }
        else if (Panic < 0.6f) { PR = PanicResponse::DefensiveAggressive; }
        else if (Panic < 0.8f) { PR = PanicResponse::FullDefensive; }
        else { PR = PanicResponse::Flee; }
    }


    //-----[Overrides]-----\\
    //Overrides Parent Functions

    void DecideToHeal() override
    {
        if (GetConsumable() == nullptr) { return; }
        std::mt19937 RNG(std::random_device{}());
        float HC = 0.0f;
        EvaluatePanicResponse();
        switch (PR)
        {
        case PanicResponse::Aggressive:
        {
            std::uniform_real_distribution<float> AC(.0f, .1f);
            HC = AC(RNG);
            break;
        }
        case PanicResponse::ReservedAggressive:
        {
            std::uniform_real_distribution<float> RAC(.2f, 0.35f);
            HC = RAC(RNG);
            break;
        }
        case PanicResponse::DefensiveAggressive:
        {
            std::uniform_real_distribution<float> DAC(.4f, 0.55f);
            HC = DAC(RNG);
            break;
        }
        case PanicResponse::FullDefensive:
        {
            std::uniform_real_distribution<float> FDC(.7f, 0.85f);
            HC = FDC(RNG);
            break;
        }
        case PanicResponse::Flee:
        {
            std::uniform_real_distribution<float> FC(.9f, 1.0f);
            HC = FC(RNG);
            break;
        }
        }
        if (GetHealthPercentage() < HC)
        {
            float CH = GetCurrHealth();
            if (!GetConsumable()->Heal(CH, GetMaxHealth()))
            { Death(); }
            SetCurrHealth(CH);
            UpdateHealthPercentage();
        }
    }

    void DecideToAttack(AdvancedCombatCharacter* Target, int CurrentTick)
    {
        std::mt19937 RNG(std::random_device{}());

        float AC = 0.0f;
        float MC = 0.0f;
        float BC = 0.0f;

        EvaluatePanicResponse();

        switch (PR)
        {
        case PanicResponse::Aggressive:
        {
            std::uniform_real_distribution<float> ACA(.0f, .1f);
            std::uniform_real_distribution<float> ACM(.9f, 1.0f);
            std::uniform_real_distribution<float> ACB(.9f, 1.0f);
            AC = ACA(RNG);
            MC = ACM(RNG);
            BC = ACB(RNG);
            break;
        }
        case PanicResponse::ReservedAggressive:
        {
            std::uniform_real_distribution<float> RACA(.2f, 0.35f);
            std::uniform_real_distribution<float> RACM(.9f, 1.0f);
            std::uniform_real_distribution<float> RACB(.7f, 0.85f);
            AC = RACA(RNG);
            MC = RACM(RNG);
            BC = RACB(RNG);
            break;
        }
        case PanicResponse::DefensiveAggressive:
        {
            std::uniform_real_distribution<float> DACA(.4f, 0.55f);
            std::uniform_real_distribution<float> DACM(.9f, 1.0f);
            std::uniform_real_distribution<float> DACB(.4f, 0.55f);
            AC = DACA(RNG);
            MC = DACM(RNG);
            BC = DACB(RNG);
            break;
        }
        case PanicResponse::FullDefensive:
        {
            std::uniform_real_distribution<float> FDCA(.7f, 0.85f);
            std::uniform_real_distribution<float> FDCM(.4f, 0.55f);
            std::uniform_real_distribution<float> FDCB(.2f, 0.35f);
            AC = FDCA(RNG);
            MC = FDCM(RNG);
            BC = FDCB(RNG);
            break;
        }
        case PanicResponse::Flee:
        {
            std::uniform_real_distribution<float> FCA(.9f, 1.0f);
            std::uniform_real_distribution<float> FCM(.0f, .1f);
            std::uniform_real_distribution<float> FCB(.2f, 0.35f);
            AC = FCA(RNG);
            MC = FCM(RNG);
            BC = FCB(RNG);
            break;
        }
        }
        std::uniform_real_distribution<float> P(0.0f, 1.0f);

        if (P(RNG) > AC) { Hit(Target); } 
        else 
        {
            if (P(RNG) > MC) { MoveAway(Target->GetLocation()); }
            else { if (P(RNG) > BC) { Block(); } }
        }
    }

    void MoveAway(std::vector<int> Target)
    {
        std::vector<int> Curr = GetLocation();
        // Move opposite direction from target
        int DX = Curr[0] - Target[0];
        int DY = Curr[1] - Target[1];
        Move(Curr[0] + DX, Curr[1] + DY);
    }
};


//-----[Player Character]-----\\
//Player Input driven OnTick

class PlayerCharacter : public AdvancedCombatCharacter
{
public:
    //-----[Obligatory]-----\\
    //Constructor & Deconstructor

    PlayerCharacter(const char* N, float Health, float D, float HL, int X, int Y, int BR, int MS, Weapon& cW, Armor* cA = nullptr, Consumable* cC = nullptr, Tick* cT = nullptr)
        : AdvancedCombatCharacter(N, Health, D, HL, X, Y, BR, MS, cW, cA, cC, cT) {}
    ~PlayerCharacter() {}

    void OnTick(int CurrentTick, AdvancedCombatCharacter* Target) override;
};