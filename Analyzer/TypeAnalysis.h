#pragma once
///////////////////////////////////////////////////////////////
// TypeAnalysis.h - This package used to do type analysis    //
//                                                           //
// Zihao Xing, CSE687 - Object Oriented Design, Spring 2017  //
///////////////////////////////////////////////////////////////
/*
Package Operations:
==================
This package used to do type analysis. The basic idea to fullfill
such goal is to use the information provided by Abstract Syntax Tree;
The TypeAnal class support several ways to explore the ATS;

Public Interface:
=================
class TypeAnal
- void findClass(); // use to find class and struct definition
- void findGlobalFunction(ASTNode* pNode); // use to find global function information
- void findTypeDecl(ASTNode* pNode); // use to find gobal data definition 
										and also typedefs, and aliases' definitons
- void doTypeAnal(); // call these three function above to implement requirement #2
- TypeTable getTypeTable(); // get the typetabl


Build Process:
==============
Required files
- TypeAnalysis.h TypeAnalysis.cpp, TypeTable.h


Maintenance History:
====================
ver 1.0 : 05 Mar 2017
- first release
*/

#include "../Parser/ActionsAndRules.h"
#include "../TypeTable/TypeTable.h"
#include <iostream>
#include <functional>

#pragma warning (disable : 4101)  // disable warning re unused variable x, below

namespace CodeAnalysis
{
  class TypeAnal
  {
  public:
    using SPtr = std::shared_ptr<ASTNode*>;

    TypeAnal();
	void findClass();
	void findGlobalFunction(ASTNode* pNode);
	void findTypeDecl(ASTNode* pNode);
    void doTypeAnal();
	TypeTableTEST::TypeTable getTypeTable();
  private:
    void DFS(ASTNode* pNode);
    AbstrSynTree& ASTref_;
    ScopeStack<ASTNode*> scopeStack_;
    Scanner::Toker& toker_;
	TypeTableTEST::TypeTable typetable_;
	std::vector<std::string> funcUnderclass_;
	void FindClassfunc();
	void ruleOutClassFuncInCPP();
	bool FindKey(std::string key);
  };

  // --return type table
  inline TypeTableTEST::TypeTable TypeAnal::getTypeTable() 
  {
	  return typetable_;
  }

  // --initialize
  inline TypeAnal::TypeAnal() :
    ASTref_(Repository::getInstance()->AST()),
    scopeStack_(Repository::getInstance()->scopeStack()),
    toker_(*(Repository::getInstance()->Toker()))
  {
   
  }                                              

  // -- just for display 
  inline bool doDisplay(ASTNode* pNode)
  {
    static std::string toDisplay[] = {
      "function", "lambda", "class", "struct", "enum", "alias", "typedef"
    };
    for (std::string type : toDisplay)
    {
      if (pNode->type_ == type)
        return true;
    }
    return false;
  }

  // -- DFS to show
  inline void TypeAnal::DFS(ASTNode* pNode)
  {
    static std::string path = "";
    if (pNode->path_ != path)
    {
      std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;
      path = pNode->path_;
    }
    if (doDisplay(pNode))
    {
      std::cout << "\n  " << pNode->name_;
      std::cout << ", " << pNode->type_;
	  std::cout << ", " << pNode->package_;
	  std::cout << ", " << pNode->parentType_;
    }
    for (auto pChild : pNode->children_)
      DFS(pChild);
  }

  // -- do type analysis
  inline void TypeAnal::doTypeAnal()
  {
    std::cout << "\n  starting type analysis: --- The current test stub are located in \"../TestFiles\"\n";
    std::cout << "\n  scanning AST and displaying important things:";
    std::cout << "\n -----------------------------------------------\n ";
    ASTNode* pRoot = ASTref_.root();
	DFS(pRoot);
	std::cout << "\n\n  doing type analysis: --- The current test stub are located in \"../TestFiles\"\n";
	findClass();
	findGlobalFunction(pRoot);
	//ruleOutClassFuncInCPP();
	findTypeDecl(pRoot);
	typetable_.show();
	std::cout << "\n";
  }

