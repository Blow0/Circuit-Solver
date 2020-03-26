#include "vccs.h"

//Constructors
VCCS::VCCS(const std::string& vcvsName, Node& posNode, Node& negNode, double factor, Node* controlPosNode, Node* controlNegNode)
	: Element(vcvsName, ElementType::VCCS)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_factor(factor)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);

	m_supplyCurrent = (controlPosNode->getNodalVoltage() - controlNegNode->getNodalVoltage()) * m_factor;
}

VCCS::VCCS(const std::string& vcvsName, Node& posNode, Node& negNode, Complex factor, Node* controlPosNode, Node* controlNegNode)
	: Element(vcvsName, ElementType::VCCS)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_factor(1)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);

	m_supplyCurrent = (controlPosNode->getNodalVoltage() - controlNegNode->getNodalVoltage()) * factor;
}

VCCS::~VCCS()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;
}

//Static Voltage Source Creation 
VCCS* VCCS::createVCCS(const std::string& ccvsName, Node& posNode, Node& negNode, double factor, Node* controlPosNode, Node* controlNegNode)
{
	std::string name = "vccs" + ccvsName;
	if (elementExists(name))
		return (VCCS*)elementsMap[name];

	VCCS* vccs = new VCCS(ccvsName, posNode, negNode, factor, controlPosNode, controlNegNode);
	elementsMap.emplace(name, vccs);
	return vccs;
}

VCCS* VCCS::createVCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Node* controlPosNode, Node* controlNegNode)
{
	std::string name = "vccs" + ccvsName;
	if (elementExists(name))
		return (VCCS*)elementsMap[name];

	VCCS* vccs = new VCCS(ccvsName, posNode, negNode, factor, controlPosNode, controlNegNode);
	elementsMap.emplace(name, vccs);
	return vccs;
}