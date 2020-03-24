#ifndef _ELEMENT_H
#define _ELEMENT_H

#include <map>
#include <list>
#include <string>
#include "complex.h"

class Node;

enum class ElementType { Resistor, Capacitor, Inductor };

class Element
{
private: //Elements Map
	static std::map<std::string, Element*> elementsMap;

private: //Members
	std::string m_name;
	ElementType m_type;
	
private: //Constructors
	Element(const std::string& name, ElementType type);
	~Element();

public: //Static Elements Map Methods
	static inline bool elementExists(std::string elementName) { return (elementsMap.find(elementName) != elementsMap.end()); }
	static inline size_t getElementsCount() { return elementsMap.size(); }

protected: //Static Element Creation
	static Element* createElement(std::string elementName, ElementType elementType);

public: //Virtual Methods
	virtual Complex getVoltageDiff() const { return 0.0; }
	virtual Complex getCurrent() const { return 0.0; }

public: //Logic
	inline bool operator==(const Element& rhs) const { return rhs.m_name == m_name; }

public: //Setters
	inline void setName(const std::string& name) { m_name = name; }

public: //Getters
	inline const std::string& geName() const { return m_name; }

	Element(const Element&) = delete;
	void operator=(const Element&) = delete;
};

#endif //_ELEMENT_H
