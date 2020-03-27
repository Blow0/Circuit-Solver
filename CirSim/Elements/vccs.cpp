#include "vccs.h"

//Constructors
VCCS::VCCS(const std::string& vcvsName, Node& posNode, Node& negNode, Complex factor, Node* controlPosNode, Node* controlNegNode, Complex internalAdmittance)
	: CurrentSource(vcvsName, posNode, negNode, 0.0, internalAdmittance)
	, m_controlPosNode(controlPosNode)
	, m_controlNegNode(controlNegNode)
	, m_voltageFactor(factor)
{
	setType(ElementType::VCCS);
}

VCCS::~VCCS()
{
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