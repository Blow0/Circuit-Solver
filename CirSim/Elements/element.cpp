#include "element.h"

//Elements Map
std::map<std::string, Element*> Element::elementsMap;

//Constructors
Element::Element(const std::string& name, ElementType type)
	: m_name(name)
	, m_type(type)
{
	elementsMap.emplace(elementNameWithType(name, type), this);
}

Element::~Element()
{
	elementsMap.erase(elementTypeToString(m_type) + m_name);
}

std::string Element::elementTypeToString(ElementType type)
{
	std::string typeStr;
	switch (type)
	{
	case ElementType::Resistor:
		typeStr = "rtor";
		break;
	case ElementType::Inductor:
		typeStr = "itor";
		break;
	case ElementType::Capacitor:
		typeStr = "ctor";
		break;
	case ElementType::CS:
		typeStr = "csrc";
		break;
	case ElementType::VS:
		typeStr = "vsrc";
		break;
	case ElementType::CCCS:
		typeStr = "cccs";
		break;
	case ElementType::CCVS:
		typeStr = "ccvs";
		break;
	case ElementType::VCCS:
		typeStr = "vccs";
		break;
	case ElementType::VCVS:
		typeStr = "vcvs";
		break;
	}
	return typeStr;
}

ElementType Element::stringToElementType(const std::string& typeStr)
{
	ElementType type = ElementType::None;
	switch (typeStr.at(0))
	{
	case 'r':
		type = ElementType::Resistor;
		break;
	case 'l':
		type = ElementType::Inductor;
		break;
	case 'c':
		if (typeStr.size() > 1)
		{
			switch (typeStr.at(1))
			{
			case 'v':
				type = ElementType::CCVS;
				break;
			case 'c':
				type = ElementType::CCCS;
				break;
			case 's':
				type = ElementType::CS;
				break;
			}
		}
		else
			type = ElementType::Capacitor;
		break;
	case 'v':
		if (typeStr.size() > 1)
		{
			switch (typeStr.at(1))
			{
			case 'v':
				type = ElementType::VCVS;
				break;
			case 'c':
				type = ElementType::VCCS;
				break;
			case 's':
				type = ElementType::VS;
				break;
			}
		}
		break;
	default:
		type = ElementType::None;
	}
	return type;
}

void Element::deleteAllElements()
{
	std::map<std::string, Element*>::iterator it = elementsMap.begin();
	while (it != elementsMap.end())
	{
		delete it->second;
		it = elementsMap.begin();
	}
}

void Element::loadAllElementsIntoMatrix(Complex* matrix, size_t matrixWidth, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	for (std::map<std::string, Element*>::iterator it = elementsMap.begin(); it != elementsMap.end(); it++)
		it->second->injectIntoMatrix(matrix, matrixWidth, nodeIndexMap, voltageIndexMap, angularFrequency);
}