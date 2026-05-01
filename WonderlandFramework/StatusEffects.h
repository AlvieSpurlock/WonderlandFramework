#pragma once

#include "Character.h"
#include "Test_Grid.h"

#include <cstring>
#include <iostream>
#include <vector>


class StatusEffect
{
private:
    char* Name = nullptr;

    float Duration = 0.0f;
    float Intensity = 0.0f;

    bool DoesDamage = false;
    float Damage = 0.0f;

    bool DoesDOT = false;
    float DamageRate = 0.0f;

    bool DoesAOE = false;
    int DamageRange = 0;

    bool IsActive = false;

    bool DoesStack = false;

    bool AffectsMovement = false;
    float MovementMultiplier = 1.0f;

    bool DoesSpread = false;
    float SpreadChance = 0.0f;

    char* ComboPartner = nullptr;

public:
    StatusEffect() {}
    ~StatusEffect() { delete[] Name; delete[] ComboPartner; }


    //-----[Accessors]-----\\

    char* GetName() const { return Name; }
    float GetDuration() const { return Duration; }
    float GetIntensity() const { return Intensity; }
    bool GetDoesDamage() const { return DoesDamage; }
    float GetDamage() const { return Damage; }
    bool GetDoesDOT() const { return DoesDOT; }
    float GetDamageRate() const { return DamageRate; }
    bool GetIsActive() const { return IsActive; }
    bool GetDoesStack() const { return DoesStack; }
    bool GetAffectsMovement() const { return AffectsMovement; }
    float GetMovementMultiplier() const { return MovementMultiplier; }
    bool GetDoesSpread() const { return DoesSpread; }
    float GetSpreadChance() const { return SpreadChance; }
    char* GetComboPartner() const { return ComboPartner; }


    //-----[Mutators]-----\\

    void SetName(const char* N)
    {
        delete[] Name;
        size_t Len = std::strlen(N) + 1;
        Name = new char[Len];
        strcpy_s(Name, Len, N);
    }
    void SetDuration(float D) { Duration = D; }
    void SetIntensity(float I) { Intensity = I; }
    void SetDoesDamage(bool DD) { DoesDamage = DD; }
    void SetDamage(float D) { Damage = D; }
    void SetDoesDOT(bool DDOT) { DoesDOT = DDOT; }
    void SetDamageRate(float DR) { DamageRate = DR; }
    void SetIsActive(bool IA) { IsActive = IA; }
    void SetDoesStack(bool DS) { DoesStack = DS; }
    void SetAffectsMovement(bool AM) { AffectsMovement = AM; }
    void SetMovementMultiplier(float MM) { MovementMultiplier = MM; }
    void SetDoesSpread(bool DS) { DoesSpread = DS; }
    void SetSpreadChance(float SC) { SpreadChance = SC; }
    void SetComboPartner(const char* CP)
    {
        delete[] ComboPartner;
        size_t Len = std::strlen(CP) + 1;
        ComboPartner = new char[Len];
        strcpy_s(ComboPartner, Len, CP);
    }


    //-----[Effect Functions]-----\\

    void Stack(StatusEffect SE)
    { Intensity += SE.GetIntensity(); }

    void AffectMovement(AdvancedCombatCharacter C)
    { C.SetMoveSpeed(C.GetMoveSpeed() * MovementMultiplier); }

    void ApplyDamage(AdvancedCombatCharacter C)
    {
        C.SetCurrHealth(C.GetCurrHealth() - (Damage * Intensity));
    }

    void ApplyAOE(std::vector<int> Origin, std::vector<AdvancedCombatCharacter*> NearbyCharacters)
    {
        if (!DoesAOE) { return; }
        for (auto& C : NearbyCharacters)
        {
            if (C == nullptr) { continue; }
            std::vector<int> Loc = C->GetLocation();
            int DistX = abs(Loc[0] - Origin[0]);
            int DistY = abs(Loc[1] - Origin[1]);
            if (DistX <= DamageRange && DistY <= DamageRange)
            {
                std::cout << GetName() << " AOE Hit: " << C->GetName() << "\n";
                C->TakeDamage(Damage * Intensity);
            }
        }
    }


    //-----[Tick]-----\\

    virtual void OnTick(float& CurrHealth)
    {

    }
};

