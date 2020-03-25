#ifndef _ELEMENT_H
#define _ELEMENT_H

#include <string>
#include <map>
#include "complex.h"

class Node;

enum class ElementType { Resistor, Capacitor, Inductor };

class Element
{
private: //Members
	std::string m_name;
	ElementType m_type;
	static Element* element;
	static std::map<std::string, Element*> elementsMap;
	
private: //Constructors
	Element(const std::string& name, ElementType type);
	~Element();

public: //Setters
	inline void setName(const std::string& name) { m_name = name; }

public: //Getters
	const std::string& geName() const { return m_name; }

	Element(const Element&) = delete;
	void operator=(const Element&) = delete;

public: //Map Methods
	static bool isFound(std::string elementName);
	static unsigned int getMapSize();
	static Element* createElement(std::string elementName);
	static void clean();

	void eraseElement(std::string elementName);
};

#endif //_ELEMENT_H
