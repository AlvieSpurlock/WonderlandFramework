#pragma once

#include "Test_Structs.h"
#include "TestNotes_Helper.h"

#include <vector>
#include <string>
#include <iostream>
#include <limits>

class Test_UI_Helper
{
public:
    Test_UI_Helper() {} //Obligitory Default Constructor


    //-----[Print Features Function]-----\\
    //Displays the Features from the TestLog.csv File

    static void PrintFeatures(const std::vector<Feature>& vF)
    {
        for (size_t index = 0, count = vF.size(); index < count; ++index)
        {
            std::cout << index + 1 << ".)" << "||" << vF[index].Name << "||" << vF[index].TestCount << "||" << vF[index].SuccessCount << "/" << vF[index].FailureCount << "||" << vF[index].Directory << "||" << std::endl;
        }
    }

    //-----[Display Note Dates Function]-----\\
    //Displays the Dates in the Notes for the User to Select

    static void DisplayNoteDates(const std::vector<NoteEntry> Notes)
    {
        for (size_t i = 0; i < Notes.size(); ++i)
        {
            std::cout << i + 1 << ".) " << Notes[i].Timestamp << "\n";
        }
    }

    //-----[Get User Selection Function]-----\\
    //Prompts, Receives, and Verifies User Input for Choice Based Interaction

    static int GetUserSelection(int Max)
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        int Choice;
        while (true)
        {
            std::cout << "\nPlease Select an Option by Number: ";
            if (std::cin >> Choice)
            {
                if (Choice >= 1 && Choice <= Max)
                {
                    return Choice - 1;
                }
                std::cout << "Invalid Selection!\n";
            }
            else
            {
                std::cout << "Please enter a valid number.\n";
                std::cin.clear();
                std::cin.ignore(1000, '\n');
            }
        }
    }

    //-----[Display Note Function]-----\\
    //Displays the Chosen Note

    static void DisplayNote(const NoteEntry N)
    {
        std::cout << "\n----- " << N.Timestamp << " -----\n";
        std::cout << N.Text << "\n";
    }

    //-----[View Notes Function]-----\\
    //Runs the Display Note Dates and Display Note Functions
    //as well as any other required Functions to let the 
    //User view their notes

    static void ViewNotes(const std::string& Directory)
    {
        TestNotes_Helper* TN = new TestNotes_Helper();
        std::vector<NoteEntry> Notes = TN->LoadNotes(Directory);
        if (Notes.empty())
        {
            std::cout << "No notes found.\n";
            return;
        }
        DisplayNoteDates(Notes);
        int index = GetUserSelection(Notes.size());
        DisplayNote(Notes[index]);
        delete TN;
    }


    void TestEnd(Feature& F)
    {
        std::cout << "Success or Failure?\nS = Success, F = Failure\n";
        char Answer;
        std::cin >> Answer;
        Answer = (char)std::toupper(static_cast<unsigned char>(Answer));
        if (Answer == 'S')
        { UpdateFeature(F, true); }
        else
        {
            if (Answer == 'F')
            { UpdateFeature(F, false); }
            else
            { std::cout << "Invalid Output\n"; }
        }
    }
    void UpdateFeature(Feature& F, bool Success)
    {
        std::string Directory = F.Name + ".txt";
        TestNotes_Helper* TN = new TestNotes_Helper();
        F.TestCount += 1;
        if (Success) { F.SuccessCount += 1; TN->MakeNotes("Success", Directory); }
        else { F.FailureCount += 1; TN->MakeNotes("Failure", Directory); }
        delete TN;
    }
};