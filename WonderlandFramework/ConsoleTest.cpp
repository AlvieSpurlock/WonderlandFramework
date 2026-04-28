// This is for Console Engine Tests
// This will be where all new features are tested before expanded upon

#include "TestLog_Helper.h"
#include "TestNotes_Helper.h"
#include "Test_UI_Helper.h"
#include "Character.h"

#include <thread>
#include <chrono>


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
            Character * Hero = new Character("Hero", 100, 7 + (rand() % (15 - 7 + 1)));
            Character* Villain = new Character("Villain", 100, 7 + (rand() % (15 - 7 + 1)));

            bool LoopSwitch = true;

            while (Hero->GetCurrHealth() > 0 && Villain->GetCurrHealth() > 0)
            {
                if (LoopSwitch)
                { Hero->Hit(Villain); }
                else { Villain->Hit(Hero); }
                LoopSwitch = !LoopSwitch;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            } UI->TestEnd(TL->Features[Choice]);
            TL->SaveTestLog(TL->Features[Choice]);
            break;
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