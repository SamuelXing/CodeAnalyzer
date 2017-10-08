///////////////////////////////////////////////////////////////////////
//NoSqlDb.h - the sketch for a key/value database                    //
//                                                                   //
// Author: zihao                                                     //
///////////////////////////////////////////////////////////////////////
// Public interface:
//class strycture description:
//class Element<Data>
// std::string show(); - to show the element
// void addChild(Key key) - to add child to an element
// void removeChild(Key key)  - to remove a child in an element
// virtual Element<Data>& operator=(const Element<Data>& rs) - overloading = operator
// virtual Element<Data>& operator()(const Element<Data>& rs) - overloading () operator
// friend bool operator==(const Element<Data>& rs, const Element<Data>& ls)  -overloading == operator

//class NoSqlDb<Data>
// bool initialize(std::string filename = "DB.xml", bool Timer = false)  - initialize DB, two parameters by defualt.
// void thread_proc(bool Timer = false); - thread proc
// void close()  - close the Db
//Keys keys() const; - get all keys
//bool save(Key key, Element<Data> elem); - save current DB
//bool addRecord(Key key, Element<Data> elem); - add a record to the DB
//bool addRecord(Item item); - add a record to the DB
//bool removeRecord(const Key& key); - remove a record from the Db
//Element<Data> getRecord(const Key& key); - get the Record 
//const Element<Data> value(Key key) const; - get the value of a specific key
//std::vector<Element<Data>> findAll(); - get all Elements
//bool replaceValue(Key key, Element<Data> newValue); -replace the element with a new instance
//bool addRelationship(Key key, Key child); - add a child relationship
//bool deleteRelationship(Key key, Key child); - delete a relationship
//bool editCategory(Key Key, std::string cate); - edit text field
//size_t count(); - get the size of current DB
//virtual NoSqlDb<Data>& operator=(const NoSqlDb<Data>& rs) -overloading = operator
//virtual NoSqlDb<Data>& operator()(const NoSqlDb<Data>& rs) -overloading () operator


// Namespace PersisFunc
// NoSqlDb<Data>& DBfromXML_V1(NoSqlDb<Data>& db, const std::string& xml)  - translate xml string to an instance of DB
// std::string DBtoXML_V1(const NoSqlDb<int>& db) - translate db to xml string- for in type data
// std::string DBtoXML_V1(const NoSqlDb<Data>& db) - translate db to XMl string

///////////////////////////////////////////////////
// Maintanence History: 2017/2/7 - first release.

#ifndef DOSQLDB_TEST
#define DOSQLDB_TEST

#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include "CppProperties.h"
#include "../xmldocument/xmldocument/xmldocument.h"
#include "../xmldocument/xmlelement/xmlelement.h"
#include "convert.h"
#include "strhelper.h"
#include "Persist.h"
#include <thread>
#include <chrono>
#include <mutex>
/*
* Element class represents a data record in our NoSql Database
* - in our NoSql database that is just the value ina key/value pair
* - it needs to store child data - not implement yet
*/

template<typename Data>
class  Element
{
public:
	using Name = std::string;
	using Category = std::string;
	using TimeDate = std::string;
	using Key = std::string;

	Property<Name> name;			//metadata
	Property<Category> categoty;	//metadata
	Property<TimeDate> timeDate;	//metadata
	std::vector<Key> children; //metadata
	Property<Data> data;			//data
	std::string show();

	// -----<add a chile for an element>-------
	void addChild(Key key) 
	{
		children.push_back(key);
		return;
	}

