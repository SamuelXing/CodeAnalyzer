#pragma once
#include <iostream>
#include "./Test1.h"
///////////////////////////////////////////////////////////////
// Test2.h - This package is used to Test Code Analyzer      //
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

// flobal function definiton
void haveFun()
{
	std::cout << "have fun\n";
}

namespace Test2
{
	// enum definition
	enum MyEnumQ{};
	int t2; // global data
	class Test2 // class definiton
	{
	public:
		Test2() {}
		~Test2() {}
		void doThing()  // member function
		{
		}
		
	private:
		TestHH::Test1 t1; // depend on Test1.h
	};

}