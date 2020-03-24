#include "element.h"

//Elements Map
std::map<std::string, Element*> Element::elementsMap;

//Constructors
Element::Element(const std::string& name, ElementType type)
	: m_name(name)
	, m_type(type)
{
}

Element::~Element()
{
	//Remove Element from its nodes
	elementsMap.erase(m_name);
}

//Static Element Creation
Element* Element::createElement(std::string elementName, ElementType elementType)
{
	if (elementExists(elementName))
		return elementsMap[elementName];

	Element* element = new Element(elementName, elementType);
	elementsMap.emplace(elementName, element);
	return element;
}