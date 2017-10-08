// NoSqlDb.cpp - key/value pair in-memory StrDatabase

#include "./NoSqlDb.h"
#include <iostream>
#include <fstream>

using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;

#ifdef DOSQLDB_TEST
int main() 
{
	std::cout << "\n Demonstrating NoSql Helper Code";
	std::cout << "\n =============================== \n";

	std::cout << "\n Creating and saving NoSqlDb elements with string data";
	std::cout << "\n ------------------------------------------------------";

	NoSqlDb<StrData> db;
	db.initialize();

	std::cout << "\n\n";
	Element<StrData> elem;
	elem.name = "elem10";
	elem.categoty = "test";
	elem.timeDate = "2017/02/06 10:56:17";
	elem.addChild("elem9");
	elem.addChild("elem5");
	elem.data = "997";
	if (db.save(elem.name, elem))
	{
		std::cout << "can't save: key already exist";
	}
	Element<StrData> elem1;
	elem1.name = "elem11";
	elem1.categoty = "test";
	elem1.timeDate = "2017/02/06 10:56:19";
	elem1.addChild("elem8");
	elem1.addChild("elem7");
	elem1.data = "998";
	bool state = db.save(elem1.name, elem1);

	Keys keys = db.keys();
	for (Key key : keys)
	{
		std::cout << "\n " << key << ":";
		std::cout << db.getRecord(key).show();
	}
	db.close();
}
#endif // DOSQLDB_TEST

// uncomment lines below to test db to /from xml
/*std::string xml = PersistFunc::DBtoXML_V1<StrData>(db);
std::cout << xml << "\n\n";
*/
/*NoSqlDb<StrData> dbnew = PersistFunc::DBfromXML_V1<StrData>(xml);
std::cout << "\n\n";
Keys ikeys = dbnew.keys();
for (Key key : ikeys)
{
std::cout << "\n " << key << ":";
std::cout << dbnew.getRecord(key).show();
}*/

/*std::cout << "\n Creating and saving NoSqlDb elements with int data";
std::cout << "\n ----------------------------------------------------";

NoSqlDb<intData> idb;

Element<intData> ielem1;
ielem1.name = "elem1";
ielem1.categoty = "test";
ielem1.data = 1;

idb.save(ielem1.name, ielem1);

Element<intData> ielem2;
ielem2.name = "elem2";
ielem2.categoty = "test";
ielem2.data = 2;

idb.save(ielem2.name, ielem2);


Element<intData> ielem3;
ielem3.name = "elem3";
ielem3.categoty = "test";
ielem3.data = 3;

idb.save(ielem3.name, ielem3);

std::cout << "\n Retrieving elements from NoSqlDb<int>";
std::cout << "\n -----------------------------------\n";

Keys ibkeys = idb.keys();
for (Key key : ibkeys)
{
std::cout << "\n " << key << ":";
std::cout << idb.getRecord(key).show();
}
std::cout << "\n\n";

Key removekey = "elem2";
std::cout << "\n attemping to remove item with key =" << removekey;
int numRemoved = idb.removeRecord(removekey);
std::cout << "\n removed " << numRemoved << " items";*/