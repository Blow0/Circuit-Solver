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
	elementsMap.erase(elementTypeToElementString(m_type) + m_name);
}

std::string Element::elementTypeToElementString(ElementType type)
{
	std::string typeStr;
	switch (type)
	{
	case ElementType::Short:
		typeStr = "shrt";
		break;
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

std::string Element::elementTypeToInputString(ElementType type)
{
	std::string typeStr;
	switch (type)
	{
	case ElementType::Node:
		typeStr = "N";
		break;
	case ElementType::Short:
		typeStr = "SC";
		break;
	case ElementType::Resistor:
		typeStr = "R";
		break;
	case ElementType::Inductor:
		typeStr = "L";
		break;
	case ElementType::Capacitor:
		typeStr = "C";
		break;
	case ElementType::CS:
		typeStr = "CS";
		break;
	case ElementType::VS:
		typeStr = "VS";
		break;
	case ElementType::CCCS:
		typeStr = "CCCS";
		break;
	case ElementType::CCVS:
		typeStr = "CCVS";
		break;
	case ElementType::VCCS:
		typeStr = "VCCS";
		break;
	case ElementType::VCVS:
		typeStr = "VCVS";
		break;
	}
	return typeStr;
}

ElementType Element::stringToElementType(const std::string& typeStr)
{
	ElementType type = ElementType::None;
	switch (typeStr.at(0))
	{
	case 'n':
		type = ElementType::Node;
		break;
	case 's':
		if (typeStr.size() > 1 && typeStr.at(1) == 'c')
			type = ElementType::Short;
		break;
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
	std::map<std::string, Element*>::iterator temp;
	for (std::map<std::string, Element*>::iterator it = elementsMap.begin(); it != elementsMap.end();)
	{
		temp = it++;
		delete temp->second;
	}
}

void Element::loadAllElementsIntoMatrix(Complex* matrix, size_t matrixWidth, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	for (std::map<std::string, Element*>::iterator it = elementsMap.begin(); it != elementsMap.end(); it++)
		it->second->injectIntoMatrix(matrix, matrixWidth, nodeIndexMap, voltageIndexMap, angularFrequency);
}