#pragma once
#include <cstring>
#include <iostream>
class Consumable
{
private:
    char* ConsumableName = nullptr;
    int Quantity = 0;
    float HealAmount = 0;
    bool IsMedicine = false;
    bool CanOD = false;
    bool IsDeadly = false;
    bool CanSurplus = false;
public:
    Consumable();
    Consumable(const char* CN, int Q, float HA, bool IM, bool COD, bool ID, bool CS);
    ~Consumable();
    bool Heal(float& CurrHealth, float MaxHealth);
};