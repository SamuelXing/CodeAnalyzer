///////////////////////////////////////////////////////////////
// Test5.h - This package is used to Test Code Analyzer      //
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

#include "Test3.h"
#include "Test1.h"

// class Test5
class Test5
{
public:
	Test5();
	~Test5();
	void haveFun() {} 
private:
	Test3::Test3 t3; //depend on Test3.h
};

Test5::Test5()
{
}

Test5::~Test5()
{
}