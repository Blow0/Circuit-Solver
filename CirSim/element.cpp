#include "element.h"

//Constructors
Element::Element(const std::string& name, ElementType type)
	: m_name("")
	, m_type(type)
{
}

Element::~Element()
{
}

//Map Methods

bool Element::isFound(std::string elementName)
{
	if (elementsMap.find(elementName) == elementsMap.end())
		return false;
	return true;
}

Element* Element::createElement(std::string elementName)
{
	if (isFound(elementName))
	{
		return elementsMap[elementName];
	}
	else
	{
		elementsMap[elementName]->setName(elementName);
	}
}
void Element::eraseElement(std::string elementName)
{
	if (isFound(elementName)) //Avoid deleting uncreated elements
		elementsMap.erase(elementName);
}