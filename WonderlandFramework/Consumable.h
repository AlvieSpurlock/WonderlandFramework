#pragma once
#include <cstring>
#include <iostream>
class Consumable
{
private:
    char* ConsumableName = nullptr;
    int MaxQuantity = 0;
    int Quantity = 0;
    float HealAmount = 0;
    float DecayRate = 0;
    bool IsMedicine = false;
    bool CanOD = false;
    bool IsDeadly = false;
    bool CanSurplus = false;
    bool IsDecay = true;

public:

    Consumable();
    Consumable(const char* CN, int Q, float HA, bool IM, bool COD, bool ID, bool CS);
    ~Consumable();
    bool Heal(float& CurrHealth, float MaxHealth);

    bool GetIsMedicine() const { return IsMedicine; }
    bool GetCanOD() const { return CanOD; }
    bool GetIsDeadly() const { return IsDeadly; }
    bool GetCanSurplus() const { return CanSurplus; }
    bool GetIsDecay() const { return IsDecay; }
    float GetDecayRate() const { return DecayRate; }
    float QuantityPercentage()
    {
        if (Quantity > 0) { return Quantity / MaxQuantity; }
        else { return 0.0f; }
    }
};