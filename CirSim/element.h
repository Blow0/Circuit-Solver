#ifndef _ELEMENT_H
#define _ELEMENT_H

#include <string>
#include <list>
#include "complex.h"

class Node;

enum class ElementType { Resistor };

class Element
{
private: //Members
	std::string m_name;
	ElementType m_type;

public: //Constructors
	Element(const std::string& name, ElementType type);
	~Element();

public: //Setters
	inline void setName(const std::string& name) { m_name = name; }

public: //Getters
	const std::string& geName() const { return m_name; }

	Element(const Element&) = delete;
	void operator=(const Element&) = delete;
};

#endif //_ELEMENT_H
