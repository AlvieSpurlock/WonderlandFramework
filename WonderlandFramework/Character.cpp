#include "Character.h"
//====={Character Base}=====\\
//-----[Obligatory]-----\\
//Constructor & Deconstructor
Character::Character() {}
Character::Character(const char* N, float Health, float D)
{
    size_t Len = std::strlen(N) + 1;
    Name = new char[Len];
    strcpy_s(Name, Len, N);
    MaxHealth = Health;
    CurrHealth = Health;
    UpdateHealthPercentage();
    Damage = D;
}
Character::~Character()
{
    delete[] Name;
}
//-----[Accessors]-----\\
//Funcitons to Access Private Vars
float Character::GetMaxHealth() const
{
    return MaxHealth;
}
float Character::GetCurrHealth()
{
    return CurrHealth;
}
float Character::GetDamage() const
{
    return Damage;
}
char* Character::GetName() const
{
    return Name;
}
float Character::GetHealthPercentage() const
{
    return HealthPercentage;
}
//-----[Mutators]-----\\
//Funcitons to Alter Private Vars
void Character::UpdateHealthPercentage()
{
    HealthPercentage = (CurrHealth / MaxHealth);
}
void Character::SetCurrHealth(float newHealth)
{
    CurrHealth = newHealth;
}
void Character::Hit(Character* Target)
{
    Target->TakeDamage(Damage);
}
void Character::TakeDamage(float DamageTaken)
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
void Character::Death()
{
    std::cout << Name << " has DIED!\n";
}
//====={Combat Character}=====\\

void CombatCharacter::Hit(Character* Target)
{
    std::mt19937 RNG(std::random_device{}());
    std::uniform_int_distribution<int> Dist(10, 20);
    Target->TakeDamage(W->ApplyMulti(GetDamage(), (W->GetMaxDurability() / 2 + Dist(RNG)) * W->GetFragility()));
}
void CombatCharacter::TakeDamage(float DamageTaken)
{
    if (A != nullptr)
    {
        std::mt19937 RNG(std::random_device{}());
        std::uniform_int_distribution<int> Dist(10, 20);
        DamageTaken = A->ApplyDeduction(DamageTaken, (A->GetMaxDurability() / 2 + Dist(RNG)) * A->GetFragility());
        if (GetCurrHealth() - DamageTaken > 0)
        {
            SetCurrHealth(GetCurrHealth() - DamageTaken);
            UpdateHealthPercentage();
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            std::cout << "Deciding Whether to Heal\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            DecideToHeal();
        }
        else
        {
            SetCurrHealth(0);
            UpdateHealthPercentage();
            Death();
        }
        std::cout << GetName() << ": " << GetCurrHealth() << "(" << GetHealthPercentage() << ")" << "\n";
    }
    else
    {
        if (GetCurrHealth() - DamageTaken > 0)
        {
            SetCurrHealth(GetCurrHealth() - DamageTaken);
            UpdateHealthPercentage();
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            std::cout << "Deciding Whether to Heal\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            DecideToHeal();
        }
        else
        {
            SetCurrHealth(0);
            UpdateHealthPercentage();
            Death();
        }
        std::cout << GetName() << ": " << GetCurrHealth() << "(" << GetHealthPercentage() << ")" << "\n";
    }
}
void CombatCharacter::DecideToHeal()
{
    std::cout << GetName() << " DecideToHeal Called\n";
    if (C == nullptr) { std::cout << GetName() << " No Consumable!\n"; return; }
    std::mt19937 RNG(std::random_device{}());
    std::uniform_real_distribution<float> Dist(0.0f, 1.0f);
    float Roll = Dist(RNG);
    std::cout << GetName() << " Heal Roll: " << Roll << " Likelihood: " << HealLikelihood << "\n";
    if (Roll <= HealLikelihood)
    {
        float CH = GetCurrHealth();
        if (!C->Heal(CH, GetMaxHealth()))
        {
            Death();
        }
        SetCurrHealth(CH);
        UpdateHealthPercentage();
    }
}