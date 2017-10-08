// Test1.h
///////////////////////////////////////////////////////////////
// Test1.h - This package is used to Test Code Analyzer      //
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


#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "./Test3.h"

namespace A
{
	Test3::Test3 Hello; // depend on Test3.h
}

namespace B {
	int c; // global data
	inline void func() {} //global function
	enum MyEnum{   // enum definition

	};
}

using namespace B;

int kk=0;   // global data
inline void GlobalFunction() {} // test global function

namespace TestHH
{
  typedef int ADJ; // typedef
  using type = std::string; //alias
  class Test1// class
  { 
  public:
	Test1();
    Test1(const Test1&) = delete;
    Test1& operator=(const Test1&) = default;
	~Test1();
    void doThing1() { /* don't do anything */ }
  private:
    Scanner::SemiExp se;
  };

  // class definition
  class  B 
  {
  public:
	  B() {}
	  ~B() {}
  private:

  };
  // struct definiton
  struct MyStruct
  {
	  // found struct definiton
  };
}