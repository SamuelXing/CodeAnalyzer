///////////////////////////////////////////////////////////////////////
//Persist.h - persist the DB string from/to XML file                 //
// Author: zihao                                                     //
///////////////////////////////////////////////////////////////////////

// public interfaces:
//  int save(const std::string& xml, const std::string& filename) - save the DB to XML
//	std::string retrieve(const std::string& filename)  - retrieve the DB from XML

// Maintanence History: 2017/2/7 - first release.

#pragma once

#include <string>
#include <fstream>
#include <iostream>


class  Persist
{
public:
	int save(const std::string& xml, const std::string& filename) 
	{
		std::cout << "\nReq #3: using my own NoSqlDB to Persist \n";
		std::ofstream outxml;
		try
		{
			//outxml.open(filename, std::ios::app); 
			outxml.open(filename);
			outxml << xml;
			outxml.close();
		}
		catch (const std::exception& ex)
		{
			std::cout << "Error: " << ex.what() << std::endl;
		}
		std::cout << "\nPersisting the following text to XML: ---Req #7: write the analysis results, in XML format, to a specified file\n" << xml<<"\n\n";
		return 0;
	}
	std::string retrieve(const std::string& filename) 
	{
		std::ifstream inxml{filename};
		std::string xml;
		try 
		{
			xml.assign((std::istreambuf_iterator<char>(inxml)),(std::istreambuf_iterator<char>() ));
			inxml.close();
		}
		catch (const std::exception& ex) 
		{	
			std::cout << "Error: " << ex.what() << std::endl;
		}
		return xml;
	}	
private:

};


