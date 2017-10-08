#pragma once
#ifndef TEST_TYPETABLE

/////////////////////////////////////////////////////////////////////////
// TableTable.h - This package provides the definition of typetable    //
//                                                                     //
// zihao xing, CSE687 - Object Oriented Design, Spring 2017            //
/////////////////////////////////////////////////////////////////////////
/*
Package Operations:
==================
This package defines the TableRecord class and TypeTable class to describe 
the type table structure

Public Interface:
=================
TypeTable:
void addRecord(const TableRecord& record); // add a table record
void removeRecord(const std::string& name); // remove a table record
TableRecord:
void addFileAndPath(std::string file, std::string path); /// add a file-and-path pair
std::string show(); // display a table record

Build Process:
==============
Required files
- TypeTable.h TypeTable.cpp


Maintenance History:
====================
ver 1.0 : 05 Mar 2017
- first release
*/

#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace TypeTableTEST 
{
	class TableRecord
	{
	public:
		using TypeName = std::string;
		using Type = std::string;
		using NameSpace = std::string;
		using File = std::vector<std::string>;
		using Path = std::vector<std::string>;
		
		TableRecord() {}
		TypeName typeName;
		Type type;
		NameSpace nameSpace;
		File file;
		Path path;
		bool operator == (const TableRecord& rhs);
		void addFileAndPath(std::string file, std::string path);
		std::string show();
	private:

	};


	class TypeTable
	{
	public:
		using iterator = typename std::vector<TableRecord>::iterator;

		TypeTable() {}
		TableRecord& operator[](size_t n);
		TableRecord operator[](size_t n) const;
		iterator begin() { return records_.begin(); }
		iterator end() { return records_.end(); }
		void addRecord(const TableRecord& record);
		void removeRecord(const std::string& name);
		void show();
	private:
		std::vector<TableRecord> records_;
	};
}
#endif // !TEST_TYPETABLE

