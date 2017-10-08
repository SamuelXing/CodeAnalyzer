#pragma once
///////////////////////////////////////////////////////////////
// DependencyTable.h - declare dependency table class        //
//                                                           //
// Zihao Xing, CSE687 - Object Oriented Design, Spring 2017  //
///////////////////////////////////////////////////////////////
/*

Public Interface:
=================
class TableElem:
- void addFile(std::string file): add a file to the related files

class DependcyTable:
- bool addRecord(std::string type, TableElem elem): add a record to the dependecy table
- std::vector<std::string> getdepfiles(): get files show up in the dependency table,
									      used to build graph
- void Extract(): used to extract fine data from raw dependcency table
- void showTinyTable(): show a tiny&refined dependency table
- void show(): display a dependency table
- std::string getFileName(const std::string& s): get filename from path


Build Process:
==============
Required files
- DependencyTable.h


Maintenance History:
====================
ver 1.0 : 08 Mar 2017
- first release
*/
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace DependenceAnalysis 
{
	class  TableElem
	{
	public:
		std::string files;
		std::vector<std::string> relatedFiles;
		TableElem() {}
		//--------<add a file to the related file>------
		void addFile(std::string file) 
		{
			if (std::find(relatedFiles.begin(), relatedFiles.end(), file) != relatedFiles.end())
				return;
			relatedFiles.push_back(file);
		}
	private:
	};

	class DependcyTable
	{
	public:
		DependcyTable() {}
		// -- get a tiny type table
		std::vector<TableElem> getTinyDepTable() { return tinyTable_; }
		// -- add a rocord in dependency table
		bool addRecord(std::string type, TableElem elem) 
		{
			if (depTable_.find(type) != depTable_.end())
				return false;
			depTable_[type] = elem;
			return true;
		}
		// -- return a file list that existed in dependency table, used for build graph
		std::vector<std::string> getdepfiles() 
		{
			std::vector<std::string> vertexs;
			std::vector<TableElem> itemList = this->getTinyDepTable();
			for (auto item : itemList) 
			{
				if (std::find(vertexs.begin(), vertexs.end(), item.files) == vertexs.end()) { vertexs.push_back(item.files); }
				for (size_t i = 0; i < item.relatedFiles.size(); i++)
				{
					if (std::find(vertexs.begin(), vertexs.end(), item.relatedFiles[i]) == vertexs.end())
						vertexs.push_back(item.relatedFiles[i]);
				}
			}
			return vertexs;
		}

		// -- used to extract fine data from raw dependcency table
		void Extract() 
		{
			for (auto item : depTable_) 
			{
				if(!ExistItem(item.second)&&item.second.files!="")
				{
					tinyTable_.push_back(item.second);
				}				
			}
		}

		// -- show a tiny&refined dependency table 
		void showTinyTable() 
		{
			std::cout << "\n";
			std::cout << "\nTiny Dependency Table\n";
			std::cout << "====================================================================================================\n";
			std::cout << std::left;
			std::cout <<std::setw(30) << "file" << std::setw(30)
				<< "related file(denpendency)" << std::setw(30) << std::endl;
			for (auto item : tinyTable_)
			{
				std::cout << std::setw(30) << getFileName(item.files);
				for (size_t i = 0; i < item.relatedFiles.size(); i++)
				{
					std::cout << getFileName(item.relatedFiles[i]) << ",";
				}
				std::cout << "\n";
			}
		}

		// -- display a dependency table
		void show() 
		{
			std::cout << "\n";
			std::cout << "\nDependency Table--Req #5: doing dependency analysis\n";
			std::cout << "====================================================================================================\n";
			std::cout << std::left;
			std::cout<< std::setw(30) << "typename" << std::setw(30) << "file" << std::setw(30)
				<< "related file(denpendency)" << std::setw(30) << std::endl;
			for (auto item: depTable_)
			{
				TableElem temp = item.second;
				if (temp.relatedFiles.size() != 0)
				{
					std::cout << std::setw(30) << item.first;
					std::cout << std::setw(30) << getFileName(temp.files);
					for (size_t i = 0; i < temp.relatedFiles.size(); i++)
					{
						std::cout << getFileName(temp.relatedFiles[i]) << ",";
					}
					std::cout << "\n";
				}		
			}		
		}

		// -- get filename from path
		std::string getFileName(const std::string& s)
		{
			char sep = '/';
#ifdef _WIN32
			sep = '\\';
#endif // _WIN32
			size_t i = s.rfind(sep, s.length());
			if (i != std::string::npos) { return(s.substr(i + 1, s.length() - i)); }
			return("");
		}

	private:
		std::unordered_map<std::string, TableElem> depTable_;
		std::vector<TableElem> tinyTable_;

		// find a depndeny existed in tinyTable_ or not
		bool ExistItem(TableElem tm) 
		{
			for (auto iter = tinyTable_.begin(); iter != tinyTable_.end(); iter++) 
			{
				if (tm.files == iter->files) { return true; }
			}
			return false;
		}
	};

}