  // used to find class definition
  inline void TypeAnal::findClass()
  {
	  std::unordered_map<std::string, ASTNode*> typemap = ASTref_.typeMap();
	  TypeTableTEST::TypeTable typetable;
	  for (auto item : typemap)            // add class, struct
	  {
		  TypeTableTEST::TableRecord tr;
		  tr.typeName = item.first;
		  tr.type = item.second->type_;
		  tr.addFileAndPath(item.second->package_, item.second->path_);
		  tr.nameSpace = item.second->parentTypeName_;
		  typetable_.addRecord(tr);
		  std::cout << "Find class/struct definition: " << tr.typeName << std::endl;
	  }
  }

  // use to find global function
  inline void TypeAnal::findGlobalFunction(ASTNode* pNode)
  {
	  static std::string path = "";
	  if (ASTref_.root()->path_ != path)
	  {
		  std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;
		  path = pNode->path_;
	  }
	  if (pNode->parentType_ == "namespace" && pNode->type_ == "function" && pNode->name_!="main")
	  {
		  TypeTableTEST::TableRecord tr;
		  tr.typeName = pNode->name_;
		  tr.type = pNode->type_;
		  tr.nameSpace = pNode->parentTypeName_;
		  tr.addFileAndPath(pNode->package_, pNode->path_);
		  typetable_.addRecord(tr);
		  std::cout << "Find gloabl function definition: " << tr.typeName << std::endl;
	  }
	  for (auto pChild : pNode->children_)
		  findGlobalFunction(pChild);
  }

  // this function use to find type declaration as well as public data, like using, typedef, aliase
  inline void TypeAnal::findTypeDecl(ASTNode* pNode)
  {
	  static std::string path = "";
	  if (ASTref_.root()->path_ != path)
	  {
		  std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;
		  path = pNode->path_;
	  }
	  if (pNode->type_ == "namespace")
	  {
		  for (auto node : pNode->decl_)
		  {
			  TypeTableTEST::TableRecord tr;
			  if (node.type_ == "using") { tr.type = "using"; std::cout << "Find ailas definition: " << node.typename_ << std::endl;}
			  if (node.type_ == "typedef") { tr.type = "typedef"; std::cout << "Find typedef definition: " << node.typename_ << std::endl;}
			  if (node.type_ == "data") { tr.type = "data"; std::cout << "Find global data definition: " << node.typename_ << std::endl;}
			  if (node.type_ == "enum") { tr.type = "enum"; std::cout << "Find enum definition: " << node.typename_ << std::endl;}
			  tr.typeName = node.typename_;
			  tr.nameSpace = pNode->name_;
			  tr.addFileAndPath(node.package_, node.path_);
			  if(node.typename_!="") typetable_.addRecord(tr); // rule out declaration of globle node
		  }
	  }
	  for (auto pChild : pNode->children_)
		  findTypeDecl(pChild);
  }


  // use to find the functions under class
  inline void TypeAnal::FindClassfunc()
  {
	  std::unordered_map<std::string, ASTNode*> typemap = ASTref_.typeMap();
	  for (auto item : typemap)            // add class, struct
	  {
		  for (size_t i = 0; i < item.second->children_.size(); i++) 
		  {
			  if(item.second->children_[i]->type_ == "function")
				funcUnderclass_.push_back(item.second->children_[i]->name_);
		  }
	  }
  }

  // this function used to rule out class function definition in CPP file
  inline void TypeAnal::ruleOutClassFuncInCPP() 
  {
	  FindClassfunc();
	  if (funcUnderclass_.size() > 0) 
	  {
		  for (size_t i = 0; i < funcUnderclass_.size(); i++) 
		  {
			  if (FindKey(funcUnderclass_[i])) 
			  {
				  typetable_.removeRecord(funcUnderclass_[i]);
			  }
		  }
	  }
  }

  // use to find a key in the typetable
  inline bool TypeAnal::FindKey(std::string name) 
  {
	  for (auto item : typetable_) 
	  {
		  if (name == item.typeName) { return true; }
	  }
	  return false;
  }
}
