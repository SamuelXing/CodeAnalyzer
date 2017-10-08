///////////////////////////////////////////////////////////////
// StrongComponent.cpp - declare the strong componnet class  //
//                                                           //
// Zihao Xing, CSE687 - Object Oriented Design, Spring 2017  //
///////////////////////////////////////////////////////////////
#include <iostream>
#include "StrongComponent.h"

// -----<test stub for StrongComponent class>---------
int main() 
{
	StrongComponent sc1(1);
	sc1.addOne("file1");
	sc1.addOne("file2");
	StrongComponent sc2(2);
	sc2.addOne("file3");
	sc2.addOne("file4");
	sc2.addOne("file5");
	sc2.removeOne("file4");
	StrongComponents* scc = new StrongComponents();
	scc->addSC(sc1);
	scc->addSC(sc2);
	scc->save(scc->persist(),"test.xml");
	delete scc;
}