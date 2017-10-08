/////////////////////////////////////////////////////////////////////////
//TableTable.cpp - This package used to test utilities of TypeTable    //
//                                                                     //
// zihao xing, CSE687 - Object Oriented Design, Spring 2017            //
/////////////////////////////////////////////////////////////////////////

#include "./TypeTable.h"
#include <iostream>

using namespace TypeTableTEST;

// - add file and path to a table record
void TableRecord::addFileAndPath(std::string f,std::string p) 
{
	file.push_back(f);
	path.push_back(p);
}

// -- define comparasion operation
bool TableRecord::operator==(const TableRecord& rhs) 
{
	if ((*this).typeName == rhs.typeName) {
		if ((*this).type == rhs.type) {
			if ((*this).file == rhs.file) { return true; }
		}		
	}
	return false;
}

// -show the details of a record
std::string TableRecord::show()
{
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\n    " << std::setw(8) << "TypeName" << " : " << typeName;
	out << "\n    " << std::setw(8) << "Type" << " : " << type;
	out << "\n    " << std::setw(8) << "NameSpace" << " : " << nameSpace;
	File children_ = static_cast<File>(file);
	if (children_.size() > 0)
	{
		out << "\n    " << std::setw(8) << "File" << " : ";
		for (size_t i = 0; i < children_.size(); ++i)
		{
			out << children_[i];
			if (i < children_.size())
				out << ", ";
		}
	}
	Path path_ = static_cast<Path>(path);
	if (path_.size() > 0)
	{
		out << "\n    " << std::setw(8) << "Path" << " : ";
		for (size_t i = 0; i < path_.size(); ++i)
		{
			out << path_[i];
			if (i < path_.size())
				out << ", ";
		}
	}
	return out.str();
}


// -- add a record to the typetable
void TypeTable::addRecord(const TableRecord& record)
{
	records_.push_back(record);
}
// --remove a record from typetable
void TypeTable::removeRecord(const std::string& name)
{
	for (size_t i=0;i<records_.size();i++) 
	{
		if (records_[i].typeName == name&&records_[i].type != "class")
		{
			records_.erase(std::remove(records_.begin(),records_.end(),records_[i]),records_.end());
		}
	}
}

// -----<overloading [] operator to get n-th record>--------
TableRecord& TypeTable::operator[](size_t n) 
{
	if (n < 0 || n >= records_.size()) 
	{
		throw(std::exception("index out of range"));
	}
	return records_[n];
}

// -----<overloading [] operator to get n-th record for const type>--------
TableRecord TypeTable::operator[](size_t n) const
{
	if (n < 0 || n >= records_.size())
	{
		throw(std::exception("index out of range"));
	}
	return records_[n];
}

// -------<display the type table>---------
void TypeTable::show() 
{
	std::cout << "\nTypeTable -- Req4# doing type analysis\n";
	std::cout << "====================================================================================================\n";
	std::cout << std::left;
	std::cout << std::setw(20) << "typename" << std::setw(20) << "type" << std::setw(20) << "namespace" << std::setw(30)
		<< "file" << std::setw(30) << "path" << std::setw(30) << std::endl;
	for (auto item : *this)
	{
		std::cout << std::setw(20) << item.typeName;
		std::cout << std::setw(20) << item.type;
		std::cout << std::setw(20) << item.nameSpace;
		std::cout << std::setw(30);
		for (size_t i = 0; i < item.file.size();i++) 
		{
			std::cout << item.file[i] << " ";
		}
		std::cout << std::setw(30);
		for (size_t i = 0; i < item.path.size(); i++)
		{
			std::cout << item.path[i] << ",";
		}
		std::cout << "\n";
	}
}

#ifdef TEST_TYPETABLE
int main() 
{
	TypeTable table;

	TableRecord tr1;
	tr1.typeName = "TypeTable";
	tr1.type = "class";
	tr1.nameSpace = "TypeTable";
	tr1.file = "Type1.h";
	tr1.path = "../";
	table.addRecord(tr1);

	TableRecord tr2;
	tr2.typeName = "Type";
	tr2.type = "class";
	tr2.nameSpace = "TTable";
	tr2.file = "../Type1.h";
	tr1.path = "../";
	table.addRecord(tr2);
	
	std::vector<std::string> keys = table.keys();
	for (std::string key : keys) 
	{
		std::cout << "\n " << key << ":";
		std::cout << table.getType(key).show();
	}
	std::cout << "\n";
}
#endif // 