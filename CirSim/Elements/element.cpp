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
	elementsMap.erase(m_name);
}
