#include "resistor.h"

//Constructors
Resistor::Resistor(const std::string& name, Node& posNode, Node& negNode, double resistance)
	: Element(name, ElementType::Resistor)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_resistance(check_not_zero(abs(resistance)))
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);
}

Resistor::~Resistor()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;
}

//Static Resistor Creation
Resistor* Resistor::createResistor(const std::string& resistorName, Node& posNode, Node& negNode, double resistance)
{
	std::string name = "r" + resistorName;
	if (elementExists(name))
		return (Resistor*)elementsMap[name];

	Resistor* resistor = new Resistor(resistorName, posNode, negNode, resistance);
	elementsMap.emplace(name, resistor);
	return resistor;
}
