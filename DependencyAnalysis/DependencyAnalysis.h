#ifndef  DEPEN_ANAL
#define  DEPEN_ANAL
#pragma once
//////////////////////////////////////////////////////////////////////
// DependencyAnalysis.h - This package used to do depedency analysis//
//                                                                  //
// Zihao Xing, CSE687 - Object Oriented Design, Spring 2017         //
//////////////////////////////////////////////////////////////////////
/*
Package Operations:
==================
This package used to do dependency analysis. The idea to do dependency
analysis is to iterator the typetable, for each type(entry) in type table,
find the type name, namespace, package name show up in other files or not.
if it is, then there is a dependency relation between these two files.
The dependency relationships will be kept in a XML by this class by using
the utilities provided by NoSQL DB.

Public Interface:
=================
class DepAnal
- DepAnal(TypeTableTEST::TypeTable tb);
To do Dependency Analysis, this class will use a typetable's instance to initialize
- void doDepAnal() ;
This function will use several private functions to do dependency analysis


Build Process:
==============
Note: if this package build has already been build as static library,
then change to build as aplication to run

Required files
- DependencyAnalysis.h, DependencyAnalysis.cpp, DependencyTable.h
NoSqlDb.h

Maintenance History:
====================
ver 1.0 : 07 Mar 2017
- first release
*/

#include "../Parser/ActionsAndRules.h"
#include "../FileSystem/FileSystem.h"
#include "../TypeTable/TypeTable.h"
#include "../NoSqlDb/NoSqlDb.h"
#include  "DepencyTable.h"
#include "../NoSqlDb/Persist.h"
#include <iostream>
#include <functional>
#include <algorithm>

namespace DependenceAnalysis
{
	class DepAnal
	{
	public:
		DepAnal(TypeTableTEST::TypeTable tb) :typetable_(tb), ASTref_(CodeAnalysis::Repository::getInstance()->AST()) {}

		DependcyTable getDepTable()
		{
			return deptable_;
		}

		// -- do dependency analysis
		void doDepAnal()
		{
			getFiles(ASTref_.root());
			std::cout << "\n\nStart doing dependency analysis..............\n";
			for (auto iter = typetable_.begin(); iter != typetable_.end(); iter++)
			{
				//1, for each file get toker;
				//2, find match;
				//3, add file to the dep table;
				std::cout << "Analyzing Type "<<iter->typeName <<"......\n";
				TableElem temp;
				for (auto file : fileLists_)
				{
					if (!AlreadyExisted(file, iter->path)) {
						if (FindTokensinFile(iter->typeName, file)
							//&&FindTokensinFile(iter->file, file) && 
							//FindTokensinFile(iter->nameSpace, file)
							) {
							if (iter->path.size() == 1)
							{
								temp.files = iter->path.at(0);
								temp.addFile(file);
							}
						}
					}
				}
				deptable_.addRecord(iter->typeName, temp);
			}
			deptable_.show();
			std::cout << "\n\n";
			deptable_.Extract();
			deptable_.showTinyTable();
			persistDepAnal();
		}
		// --- doing dependency analysis by using include relationship
		void depAnalsis() 
		{
			getFiles(ASTref_.root());
			for (auto file : fileLists_) 
			{
				// find include file
			}
		}
	private:
		TypeTableTEST::TypeTable typetable_;
		DependcyTable deptable_;
		CodeAnalysis::AbstrSynTree& ASTref_;
		std::vector<std::string> fileLists_;

	private:
		// --- get all files
		void getFiles(CodeAnalysis::ASTNode* pNode)
		{
			static std::string path = "";
			if (pNode->path_ != path)
			{
				std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;
				path = pNode->path_;
				fileLists_.push_back(path);
			}
			for (auto pChild : pNode->children_)
				getFiles(pChild);
		}

		// -- find a token existed in a file or not
		bool FindTokensinFile(std::string keyword, std::string fileSpec)
		{
			try
			{
				std::ifstream in(fileSpec);
				if (!in.good())
				{
					std::cout << "\n  can't open " << fileSpec << "\n\n";
					return false;
				}
				{
					Scanner::Toker toker;
					toker.returnComments();
					toker.attach(&in);
					do
					{
						std::string tok = toker.getTok();
						if (  + ".h" == keyword || tok + ".cpp" == keyword) return true;
						if (tok == keyword) return true;
					} while (in.good());
				}
			}
			catch (std::logic_error& ex)
			{
				std::cout << "\n  " << ex.what();
			}
			return false;
		}

		// -- find tokens existed in file or not
		bool FindTokensinFile(std::vector<std::string> files, std::string fileSpec)
		{
			for (auto item : files)
			{
				if (FindTokensinFile(item, fileSpec)) return true;
			}
			return false;
		}

		//-- used to find a path exist in the vector of record's pathes
		bool AlreadyExisted(std::string path, std::vector<std::string> paths)
		{
			if (std::find(paths.begin(), paths.end(), path) != paths.end()) { return true; }
			return false;
		}

		// -- persist the type analysis results to XML
		void persistDepAnal()
		{
			NoSqlDb<std::string> dbInstance;
			for (auto item : deptable_.getTinyDepTable())
			{
				Element<std::string> Elem;
				Elem.name = deptable_.getFileName(item.files);
				for (auto it : item.relatedFiles)
				{
					Elem.addChild(it);
				}
				Elem.categoty = item.files;
				Elem.timeDate = "now";
				Elem.data = "default";
				dbInstance.addRecord(Elem.name, Elem);
			}
			std::string xml = PersistFunc::DBtoXML_V1<std::string>(dbInstance);
			Persist persister;
			persister.save(xml, "dependencyRelation.xml");
			std::cout << "Already save the dependency relationships to " << FileSystem::Path::getFullFileSpec("dependencyRelation.xml") << std::endl;
		}
	};
}

#endif // ! DEPEN_ANAL
