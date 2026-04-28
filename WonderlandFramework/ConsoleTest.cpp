// This is for Console Engine Tests
// This will be where all new features are tested before expanded upon

#include "TestLog_Helper.h"
#include "TestNotes_Helper.h"
#include "Test_UI_Helper.h"
#include "Character.h"
#include "Weapon.h"
#include "Armor.h"
#include "Consumable.h"

#include <thread>
#include <chrono>
#include <random>

static std::mt19937 RNG(std::random_device{}());

//-----[User Unput Loop]-----\\
//Core User Input Loop for recursion and Input Validation

void UserInputLoop(TestLog_Helper* TL, TestNotes_Helper* TN, Test_UI_Helper* UI)
{
    std::cout << "Test or Make New?\nT = Test, M = Make New, N = View Notes\n";
    char Answer;
    std::cin >> Answer;
    Answer = (char)std::toupper(static_cast<unsigned char>(Answer));
    if (Answer == 'T')
    {
        std::cout << "Which Feature are we testing today?\n";
        UI->PrintFeatures(TL->Features);
        int Choice = UI->GetUserSelection(TL->Features.size());
        switch (Choice)
        {
        case 0:
            UI->TestEnd(TL->Features[Choice]);
            TL->SaveTestLog(TL->Features[Choice]);
            break;
        case 1:
        {
            std::uniform_int_distribution<int> Dist(7, 15);
            Character* Hero = new Character("Hero", 100, Dist(RNG));
            Character* Villain = new Character("Villain", 100, Dist(RNG));
            bool LoopSwitch = true;
            while (Hero->GetCurrHealth() > 0 && Villain->GetCurrHealth() > 0)
            {
                if (LoopSwitch) { Hero->Hit(Villain); }
                else { Villain->Hit(Hero); }
                LoopSwitch = !LoopSwitch;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            } UI->TestEnd(TL->Features[Choice]);
            TL->SaveTestLog(TL->Features[Choice]);
            delete Hero;
            delete Villain;
            break;
        }
        case 2:
        {
            std::uniform_real_distribution<float> wdDist(1.25f, 2.5f);
            std::uniform_real_distribution<float> wfDist(0.1f, 1.25f);
            std::uniform_int_distribution<int> Dist(7, 15);
            Weapon* HeroWeapon = new Weapon("Hero Weapon", 100, wdDist(RNG), wfDist(RNG), false, false, true, false, true);
            Weapon* VillainWeapon = new Weapon("Villain Weapon", 100, wdDist(RNG), wfDist(RNG), false, false, true, false, true);
            CombatCharacter* Hero = new CombatCharacter("Hero", 100, Dist(RNG), .7, *HeroWeapon);
            CombatCharacter* Villain = new CombatCharacter("Villain", 100, Dist(RNG), .5, *VillainWeapon);
            bool LoopSwitch = true;
            for (size_t index = 0; index < 3; ++index)
            {
                if (Hero->GetCurrHealth() > 0 && Villain->GetCurrHealth() > 0)
                {
                    if (LoopSwitch)
                    {
                        Hero->Hit(Villain);
                        std::cout << "Hero Weapon Fragility: " << Hero->GetWeapon()->GetFragility() << "\n";
                    }
                    else
                    {
                        Villain->Hit(Hero);
                        std::cout << "Villain Weapon Fragility: " << Villain->GetWeapon()->GetFragility() << "\n";
                    }
                    LoopSwitch = !LoopSwitch;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
            }
            UI->TestEnd(TL->Features[Choice]);
            TL->SaveTestLog(TL->Features[Choice]);
            delete Hero;
            delete Villain;
            delete HeroWeapon;
            delete VillainWeapon;
            break;
        }
        case 3:
        {
            std::uniform_real_distribution<float> wdDist(1.25f, 2.5f);
            std::uniform_real_distribution<float> wfDist(0.1f, 1.25f);
            std::uniform_int_distribution<int> Dist(7, 15);
            Weapon* HeroWeapon = new Weapon("Hero Weapon", 100, wdDist(RNG), wfDist(RNG), false, false, true, false, true);
            Weapon* VillainWeapon = new Weapon("Villain Weapon", 100, wdDist(RNG), wfDist(RNG), false, false, true, false, true);
            Armor* HeroArmor = new Armor("Hero Armor", 100, wdDist(RNG), wfDist(RNG), false, false, true, false, true);
            Armor* VillainArmor = new Armor("Villain Armor", 100, wdDist(RNG), wfDist(RNG), false, false, true, false, true);
            CombatCharacter* Hero = new CombatCharacter("Hero", 100, Dist(RNG), .7, *HeroWeapon, HeroArmor);
            CombatCharacter* Villain = new CombatCharacter("Villain", 100, Dist(RNG), .5, *VillainWeapon, VillainArmor);
            bool LoopSwitch = true;
            for (size_t index = 0; index < 3; ++index)
            {
                if (Hero->GetCurrHealth() > 0 && Villain->GetCurrHealth() > 0)
                {
                    if (LoopSwitch)
                    {
                        Hero->Hit(Villain);
                        std::cout << "Hero Weapon Fragility: " << Hero->GetWeapon()->GetFragility() << "\n";
                        std::cout << "Hero Armor Fragility: " << Hero->GetArmor()->GetFragility() << "\n";
                    }
                    else
                    {
                        Villain->Hit(Hero);
                        std::cout << "Villain Weapon Fragility: " << Villain->GetWeapon()->GetFragility() << "\n";
                        std::cout << "Villain Armor Fragility: " << Villain->GetArmor()->GetFragility() << "\n";
                    }
                    LoopSwitch = !LoopSwitch;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
            }
            UI->TestEnd(TL->Features[Choice]);
            TL->SaveTestLog(TL->Features[Choice]);
            delete Hero;
            delete Villain;
            delete HeroWeapon;
            delete VillainWeapon;
            delete HeroArmor;
            delete VillainArmor;
            break;
        }
        case 4:
        {
            std::uniform_real_distribution<float> wdDist(1.25f, 2.5f);
            std::uniform_real_distribution<float> wfDist(0.1f, 1.25f);
            std::uniform_int_distribution<int> Dist(7, 15);
            Weapon* HeroWeapon = new Weapon("Hero Weapon", 100, wdDist(RNG), wfDist(RNG), false, false, true, false, true);
            Weapon* VillainWeapon = new Weapon("Villain Weapon", 100, wdDist(RNG), wfDist(RNG), false, false, true, false, true);
            Armor* HeroArmor = new Armor("Hero Armor", 100, wdDist(RNG), wfDist(RNG), false, false, true, false, true);
            Armor* VillainArmor = new Armor("Villain Armor", 100, wdDist(RNG), wfDist(RNG), false, false, true, false, true);
            Consumable* HeroConsumable = new Consumable("Hero Consumable", 5, 20, true, true, true, false);
            Consumable* VillainConsumable = new Consumable("Villain Consumable", 5, 20, true, true, false, true);
            CombatCharacter* Hero = new CombatCharacter("Hero", 100, Dist(RNG), .7, *HeroWeapon, HeroArmor, HeroConsumable);
            CombatCharacter* Villain = new CombatCharacter("Villain", 100, Dist(RNG), .5, *VillainWeapon, VillainArmor, VillainConsumable);
            std::cout << "Case 4 Running\n";
            std::cout << "Hero Heal Likelihood: " << .7 << "\n";
            std::cout << "Villain Heal Likelihood: " << .5 << "\n";
            bool LoopSwitch = true;
            for (size_t index = 0; index < 3; ++index)
            {
                std::cout << "Round " << index + 1 << "\n";
                if (Hero->GetCurrHealth() > 0 && Villain->GetCurrHealth() > 0)
                {
                    if (LoopSwitch)
                    {
                        Hero->Hit(Villain);
                        std::cout << "Hero Weapon Fragility: " << Hero->GetWeapon()->GetFragility() << "\n";
                        std::cout << "Hero Armor Fragility: " << Hero->GetArmor()->GetFragility() << "\n";
                    }
                    else
                    {
                        Villain->Hit(Hero);
                        std::cout << "Villain Weapon Fragility: " << Villain->GetWeapon()->GetFragility() << "\n";
                        std::cout << "Villain Armor Fragility: " << Villain->GetArmor()->GetFragility() << "\n";
                    }
                    LoopSwitch = !LoopSwitch;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
            }
            UI->TestEnd(TL->Features[Choice]);
            TL->SaveTestLog(TL->Features[Choice]);
            delete Hero;
            delete Villain;
            delete HeroWeapon;
            delete VillainWeapon;
            delete HeroArmor;
            delete VillainArmor;
            delete HeroConsumable;
            delete VillainConsumable;
            break;
        }
        }
    }
    else
    {
        if (Answer == 'M')
        {
            TL->MakeFeature("", 0, 0, 0, true);
            UI->PrintFeatures(TL->Features);
            UserInputLoop(TL, TN, UI);
        }
        else
        {
            if (Answer == 'N')
            {
                UI->PrintFeatures(TL->Features);
                std::cout << "Which Feature are we looking at today?\n";
                int Choice = UI->GetUserSelection(TL->Features.size());
                UI->ViewNotes(TN->MakeDirectory(TL->Features[Choice].Name));
            }
            else
            {
                std::cout << " Incorrect Input! Try Again!\n";
                UserInputLoop(TL, TN, UI);
            }
        }
    }
}

//-----[Main]-----\\
//Used to run the Debug Console itself

int main()
{
    TestLog_Helper* TL = new TestLog_Helper();
    TestNotes_Helper* TN = new TestNotes_Helper();
    Test_UI_Helper* UI = new Test_UI_Helper();
    TL->LoadTestLog();
    UI->PrintFeatures(TL->Features);
    UserInputLoop(TL, TN, UI);
    delete TL;
    delete TN;
    delete UI;
    return 0;
}