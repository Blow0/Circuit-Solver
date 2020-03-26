#include "vccs.h"

//Constructors
VCCS::VCCS(const std::string& vcvsName, Node& posNode, Node& negNode, double factor, Complex controlVoltage)
	: Element(vcvsName, ElementType::VCCS)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_controlVoltage(controlVoltage)
	, m_factor(factor)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);

	m_current = m_controlVoltage * m_factor;
}

VCCS::VCCS(const std::string& vcvsName, Node& posNode, Node& negNode, Complex factor, Complex controlVoltage)
	: Element(vcvsName, ElementType::VCCS)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_controlVoltage(controlVoltage)
	, m_factor(1)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);

	m_current = m_controlVoltage * factor;
}

VCCS::~VCCS()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;
}

//Static Voltage Source Creation 
VCCS* VCCS::createVCCS(const std::string& ccvsName, Node& posNode, Node& negNode, double factor, Complex controlVoltage)
{
	std::string name = "vccs" + ccvsName;
	if (elementExists(name))
		return (VCCS*)elementsMap[name];

	VCCS* vccs = new VCCS(ccvsName, posNode, negNode, factor, controlVoltage);
	elementsMap.emplace(name, vccs);
	return vccs;
}

VCCS* VCCS::createVCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Complex controlVoltage)
{
	std::string name = "vccs" + ccvsName;
	if (elementExists(name))
		return (VCCS*)elementsMap[name];

	VCCS* vccs = new VCCS(ccvsName, posNode, negNode, factor, controlVoltage);
	elementsMap.emplace(name, vccs);
	return vccs;
}