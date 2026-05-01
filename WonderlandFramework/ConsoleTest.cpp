// This is for Console Engine Tests
// This will be where all new features are tested before expanded upon

#include "Test_Grid.h"

#include "TestLog_Helper.h"
#include "TestNotes_Helper.h"
#include "Test_UI_Helper.h"
#include "Character.h"
#include "Weapon.h"
#include "Armor.h"
#include "Consumable.h"

#include "Tick.h"

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
        std::string CurrentSubtest = "";
        try
        {
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
                Tick* T = new Tick(500, 1.0f);
                T->Register([&](int tick)
                    {
                        if (Hero->GetCurrHealth() <= 0 || Villain->GetCurrHealth() <= 0 || tick > 10)
                        {
                            T->Stop(); return;
                        }
                        std::cout << "\n|-------[Tick " << tick << "]-------|\n";
                        if (tick % 2 == 0)
                        {
                            std::cout << "//=====[Hero]=====\\\\\n";
                            Hero->Hit(Villain);
                        }
                        else
                        {
                            std::cout << "//=====[Villain]=====\\\\\n";
                            Villain->Hit(Hero);
                        }
                    });
                T->Run();
                UI->TestEnd(TL->Features[Choice]);
                TL->SaveTestLog(TL->Features[Choice]);
                delete Hero;
                delete Villain;
                delete T;
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
                Tick* T = new Tick(500, 1.0f);
                T->Register([&](int tick)
                    {
                        if (Hero->GetCurrHealth() <= 0 || Villain->GetCurrHealth() <= 0 || tick > 10)
                        {
                            T->Stop(); return;
                        }
                        std::cout << "\n|-------[Tick " << tick << "]-------|\n";
                        if (tick % 2 == 0)
                        {
                            std::cout << "//=====[Hero]=====\\\\\n";
                            Hero->Hit(Villain);
                            std::cout << "Weapon Fragility: " << Hero->GetWeapon()->GetFragility() << "\n";
                        }
                        else
                        {
                            std::cout << "//=====[Villain]=====\\\\\n";
                            Villain->Hit(Hero);
                            std::cout << "Weapon Fragility: " << Villain->GetWeapon()->GetFragility() << "\n";
                        }
                    });
                T->Run();
                UI->TestEnd(TL->Features[Choice]);
                TL->SaveTestLog(TL->Features[Choice]);
                delete Hero;
                delete Villain;
                delete HeroWeapon;
                delete VillainWeapon;
                delete T;
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
                Tick* T = new Tick(500, 1.0f);
                T->Register([&](int tick)
                    {
                        if (Hero->GetCurrHealth() <= 0 || Villain->GetCurrHealth() <= 0 || tick > 10)
                        {
                            T->Stop(); return;
                        }
                        std::cout << "\n|-------[Tick " << tick << "]-------|\n";
                        if (tick % 2 == 0)
                        {
                            std::cout << "//=====[Hero]=====\\\\\n";
                            Hero->Hit(Villain);
                            std::cout << "Weapon Fragility: " << Hero->GetWeapon()->GetFragility() << "\n";
                            std::cout << "Armor Fragility: " << Hero->GetArmor()->GetFragility() << "\n";
                        }
                        else
                        {
                            std::cout << "//=====[Villain]=====\\\\\n";
                            Villain->Hit(Hero);
                            std::cout << "Weapon Fragility: " << Villain->GetWeapon()->GetFragility() << "\n";
                            std::cout << "Armor Fragility: " << Villain->GetArmor()->GetFragility() << "\n";
                        }
                    });
                T->Run();
                UI->TestEnd(TL->Features[Choice]);
                TL->SaveTestLog(TL->Features[Choice]);
                delete Hero;
                delete Villain;
                delete HeroWeapon;
                delete VillainWeapon;
                delete HeroArmor;
                delete VillainArmor;
                delete T;
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
                Tick* T = new Tick(500, 1.0f);
                T->Register([&](int tick)
                    {
                        if (Hero->GetCurrHealth() <= 0 || Villain->GetCurrHealth() <= 0 || tick > 10)
                        {
                            T->Stop(); return;
                        }
                        std::cout << "\n|-------[Tick " << tick << "]-------|\n";
                        if (tick % 2 == 0)
                        {
                            std::cout << "//=====[Hero]=====\\\\\n";
                            Hero->Hit(Villain);
                            std::cout << "Weapon Fragility: " << Hero->GetWeapon()->GetFragility() << "\n";
                            std::cout << "Armor Fragility: " << Hero->GetArmor()->GetFragility() << "\n";
                        }
                        else
                        {
                            std::cout << "//=====[Villain]=====\\\\\n";
                            Villain->Hit(Hero);
                            std::cout << "Weapon Fragility: " << Villain->GetWeapon()->GetFragility() << "\n";
                            std::cout << "Armor Fragility: " << Villain->GetArmor()->GetFragility() << "\n";
                        }
                    });
                T->Run();
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
                delete T;
                break;
            }
            case 5:
            {
                // Full AI test — panic system, blocking, movement, consumables
                std::uniform_real_distribution<float> wdDist(1.25f, 2.5f);
                std::uniform_real_distribution<float> wfDist(0.1f, 1.25f);
                std::uniform_int_distribution<int> Dist(7, 15);
                Weapon* HeroWeapon = new Weapon("Hero Weapon", 100, wdDist(RNG), wfDist(RNG), false, false, true, false, true);
                Weapon* VillainWeapon = new Weapon("Villain Weapon", 100, wdDist(RNG), wfDist(RNG), false, false, true, false, true);
                Armor* HeroArmor = new Armor("Hero Armor", 100, wdDist(RNG), wfDist(RNG), false, false, true, false, true);
                Armor* VillainArmor = new Armor("Villain Armor", 100, wdDist(RNG), wfDist(RNG), false, false, true, false, true);
                Consumable* HeroConsumable = new Consumable("Hero Consumable", 5, 20, true, true, true, false);
                Consumable* VillainConsumable = new Consumable("Villain Consumable", 5, 20, true, true, false, true);
                Tick* T = new Tick(500, 1.0f);
                AICharacter* Hero = new AICharacter("Hero", 100, Dist(RNG), .7, 0, 0, 2, 4, *HeroWeapon, HeroArmor, HeroConsumable, T);
                AICharacter* Villain = new AICharacter("Villain", 100, Dist(RNG), .5, 10, 0, 2, 4, *VillainWeapon, VillainArmor, VillainConsumable, T);
                GridVisualizer GV;
                GV.Init(
                    {
                        {Hero->GetName(),    Hero->GetLocation()[0], Hero->GetLocation()[1]},
                        {Villain->GetName(), Villain->GetLocation()[0], Villain->GetLocation()[1]}
                    }, 20, 20);
                GV.Launch();
                T->Register([&](int tick)
                    {
                        if (Hero->GetCurrHealth() <= 0 || Villain->GetCurrHealth() <= 0 || tick > 20)
                        {
                            T->Stop(); return;
                        }
                        std::cout << "\n|-------[Tick " << tick << "]-------|\n";
                        std::cout << "//=====[Hero]=====\\\\\n";
                        Hero->OnTick(tick, Villain);
                        std::cout << "Panic: " << Hero->GetPanic() << " | Response: " << Hero->GetPanicResponseString() << "\n";
                        std::cout << "Location: (" << Hero->GetLocation()[0] << ", " << Hero->GetLocation()[1] << ")\n";
                        std::cout << "Weapon Fragility: " << Hero->GetWeapon()->GetFragility() << "\n";
                        std::cout << "Armor Fragility: " << Hero->GetArmor()->GetFragility() << "\n";
                        std::cout << "//=====[Villain]=====\\\\\n";
                        Villain->OnTick(tick, Hero);
                        std::cout << "Panic: " << Villain->GetPanic() << " | Response: " << Villain->GetPanicResponseString() << "\n";
                        std::cout << "Location: (" << Villain->GetLocation()[0] << ", " << Villain->GetLocation()[1] << ")\n";
                        std::cout << "Weapon Fragility: " << Villain->GetWeapon()->GetFragility() << "\n";
                        std::cout << "Armor Fragility: " << Villain->GetArmor()->GetFragility() << "\n";

                        //-----[Grid Visualizer Update]-----\\

                        std::vector<CharacterMarker> Characters =
                        {
                            {Hero->GetName(),    Hero->GetLocation()[0], Hero->GetLocation()[1]},
                            {Villain->GetName(), Villain->GetLocation()[0], Villain->GetLocation()[1]}
                        };
                        std::vector<std::tuple<int, int, std::string>> TileUpdates;
                        // Wire status effects here when implemented
                        // Example: TileUpdates.push_back({Hero->GetLocation()[0], Hero->GetLocation()[1], "Fire"});
                        GV.OnTick(Characters, TileUpdates);
                    });
                T->Run();
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
                delete T;
                break;
            }
            case 6:
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
                Tick* T = new Tick(500, 1.0f);
                PlayerCharacter* Hero = new PlayerCharacter("Hero", 100, Dist(RNG), .7, 0, 0, 2, 4, *HeroWeapon, HeroArmor, HeroConsumable, T);
                AICharacter* Villain = new AICharacter("Villain", 100, Dist(RNG), .5, 10, 0, 2, 4, *VillainWeapon, VillainArmor, VillainConsumable, T);
                GridVisualizer GV;
                GV.Init(
                    {
                        {Hero->GetName(),    Hero->GetLocation()[0], Hero->GetLocation()[1]},
                        {Villain->GetName(), Villain->GetLocation()[0], Villain->GetLocation()[1]}
                    }, 20, 20);
                GV.Launch();
                T->Register([&](int tick)
                    {
                        if (Hero->GetCurrHealth() <= 0 || Villain->GetCurrHealth() <= 0 || tick > 5)
                        {
                            T->Stop(); return;
                        }
                        std::cout << "\n|-------[Tick " << tick << "]-------|\n";
                        std::cout << "//=====[Villain]=====\\\\\n";
                        Villain->OnTick(tick, Hero);
                        std::cout << "Panic: " << Villain->GetPanic() << " | Response: " << Villain->GetPanicResponseString() << "\n";
                        std::cout << "Location: (" << Villain->GetLocation()[0] << ", " << Villain->GetLocation()[1] << ")\n";
                        std::cout << "Weapon Fragility: " << Villain->GetWeapon()->GetFragility() << "\n";
                        std::cout << "Armor Fragility: " << Villain->GetArmor()->GetFragility() << "\n";
                        std::cout << "//=====[Hero - Your Turn]=====\\\\\n";
                        std::cout << "Health: " << Hero->GetCurrHealth() << " | Location: (" << Hero->GetLocation()[0] << ", " << Hero->GetLocation()[1] << ")\n";
                        std::cout << "Enemy Health: " << Villain->GetCurrHealth() << " | Enemy Location: (" << Villain->GetLocation()[0] << ", " << Villain->GetLocation()[1] << ")\n";
                        std::cout << "1. Attack\n2. Move Toward\n3. Heal\n4. Block\n";
                        T->Stop();
                        char PlayerChoice;
                        std::cin >> PlayerChoice;
                        switch (PlayerChoice)
                        {
                        case '1':
                            Hero->Hit(Villain);
                            break;
                        case '2':
                            Hero->MoveTo(Villain->GetLocation());
                            break;
                        case '3':
                            Hero->DecideToHeal();
                            break;
                        case '4':
                            Hero->Block();
                            break;
                        default:
                            std::cout << "Invalid Input!\n";
                            break;
                        }

                        //-----[Grid Visualizer Update]-----\\

                        std::vector<CharacterMarker> Characters =
                        {
                            {Hero->GetName(),    Hero->GetLocation()[0], Hero->GetLocation()[1]},
                            {Villain->GetName(), Villain->GetLocation()[0], Villain->GetLocation()[1]}
                        };
                        std::vector<std::tuple<int, int, std::string>> TileUpdates;
                        // Wire status effects here when implemented
                        GV.OnTick(Characters, TileUpdates);
                        T->Start();
                    });
                T->Run();
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
                delete T;
                break;
            }
            case 7:
            {
                // Surplus + Decay test — AI consumes at full health, decays back down
                std::uniform_real_distribution<float> wdDist(1.25f, 2.5f);
                std::uniform_real_distribution<float> wfDist(0.1f, 1.25f);
                std::uniform_int_distribution<int> Dist(7, 15);
                Weapon* HeroWeapon = new Weapon("Hero Weapon", 100, wdDist(RNG), wfDist(RNG), false, false, true, false, true);
                Consumable* HeroConsumable = new Consumable("Surplus Consumable", 5, 50, true, false, false, true);
                Tick* T = new Tick(500, 1.0f);
                AICharacter* Hero = new AICharacter("Hero", 100, Dist(RNG), 1.0f, 0, 0, 2, 4, *HeroWeapon, nullptr, HeroConsumable, T);
                T->Register([&](int tick)
                    {
                        if (tick > 20) { T->Stop(); return; }
                        std::cout << "\n|-------[Tick " << tick << "]-------|\n";
                        std::cout << "//=====[Hero]=====\\\\\n";
                        // Force heal at tick 1 regardless of health
                        if (tick == 1)
                        {
                            float CH = Hero->GetCurrHealth();
                            if (!HeroConsumable->Heal(CH, Hero->GetMaxHealth()))
                            { Hero->Death(); }
                            Hero->SetCurrHealth(CH);
                            Hero->UpdateHealthPercentage();
                            std::cout << "Hero consumed Surplus Consumable!\n";
                        }
                        Hero->OnTick(tick, Hero);
                        std::cout << "Health: " << Hero->GetCurrHealth() << "(" << Hero->GetHealthPercentage() << ")\n";
                        std::cout << "Consumable Quantity: " << HeroConsumable->QuantityPercentage() << "\n";
                    });
                T->Run();
                UI->TestEnd(TL->Features[Choice]);
                TL->SaveTestLog(TL->Features[Choice]);
                delete Hero;
                delete HeroWeapon;
                delete HeroConsumable;
                delete T;
                break;
            }
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Test Failed: " << e.what() << "\n";
            if (!CurrentSubtest.empty())
            {
                std::cout << "Failed in subtest: " << CurrentSubtest << "\n";
                TN->SaveNote("Test Failed: " + std::string(e.what()), TN->MakeDirectory("StatusEffects_" + CurrentSubtest));
            }
            std::cout << "Falling back to TestEnd\n";
            UI->TestEnd(TL->Features[Choice]);
            TL->SaveTestLog(TL->Features[Choice]);
        }
        catch (...)
        {
            std::cout << "Test Failed: Unknown Error\n";
            if (!CurrentSubtest.empty())
            {
                std::cout << "Failed in subtest: " << CurrentSubtest << "\n";
                TN->SaveNote("Test Failed: Unknown Error", TN->MakeDirectory("StatusEffects_" + CurrentSubtest));
            }
            std::cout << "Falling back to TestEnd\n";
            UI->TestEnd(TL->Features[Choice]);
            TL->SaveTestLog(TL->Features[Choice]);
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