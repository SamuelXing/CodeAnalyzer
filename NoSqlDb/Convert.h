///////////////////////////////////////////////////////////////////////
//Convert.h - (de)serializes objects of type T (from)to strings    ////
//  - generates a type identifier using Run-Time Type Identification //
// Author: zihao                                                     //
///////////////////////////////////////////////////////////////////////

//  class convert
//	virtual ~Convert<T>() {}
//  static std::string toString(const T& t);
//  static T fromString(const std::string& std);
//  static std::string id(const T& t);

// Maintanence History: 2017/2/7 - first release.


#pragma once
#include <sstream>
#include <string>

//Convert<T> class
// - (de)serializes objects of type T (from)to strings
// - generates a type identifier using Run-Time Type Identification

template<typename T>
class Convert
{
public:
	virtual ~Convert<T>() {}
	static std::string toString(const T& t);
	static T fromString(const std::string& std);
	static std::string id(const T& t);
};

//---<serialize t to a std::string >----
/*
* Assumes that T defines an insertion operator.
*/
template<typename T>
std::string Convert<T>::toString(const T& t) 
{
	std::ostringstream out;
	out << *const_cast<T*>(&t);
	return out.str();
}

template<typename T>
T Convert<T>::fromString(const std::string& str) 
{
	std::istringstream in(str);
	T value;
	in >> value; //istringstream extraction provides the conversion
	return value;
}

template<typename T>
std::string Convert<T>::id(const T& t) 
{
	return typeid(t).name;
}
