#include "inductor.h"

//Constructors
Inductor::Inductor(const std::string& inductorName, Node& posNode, Node& negNode, double inductance)
	: Element(inductorName, ElementType::Inductor)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_inductance(abs(inductance))
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);
}

Inductor::~Inductor()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;
}

//Static Resistor Creation
Inductor* Inductor::createInductor(const std::string& inductorName, Node& posNode, Node& negNode, double inductance)
{
	std::string name = "l" + inductorName;
	if (elementExists(name))
		return (Inductor*)elementsMap[name];

	Inductor* inductor = new Inductor(inductorName, posNode, negNode, inductance);
	elementsMap.emplace(name, inductor);
	return inductor;
}
