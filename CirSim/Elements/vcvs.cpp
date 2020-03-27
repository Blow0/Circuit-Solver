#include "vcvs.h"

std::list <VCVS*> VCVS::m_VCVS;
//Constructors
VCVS::VCVS(const std::string& vcvsName, Node& posNode, Node& negNode, Complex factor, Node* controlPosNode, Node* controlNegNode, Complex internalImpedance)
	: VoltageSource(vcvsName, posNode, negNode, 0.0, internalImpedance)
	, m_controlPosNode(controlPosNode)
	, m_controlNegNode(controlNegNode)
	, m_voltageFactor(factor)
{ 
	setType(ElementType::VCVS);
}

VCVS::~VCVS()
{
	m_controlPosNode = m_controlNegNode = nullptr;
	for (std::list<VCVS*>::iterator it = m_VCVS.begin(); it != m_VCVS.end(); it++)
	{
		if (this == *it)
		{
			m_VCVS.erase(it);
			break;
		}
	}
}

//Static Voltage Source Creation 
VCVS* VCVS::createVCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Node* controlPosNode, Node* controlNegNode, Complex internalImpedance)
{
	std::string name = "vcvs" + ccvsName;
	if (elementExists(name))
		return (VCVS*)elementsMap[name];

	VCVS* vcvs = new VCVS(ccvsName, posNode, negNode, factor, controlPosNode, controlNegNode, internalImpedance);
	m_VCVS.push_back(vcvs);
	elementsMap.emplace(name, vcvs);
	return vcvs;
}