#include "capacitor.h"

//Constructors
Capacitor::Capacitor(const std::string& capacitorName, Node& posNode, Node& negNode, double capacitance)
	: Element(capacitorName, ElementType::Capacitor)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_capacitance(abs(capacitance))
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);
}

Capacitor::~Capacitor()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;
}

//Static Resistor Creation
Capacitor* Capacitor::createCapacitor(const std::string& capacitorName, Node& posNode, Node& negNode, double capacitance)
{
	std::string name = "c" + capacitorName;
	if (elementExists(name))
		return (Capacitor*)elementsMap[name];

	Capacitor* capacitor = new Capacitor(capacitorName, posNode, negNode, capacitance);
	elementsMap.emplace(name, capacitor);
	return capacitor;
}
