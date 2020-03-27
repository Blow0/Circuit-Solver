#include "vccs.h"

//Constructors
VCCS::VCCS(const std::string& vcvsName, Node& posNode, Node& negNode, Complex factor, Node* controlPosNode, Node* controlNegNode, Complex internalAdmittance)
	: Element(vcvsName, ElementType::VCCS)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_supplyCurrent(0.0)
	, m_voltageFactor(factor)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);
}

VCCS::~VCCS()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;
	m_controlPosNode = m_controlNegNode = nullptr;
}

//Static Voltage Source Creation 
VCCS* VCCS::createVCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Node* controlPosNode, Node* controlNegNode, Complex internalAdmittance)
{
	std::string name = "vccs" + ccvsName;
	if (elementExists(name))
		return (VCCS*)elementsMap[name];

	VCCS* vccs = new VCCS(ccvsName, posNode, negNode, factor, controlPosNode, controlNegNode, internalAdmittance);
	elementsMap.emplace(name, vccs);
	return vccs;
}