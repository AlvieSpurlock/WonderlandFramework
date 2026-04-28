#include "Consumable.h"


Consumable::Consumable() {}

Consumable::Consumable(const char* CN, int Q, float HA, bool IM, bool COD, bool ID, bool CS)
{
    size_t Len = std::strlen(CN) + 1;
    ConsumableName = new char[Len];
    strcpy_s(ConsumableName, Len, CN);
    Quantity = Q;
    HealAmount = HA;
    IsMedicine = IM;
    CanOD = COD;
    IsDeadly = ID;
    CanSurplus = CS;
}

Consumable::~Consumable()
{ delete[] ConsumableName; }


bool Consumable::Heal(float& CurrHealth, float MaxHealth)
{
    if (Quantity <= 0) { std::cout << "No " << ConsumableName << " Left!\n"; return true; }
    Quantity--;
    if (!IsMedicine)
    {
        if (MaxHealth < (CurrHealth + HealAmount))
        {
            CurrHealth = MaxHealth;
            std::cout << ConsumableName << " Healed to Max!\n";
        }
        else
        {
            CurrHealth = CurrHealth + HealAmount;
            std::cout << ConsumableName << " Healed " << HealAmount << "!\n";
        }
    }
    else
    {
        if (MaxHealth < (CurrHealth + HealAmount))
        {
            if (CanOD)
            {
                if (IsDeadly)
                {
                    float Excess = (CurrHealth + HealAmount) - MaxHealth;
                    CurrHealth -= Excess;
                    std::cout << ConsumableName << " Overdose! Damaged " << Excess << "!\n";
                    if (CurrHealth <= 0)
                    {
                        CurrHealth = 0;
                        std::cout << ConsumableName << " Killed Consumer!\n";
                        return false;
                    }
                }
                else
                {
                    CurrHealth = MaxHealth / 2;
                    std::cout << ConsumableName << " Overdose! Health halved!\n";
                }
            }
            else
            {
                if (!CanSurplus)
                {
                    CurrHealth = MaxHealth;
                    std::cout << ConsumableName << " Healed to Max!\n";
                }
                else
                {
                    CurrHealth += HealAmount;
                    std::cout << ConsumableName << " Surplus Heal! Health at " << CurrHealth << "!\n";
                }
            }
        }
        else
        {
            if (MaxHealth < (CurrHealth + HealAmount))
            {
                CurrHealth = MaxHealth;
                std::cout << ConsumableName << " Healed to Max!\n";
            }
            else
            {
                CurrHealth = CurrHealth + HealAmount;
                std::cout << ConsumableName << " Healed " << HealAmount << "!\n";
            }
        }
    } return true;
}