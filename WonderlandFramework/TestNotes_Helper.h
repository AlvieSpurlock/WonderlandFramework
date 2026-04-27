#pragma once

#include "Test_Structs.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>

class TestNotes_Helper
{
public:
	TestNotes_Helper() {} //Obligitory Default Constructor

	std::string MakeDirectory(std::string FeatureName)
	{
		std::string FeatureDirectory = FeatureName + ".txt";
		return FeatureDirectory;
	}

	void MakeNotes(std::string Condition, std::string Directory)
	{
		std::cout << "Why is this a " << Condition << "?\n";
		std::string Note;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::getline(std::cin, Note);
		SaveNote(Note, Directory);
	}

	void SaveNote(std::string Note, std::string Directory)
	{
		//-----[Init Note File]-----\\

		std::ofstream NoteFile(Directory, std::ios::app);
		if (!NoteFile.is_open())
		{
			std::cout << Directory << " Failed to Open!\n";
		}

		//-----[Add Time Header]-----\\

		std::time_t Now = std::time(nullptr);
		tm TimeInfo;
		localtime_s(&TimeInfo, &Now);
		NoteFile << std::put_time(&TimeInfo, "%Y-%m-%d %H:%M:%S") << "\n\n";

		//-----[Wrap Text]-----\\

		size_t Width = 26;
		std::vector<std::string> Lines;
		size_t Start = 0;
		while (Start < Note.size())
		{
			size_t End = Start + Width;
			if (End >= Note.size())
			{
				Lines.push_back(Note.substr(Start));
				break;
			}
			size_t Wrap = End;
			while (Wrap > Start && Note[Wrap] != ' ')
			{
				--Wrap;
			}
			if (Wrap == Start)
			{
				Wrap = End;
			}
			Lines.push_back(Note.substr(Start, Wrap - Start));
			Start = Wrap + 1;
			while (Start < Note.size() && Note[Start] == ' ')
			{
				++Start;
			}
		}

		//-----[Save Wrapped Text]-----\\

		for (auto& Line : Lines)
		{ NoteFile << Line << "\n"; }
		//-----[Add Indent]-----\\

		NoteFile << "\n\n";
		NoteFile.close(); // Obligatory FIle Close
	}

	bool IsTimestamp(const std::string& Line)
	{
		if (Line.size() != 19) return false;
		return
			Line[4] == '-' &&
			Line[7] == '-' &&
			Line[10] == ' ' &&
			Line[13] == ':' &&
			Line[16] == ':';
	}

	std::vector<NoteEntry> LoadNotes(const std::string& Directory)
	{
		std::ifstream NoteFile(Directory);
		if (!NoteFile.is_open())
		{
			std::cout << Directory << " Failed to Open!\n";
			return {};
		}
		std::vector<NoteEntry> Notes;
		std::string Line;
		NoteEntry Current;
		while (std::getline(NoteFile, Line))
		{
			if (IsTimestamp(Line))
			{
				if (!Current.Timestamp.empty())
				{
					Notes.push_back(Current);
				}
				Current = NoteEntry{};
				Current.Timestamp = Line;
			}
			else
			{
				if (!Line.empty())
				{
					Current.Text += Line + "\n";
				}
			}
		}
		if (!Current.Timestamp.empty())
		{
			Notes.push_back(Current);
		}
		return Notes;
	}
};