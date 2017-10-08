#pragma once
///////////////////////////////////////////////////////////////
// StrongComponent.h - declare the strong componnet class    //
//                                                           //
// Zihao Xing, CSE687 - Object Oriented Design, Spring 2017  //
///////////////////////////////////////////////////////////////
/*
Package Operations:
==================
This package provide the definition of StrongComponent class and 
StrongComponents class, StrongComponent describes the structure of
a single strong component, while the strongcompoents is used to connect 
single components.

Public Interface:
=================
StrongComponent:
- id(): return the id of a strongcomponent
- addOne(): add an element to current component
- removeOne(): remove an element from currect component

StrongComponents:
- addSC(): add a strongcomponent to currect StrongComponents
- std::string persist(): persits StrongComponents to XML string
- int save(const std::string& xml, const std::string& filename): save xml string to a file; 
																  return 0 if success.


Build Process:
==============
Required files
- StrongComponnet.h StrongComponents.cpp


Maintenance History:
====================
ver 1.0 : 08 Mar 2017
- first release
*/

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../FileSystem/FileSystem.h"


class StrongComponent
{
public:
	// -----<constructor>-------
	StrongComponent() { id_++; }
	StrongComponent(int id) { id_ = id; }
	std::vector<std::string>::iterator begin() { return SC.begin(); }
	std::vector<std::string>::iterator end() { return SC.end(); }

	// ----<get id>-------
	int id() 
	{
		return id_;
	}
	// ----<add one file to strong componnet>--------
	void addOne(std::string comp) 
	{
		SC.push_back(comp);
	};
	// -------<remove one file from strong component>------
	void removeOne(std::string comp)
	{
		for (size_t i = 0; i < SC.size(); i++) 
		{
			if (SC[i] == comp)
			{
				SC.erase(SC.begin() + i);
			}		
		}
	}
private:
	static int id_;
	std::vector<std::string> SC;
};

int StrongComponent::id_ = 0;

using namespace XmlProcessing;
class StrongComponents 
{
public:
	using SPtr = std::shared_ptr<AbstractXmlElement>;
	StrongComponents() {}
	// -----<add a strong component to strong components>------
	void addSC(StrongComponent sc) 
	{
		SCCs.push_back(sc);
	}
	// -------<Persist StrongComponent to a XML file>-------
	int save(const std::string& xml, const std::string& filename)
	{
		std::ofstream outxml;
		try
		{
			//outxml.open(filename, std::ios::app);
			outxml.open(filename);
			outxml << xml;
			outxml.close();			
		}
		catch (const std::exception& ex)
		{
			std::cout << "Error: " << ex.what() << std::endl;
		}
		std::cout << "Persisting the strong component to XML: -----Req# 7: persist SC analysis result to XML \n" << xml << "\n";
		std::cout << "\nAlready save the found strong component to " << FileSystem::Path::getFullFileSpec(filename) << std::endl;
		return 0;
	}

	// ------<persist the DB to XML>-------
	std::string persist()
	{
		std::string xml;
		XmlProcessing::XmlDocument doc;
		SPtr pRoot = makeTaggedElement("StrongComponents");
		doc.docElement() = pRoot;
		for (auto item : SCCs)
		{
			SPtr pKeyElement = makeTaggedElement("StrongComponent");
			pRoot->addChild(pKeyElement);

			for (std::string file : item)
			{
				SPtr pComponent = makeTaggedElement("Component");
				pKeyElement->addChild(pComponent);
				SPtr Text = makeTextElement(file);
				pComponent->addChild(Text);
			}
		}
		xml = doc.toString();
		return xml;
	}
private:
	std::vector<StrongComponent> SCCs;
};
