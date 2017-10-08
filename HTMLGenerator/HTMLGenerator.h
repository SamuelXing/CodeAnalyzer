#pragma once
///////////////////////////////////////////////////////////////
// HTMLGenerator.h -- Define HTMLGenerator Class             //
//                                                           //
// Zihao Xing, CSE687 - Object Oriented Design, Spring 2017  //
///////////////////////////////////////////////////////////////

#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace XmlProcessing;
using SPtr = std::shared_ptr<AbstractXmlElement>;

class HTMLGenerator
{
public:
	HTMLGenerator() {}
	~HTMLGenerator(){}
	int MakeHTML(const std::string& filename)
	{
		//read file
		std::string fileString = retrieve(filename);
		std::string html;
		XmlDocument doc;
		SPtr pRoot = makeTaggedElement("html");
		doc.docElement() = pRoot;
		// make HTML head - may use a separated function to do this job
		SPtr pHead = makeTaggedElement("head");
		pHead->addAttrib("lang", "en");
		pRoot->addChild(pHead);
		// make meta tag
		SPtr pMetaCharset = makeTaggedElement("meta");
		pHead->addChild(pMetaCharset);
		pMetaCharset->addAttrib("charset","UTF-8");
		// make title
		SPtr pTitle = makeTaggedElement("title");
		SPtr pTitleText = makeTextElement(filename+".html");
		pTitle->addChild(pTitleText);
		pHead->addChild(pTitle);
		// add CSS
		SPtr pCSS = makeTaggedElement("link");
		pCSS->addAttrib("rel", "stylesheet"); pCSS->addAttrib("href","../static/CSS/Generator.css");
		pHead->addChild(pCSS);
		// add javascript
		// make HTML body
		SPtr pBody = makeTaggedElement("body");
		pRoot->addChild(pBody);
		// make file tile
		SPtr pH3 = makeTaggedElement("h3");
		SPtr pH3Text = makeTextElement(filename);
		pH3->addChild(pH3Text);
		pBody->addChild(pH3);
		// make HR 
		SPtr pHr = makeTaggedElement("hr");
		pBody->addChild(pHr);
		// make DIV
		SPtr pDiv = makeTaggedElement("div");
		pDiv->addAttrib("class","indent");
		pDiv->addChild(pDiv);
		// make dependency head
		SPtr pH4 = makeTaggedElement("h4");
		SPtr Ph4Text = makeTextElement("Dependencies:");
		pH4->addChild(Ph4Text);
		pDiv->addChild(pH4);
		// make dependency results -- introduce dependency table
		pBody->addChild(pHr);
		// make code text -- first step 
		SPtr pPre = makeTaggedElement("pre");
		SPtr pCodeText = makeTextElement(replaceBrackets(fileString));
		pBody->addChild(pPre);

		// persist html
		html = doc.toString();
		return save(html, "../static/HTML/"+filename+".html");
	}

	// makeHead
	// code modification
private:
	std::vector<std::string> filelist;
private:
	// ---------<read a file to a string>-----------
	std::string retrieve(const std::string& filename)
	{
		std::ifstream inxml{ filename };
		std::string xml;
		try
		{
			xml.assign((std::istreambuf_iterator<char>(inxml)), (std::istreambuf_iterator<char>()));
			inxml.close();
		}
		catch (const std::exception& ex)
		{
			std::cout << "Error: " << ex.what() << std::endl;
		}
		return xml;
	}
	// ---------<save a file to a string>-------------
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
		return 0;
	}
	std::string replaceBrackets(std::string& fileString)
	{
		size_t start_pos = 0;
		while ((start_pos=fileString.find("<",start_pos))!=std::string::npos) 
		{
			fileString.replace(start_pos, 1, "&lt;");
		}
		start_pos = 0;
		while ((start_pos = fileString.find(">", start_pos)) != std::string::npos)
		{
			fileString.replace(start_pos, 1, "&gt;");
		}
	}
};

