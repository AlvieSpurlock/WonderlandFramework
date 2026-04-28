#pragma once

#include <string>
#include <vector>
#include <algorithm>


//-----[Note Entry Struct]-----\\

struct NoteEntry
{
	std::string Timestamp;
	std::string Text;
};



//-----[Feature Struct]-----\\

struct Feature
{
	std::string Name;
	int TestCount = 0;
	int SuccessCount = 0;
	int FailureCount = 0;
	std::string Directory;
	Feature() {} //Obligitory Default Constructor


	//-----[Actual Feature Constructor]-----\\

	Feature(std::string N, int TC, int SC, int FC)
	{
		Name = N;
		TestCount = TC;
		SuccessCount = SC;
		FailureCount = FC;
		std::string NoSpaces = N;
		NoSpaces.erase(std::remove(NoSpaces.begin(), NoSpaces.end(), ' '), NoSpaces.end());
		Directory = NoSpaces + ".txt"; 
	}
};