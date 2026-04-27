#pragma once
#include "Test_Structs.h"
#include "Test_UI_Helper.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
class TestLog_Helper
{
public:
	std::vector<Feature> Features; //Vector of Features
	Test_UI_Helper* UI = nullptr;
	
	//-----[Obligatory]-----\\
	//Constructor & Deconstructor
	
	TestLog_Helper() { UI = new Test_UI_Helper(); } 
	~TestLog_Helper() { delete UI; }
	

	//-----[Save Test Log Function]-----\\
    //Saves the TestLog.csv for Persistance

	void SaveTestLog()
	{
		std::ofstream TestLog("TestLog.csv", std::ios::app);
		Feature& F = Features.back();
		TestLog << F.Name << "," << F.TestCount << "," << F.SuccessCount << "," << F.FailureCount << "\n";
	}
	

	//-----[Feature Creation]-----\\
	//Used for Loading Features and related data as 
	//well initializing the TestLog.csv file

	void MakeFeature(std::string N = "", int TC = 0, int SC = 0, int FC = 0, bool IsNew = false)
	{
		std::string Name = N;
		int TestCount = TC;
		int SuccessCount = SC;
		int FailureCount = FC;
		if (Name.empty())
		{
			std::cout << "Feature Name?\n";
			std::getline(std::cin >> std::ws, Name);
		}
		Feature newFeature(Name, TestCount, SuccessCount, FailureCount);
		Features.push_back(newFeature);
		if (IsNew)
		{
			SaveTestLog();
			std::string NotesPath = newFeature.Directory.substr(0, newFeature.Directory.size() - 4) + ".txt";
			std::ofstream NotesFile(NotesPath);
			NotesFile.close();
		}
	}


	//-----[Load File]-----\\
	
	void LoadTestLog()
	{
		std::ifstream TestLog("TestLog.csv");
		if (!TestLog.is_open())
		{
			std::cout << "No Test Log Exists!\nDo you wish to make a new Feature to Test?\n Y = Yes, N = No\n";
			char Answer;
			std::cin >> Answer;
			Answer = (char)std::toupper(static_cast<unsigned char>(Answer));
			if (Answer == 'Y')
			{
				MakeFeature("", 0, 0, 0, true);
			}
			else
			{
				if (Answer == 'N')
				{
					std::cout << "Goodbye!";
					abort();
				}
				else
				{
					std::cout << "Incorrect Input\n";
					LoadTestLog();
				}
			}
		}
		else
		{
			std::string Feature;
			while (std::getline(TestLog, Feature))
			{
				std::stringstream Info(Feature);
				std::string Data;
				std::vector<std::string> FeatureInfo;
				while (std::getline(Info, Data, ','))
				{
					FeatureInfo.push_back(Data);
				}
				if (FeatureInfo.size() >= 4)
				{
					MakeFeature(FeatureInfo[0], stoi(FeatureInfo[1]), stoi(FeatureInfo[2]), stoi(FeatureInfo[3]), false);
				}
			}
		}
	}
};