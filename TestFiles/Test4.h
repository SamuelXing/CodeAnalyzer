#include "Test2.h"
///////////////////////////////////////////////////////////////
// Test4.h - This package is used to Test Code Analyzer      //
//                                                           //
// zihao xing, CSE687 - Object Oriented Design, Spring 2017  //
///////////////////////////////////////////////////////////////
/*
Package Operations:
==================
used for test

Note:
==================
Please look at the comments to check the type definitons

*/
// class definition
class Test4
{
public:
	Test4();
	~Test4();
	void DoThing() // do nothing here; depend on Test1.h & Test2.h
	{
		test1.doThing1(); // depend on Test1.h 
		Test2::t2++;      // depend on Test2.h
	}
private:
	TestHH::Test1 test1;
};

Test4::Test4()
{
}

Test4::~Test4()
{
}