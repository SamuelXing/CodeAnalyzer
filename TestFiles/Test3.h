#pragma once
#pragma once
// Test3.h
///////////////////////////////////////////////////////////////
// Test3.h - This package is used to Test Code Analyzer      //
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

#include <string>
#include <iostream>
#include "Test2.h"

namespace Test3 
{
	// class definiton
	void B() {} // global function
	class  Test3
	{
	public:
		 Test3();
		~ Test3();
		void doThing() 
		{
		}
	private:
		Test2::Test2 compo; // depend on Test2.h
	};

	 Test3:: Test3()
	{
	}

	 Test3::~ Test3()
	{
	}

}