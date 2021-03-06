///////////////////////////////////////////////////////////////
// TypeAnalysis.cpp - This package used to do type analysis  //
//                                                           //
// Zihao Xing, CSE687 - Object Oriented Design, Spring 2017  //
///////////////////////////////////////////////////////////////
/*
* TypeAnalysis.cpp: used as Test Stub for TypeAnalysis.h
*/
#include <iostream>
#include <string>
#include "../Analyzer/TypeAnalysis.h"
#include "../Parser/ConfigureParser.h"
#include "../Parser/Parser.h"

int main(int argc, char* argv[])
{
	using namespace CodeAnalysis;
	std::cout<<"Testing Type Analysis \n";
	// collecting tokens from files, named on the command line
	if (argc < 2)
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}
	for (int i = 1; i<argc; ++i)
	{
		std::string fileSpec = FileSystem::Path::getFullFileSpec(argv[i]);
		std::string msg = "Processing file" + fileSpec;
		std::cout << msg << std::endl;

		ConfigParseForCodeAnal configure;
		Parser* pParser = configure.Build();
		try
		{
			if (pParser)
			{
				if (!configure.Attach(argv[i]))
				{
					std::cout << "\n  could not open file " << fileSpec << std::endl;
					continue;
				}
			}
			else
			{
				std::cout << "\n\n  Parser not built\n\n";
				return 1;
			}
			while (pParser->next())
				pParser->parse();
			std::cout << "\n";
			// show AST
			Repository* pRepo = Repository::getInstance();
			ASTNode* pGlobalScope = pRepo->getGlobalScope();
			TreeWalk(pGlobalScope);
			TypeAnal ta;
			ta.doTypeAnal();
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
	}
	std::cout << "\n";
}