	// ---------<remove an child of an Element>------
	void removeChild(Key key) 
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			if (!trim(children.at(i)).compare(trim(key))) 
			{
				children.erase(children.begin() + i);
			}
		}
		return;
	}

	// ----<orverloading assignment operator>---------
	virtual Element<Data>& operator=(const Element<Data>& rs)
	{
		name = rs.name;
		categoty = rs.categoty;
		timeDate = rs.timeDate;
		data = rs.data;
		for (auto child:rs.children) 
		{
			children.push_back(child);
		}
		return *this;
	}

	// ------<ovreloading () operator>--------
	virtual Element<Data>& operator()(const Element<Data>& rs)
	{
		name = rs.name;
		categoty = rs.categoty;
		timeDate = rs.timeDate;
		data = rs.data;
		for (auto child : rs.children)
		{
			children.push_back(child);
		}
		return *this;
	}

	// ----- <overloading comparasion operator>--------
	friend bool operator==(const Element<Data>& rs, const Element<Data>& ls)
	{
		if (rs.name == ls.name && 
			rs.categoty == ls.categoty &&
			rs.timeDate == ls.timeDate &&
			rs.data == ls.data) {
			for (auto child1 : rs.children)
			{
				for (auto child2 : ls.children)
				{
					if (child1 == child2)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	// ----------<overloading comparasion operator !=>--------
	friend bool operator!=(const Element<Data>& rs, const Element<Data>& ls) 
	{
		return !(rs == ls);
	}
};



// --------<this function used to display the DB>--------
template<typename Data>
std::string Element<Data>::show()
{
	//show children when implemented

	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\n	" << std::setw(8) << "name" << ":" << name;
	out << "\n	" << std::setw(8) << "category" << ":" << categoty;
	out << "\n	" << std::setw(8) << "timeDate" << ":" << timeDate;
	/*for (auto child : children)
	{
		out << "\n  " << std::setw(8) << "child" << ":" << child;
	}*/
	for (size_t i = 0; i < children.size(); i++) 
	{
		out << "\n  " << std::setw(0) << "      child" << ":" << children[i];
	}
	out << "\n	" << std::setw(8) << "data" << ":" << Convert<Data>::toString(data);
	out << "\n";
	return out.str();
}
/////////////////////////////////////////////////////////
//NoSqlDb class is a key/value pair in-memory database
// - store and retrieves elements
// 
template<typename Data>
class NoSqlDb
{
public:

	using Key = std::string;
	using Keys = std::vector<Key>;
	using Item = std::pair<Key, Element<Data>>;

	void thread_proc(bool Timer = false);
	
	NoSqlDb(){}

	// initialize by using thread_proc	
	bool initialize(std::string filename = "DB.xml", bool Timer = false) 
	{
		thread_t=std::thread(&NoSqlDb<Data>::thread_proc, this, Timer);
		init = true;
		std::string DbXml = persister.retrieve(filename);
		DbXml.erase(std::remove(DbXml.begin(),DbXml.end(),'\n'),DbXml.end());
		DbXml.erase(std::remove(DbXml.begin(), DbXml.end(), ' '), DbXml.end());
		try
		{
			mtx.lock();
			*this = PersistFunc::DBfromXML_V1<Data>(*this, DbXml);
			mtx.unlock();
		}
		catch (const std::exception& ex)
		{
			std::cout << ex.what() << "\n\n";
		}
		
		return init;
	}
	// ------<close the DB: join the thread>-------
	void close() 
	{
		if (init && thread_t.joinable())
			singnal = false;
		thread_t.join();
	}
	~NoSqlDb() 
	{	
	};
	Keys keys() const;
	bool save(Key key, Element<Data> elem);
	void save();
	bool addRecord(Key key, Element<Data> elem);
	bool addRecord(Item item);
	bool removeRecord(const Key& key);
	// this function is used to get the value of the key
	Element<Data> getRecord(Key key);
	// this function used to find the value according to the key
	// which means the it not allowed to edit the Element directly
	const Element<Data> value(Key key) const;
	std::vector<Element<Data>> findAll();
	bool replaceValue(Key key, Element<Data> newValue);
	bool addRelationship(Key key, Key child);
	bool deleteRelationship(Key key, Key child);
	bool editCategory(Key Key, std::string cate);
	size_t count();

	// ------<overloading cody assignment operator for DB>-------
	virtual NoSqlDb<Data>& operator=(const NoSqlDb<Data>& rs) 
	{
		Keys ks = rs.keys();
		for (Key k : ks)
		{
			Item item;
			item.first = k;
			item.second = rs.value(k);
			store.insert(item);
		}
		return *this;
	}

	// --------<overloading copy assignment operator> -------
	virtual NoSqlDb<Data>& operator()(const NoSqlDb<Data>& rs)
	{
		Keys ks = rs.keys();
		for (Key k : ks)
		{	
			Item item;
			item.first = k;
			item.second = rs.value(k);
			store.insert(item);
		}
		return *this;
	}
private:	
	Persist persister;
	std::thread thread_t;
	bool init = false;
	int counter=0;
	std::mutex mtx;
	std::unordered_map<Key, Element<Data>> store;
	bool singnal = true;
};

// ------<DB thread proc to implement automatically save the DB to XMl>-----
template<typename Data>
void NoSqlDb<Data>::thread_proc(bool Timer = false)
{
	while (true)
	{
		if (Timer == true)
		{
			mtx.lock();
			std::string xml = PersistFunc::DBtoXML_V1(*this);
			mtx.unlock();
			persister.save(xml, "DB.xml");
			// Persist DB to "DB.xml" every 3s
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		}
		if (counter == 2)
		{
			mtx.lock();
			counter = 0;		
			std::string xml = PersistFunc::DBtoXML_V1(*this);
			std::cout << "\n Persist to DB after 2 writes\n";
			persister.save(xml, "DB.xml");
			mtx.unlock();
		}
		if (singnal == false)
			break;
	}
}

// -----<add a child for an element by its key>-----
template<typename Data>
bool NoSqlDb<Data>::addRelationship(Key key, Key child) 
{
	Element<Data> record = getRecord(key);
	if (record != Element<Data>())
	{
		record.addChild(child);
		removeRecord(key);
		save(key,record);
		return true;
	}
	else
	{
		return false;
	}
}

// ---------<delete a child of an element by its key>-----
template<typename Data>
bool NoSqlDb<Data>::deleteRelationship(Key key, Key child)
{
	Element<Data> record = getRecord(key);
	if (record != Element<Data>())
	{
		record.removeChild(child);
		store.erase(key);
		save(key, record);
		return true;
	}
	else
	{
		return false;
	}
}

// ------<edit category filed of an element>--------
template<typename Data>
bool NoSqlDb<Data>::editCategory(Key key, std::string cate)
{
	Element<Data> record = getRecord(key);
	if (record != Element<Data>())
	{
		record.categoty = cate;
		removeRecord(key);
		save(key, record);
		return true;
	}
	else
	{
		return false;
	}
}

// -------<get all keys in the DB>------
template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys() const
{
	Keys keys;
	for (Item item : store)
	{
		keys.push_back(item.first);
	}
	return keys;
}

// --- <explicitly call this function to save an element by its key>----
template<typename Data>
bool NoSqlDb<Data>::save(Key key, Element<Data> elem)
{
	for (auto pos = begin(store); pos != end(store); ++pos)
	{
		if (pos->first == key)
		{
			std::cout << "\n find key! cant save\n";
			return true;
		}
	}
	auto item = std::make_pair(key,elem);
	mtx.lock();
	++counter;
	mtx.unlock();
	store.insert(item);
	return false;
}

// ---<save() funtion oveloading: just increase the counter>-----
template<typename Data>
void NoSqlDb<Data>::save() 
{
	mtx.lock();
	++counter;
	mtx.unlock();
	return;
}

// -- <call this function to insert an element to DB>-----
template<typename Data>
bool NoSqlDb<Data>::addRecord(Key key, Element<Data> elem) 
{
	try 
	{
		auto item = std::make_pair(key, elem);
		store.insert(item);
		//++counter;
		return false;
	}
	catch (std::exception& ex) 
	{
		std::cout << "Error: " << ex.what() << "\n";
	}
	return true;
}

// ----<overloading addRecord function: accepct std::pair<std::string, Element> param>-----
template<typename Data>
bool NoSqlDb<Data>::addRecord(Item item) 
{
	try
	{
		store.insert(item);
		//++counter;
		return false;
	}
	catch (std::exception& ex)
	{
		std::cout << "Error: " << ex.what() << "\n";
	}
	return true;
}

// --- <remove a record by its key>------
template<typename Data>
bool NoSqlDb<Data>::removeRecord(const Key& key)
{
	if (store.find(key) != store.end()) 
	{
		Keys allks = keys();
		for (auto origin : allks)
		{
			Element<Data> temp = getRecord(origin);
			if (std::find(temp.children.begin(), temp.children.end(), key) != temp.children.end())
				deleteRelationship(origin,key);
		}
		//++counter;
		store.erase(key);
		std::cout << "success\n";
		return true;
	}
	return false;
}

// ----<get value of specific key>----
template<typename Data>
Element<Data> NoSqlDb<Data>::getRecord(Key key) 
{
	for (auto it = store.begin(); it != store.end(); it++) 
	{
		std::string temp=it->first;
		if (!trim(temp).compare(trim(key)))
			return store[temp];
	}
	std::cout << "\n record not found \n";
	return Element<Data>();
}

// ----<get the value of specific key: const type>-----
template<typename Data>
const Element<Data> NoSqlDb<Data>::value(Key key) const
{
	if (store.find(key) != store.end())
		return store.at(key);
	return Element<Data>();
}

// ---<find all the values of NoSQL DB>------
template<typename Data>
std::vector<Element<Data>> NoSqlDb<Data>::findAll() 
{
	std::vector<Element<Data>> vec;
	for (auto elem : store) 
	{
		vec.push_back(elem.second);
	}
	return vec;
}

// ---- <replace a record a new value>-----
template<typename Data>
bool NoSqlDb<Data>::replaceValue(Key key, Element<Data> newValue) 
{
	auto it = store.find(key);
	if (it != store.end()) 
	{
		it->second = newValue;
		return true;
	}
	std::cout << "\n record not found \n";
	return false;
}

// --- <return size of DB>-------
template<typename Data>
size_t NoSqlDb<Data>::count()
{
	return store.count();
}

// ----------<Persistance functions>-------
namespace PersistFunc 
{
	using namespace XmlProcessing;
	using SPtr = std::shared_ptr<AbstractXmlElement>;

	template<typename Data>
	std::string DBtoXML_V1(const NoSqlDb<Data>& db)
	{
		std::string xml;
		XmlDocument doc;
		SPtr pRoot = makeTaggedElement("NoSqlDb");
		doc.docElement() = pRoot;
		NoSqlDb<Data>::Keys Keys = db.keys();
		for (auto Key : Keys)
		{
			SPtr pKeyElement = makeTaggedElement("key");
			pRoot->addChild(pKeyElement);
			SPtr pTextElement = makeTextElement(trim(db.value(Key).name.getConstValue()));
			pKeyElement->addChild(pTextElement);

			SPtr pValueElement = makeTaggedElement("value");
			pRoot->addChild(pValueElement);

			SPtr pNameElement = makeTaggedElement("name");
			pValueElement->addChild(pNameElement);
			pTextElement = makeTextElement(db.value(Key).name);
			pNameElement->addChild(pTextElement);

			SPtr pCat = makeTaggedElement("path");
			pValueElement->addChild(pCat);
			pTextElement = makeTextElement(db.value(Key).categoty);
			pCat->addChild(pTextElement);

			SPtr pTime = makeTaggedElement("timedate");
			pValueElement->addChild(pTime);
			pTextElement = makeTextElement(db.value(Key).timeDate);
			pTime->addChild(pTextElement);

			SPtr pChild = makeTaggedElement("dependecy");
			pValueElement->addChild(pChild);
			for (auto content : db.value(Key).children)
			{
				SPtr key = makeTaggedElement("relatedFile");
				pChild->addChild(key);
				SPtr KeyStr = makeTextElement(content);
				key->addChild(KeyStr);
			}
			SPtr pData = makeTaggedElement("data");
			pValueElement->addChild(pData);
			// data is std::string
			pTextElement = makeTextElement(db.value(Key).data);
			pData->addChild(pTextElement);
		}
		xml = doc.toString();
		return xml;
	}

	//----------<Specialization for int type>-------------
	template<>
	std::string DBtoXML_V1(const NoSqlDb<int>& db)
	{	std::string xml;
		XmlDocument doc;
		SPtr pRoot = makeTaggedElement("NoSqlDb");
		doc.docElement() = pRoot;
		NoSqlDb<int>::Keys Keys = db.keys();
		for (auto Key : Keys){
			SPtr pKeyElement = makeTaggedElement("key");
			pRoot->addChild(pKeyElement);
			SPtr pTextElement = makeTextElement(trim(db.value(Key).name.getConstValue()));
			pKeyElement->addChild(pTextElement);
			SPtr pValueElement = makeTaggedElement("value");
			pRoot->addChild(pValueElement);
			SPtr pNameElement = makeTaggedElement("name");
			pValueElement->addChild(pNameElement);
			pTextElement = makeTextElement(db.value(Key).name);
			pNameElement->addChild(pTextElement);

			SPtr pCat = makeTaggedElement("path");
			pValueElement->addChild(pCat);
			pTextElement = makeTextElement(db.value(Key).categoty);
			pCat->addChild(pTextElement);

			SPtr pTime = makeTaggedElement("timedate");
			pValueElement->addChild(pTime);
			pTextElement = makeTextElement(db.value(Key).timeDate);
			pTime->addChild(pTextElement);

			SPtr pChild = makeTaggedElement("dependency");
			pValueElement->addChild(pChild);
			for (auto content : db.value(Key).children)
			{
				SPtr key = makeTaggedElement("relatedFile");
				pChild->addChild(key);
				SPtr KeyStr = makeTextElement(content);
				key->addChild(KeyStr);
			}
			SPtr pData = makeTaggedElement("data");
			pValueElement->addChild(pData);
			std::string dt = Convert<int>::toString(db.value(Key).data);
			pTextElement = makeTextElement(dt);
			pData->addChild(pTextElement);
		}
		xml = doc.toString();
		return xml;
	}

	template<typename Data>
	NoSqlDb<Data>& DBfromXML_V1(NoSqlDb<Data>& db, const std::string& xml)
	{
		try{XmlDocument doc(xml, XmlDocument::str);
			//fetch the name string from XML element
			std::vector<SPtr> keys = doc.descendents("key").select();
			for (size_t i = 0; i< keys.size(); i++)
			{
				// Extract Key
				std::string _key = keys[i]->children()[0]->value();
				// Extract Value
				Element<Data> elem;
				std::vector<SPtr> desc = doc.descendents("name").select();
				std::string name_value = desc[i]->children()[0]->value();
				name_value = trim(name_value);
				elem.name = name_value;
				//fetch category
				desc = doc.descendents("path").select();
				std::string category = desc[i]->children()[0]->value();
				category = trim(category);
				elem.categoty = category;
				//fetch timedate
				desc = doc.descendents("timedate").select();
				std::string timedate = desc[i]->children()[0]->value();
				timedate = trim(timedate);
				elem.timeDate = timedate;
				//fectch children
				desc = doc.descendents("relatedFile").select();
				std::vector<SPtr> children = desc[i]->children();
				for (auto child : children)
				{
					std::string key = child->children()[0]->value();
					key = trim(key);
					elem.children.push_back(key);
				}
				// data is defualt string type
				desc = doc.descendents("data").select();
				std::string data = desc[i]->children()[0]->value();
				data = trim(data);
				elem.data = data;
				auto Item = std::make_pair(trim(_key), elem);
				db.addRecord(Item);
			}}
		catch (std::exception& ex)
		{
			std::cout << "\n something bad happend";
			std::cout << "\n " << ex.what();
		}
		return db;
}}

#endif // !DOSQLDB_TEST