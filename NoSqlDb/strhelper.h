#pragma once
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <string>

static inline std::string &ltrim(std::string& s) 
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), 
		std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

static inline std::string &rtrim(std::string& s) 
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base());
	return s;
}

static inline std::string& trim(std::string& s) 
{
	try 
	{
		s=ltrim(rtrim(s));
	}
	catch (std::exception& ex) 
	{
		std::cout << "ERROR: " << ex.what() << "\n";
	}
	return s;
}