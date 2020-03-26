#include "vcvs.h"

//Constructors
VCVS::VCVS(const std::string& vcvsName, Node& posNode, Node& negNode, double factor, Node* controlPosNode, Node* controlNegNode, Complex internalImpedance )
	: VoltageSource(vcvsName, posNode, negNode, getVoltageDiff() * factor, internalImpedance)
	, m_controlPosNode(controlPosNode)
	, m_controlNegNode(controlNegNode)
	, m_factor(factor)
{
	
}

VCVS::VCVS(const std::string& vcvsName, Node& posNode, Node& negNode, Complex factor, Node* controlPosNode, Node* controlNegNode, Complex internalImpedance)
	: VoltageSource(vcvsName, posNode, negNode, getVoltageDiff()* factor, internalImpedance)
	, m_controlPosNode(controlPosNode)
	, m_controlNegNode(controlNegNode)
	, m_factor(1)
{

}

VCVS::~VCVS()
{
	VoltageSource::~VoltageSource();
	m_controlPosNode = m_controlNegNode = nullptr;
}

//Static Voltage Source Creation 
VCVS* VCVS::createVCVS(const std::string& ccvsName, Node& posNode, Node& negNode, double factor, Node* controlPosNode, Node* controlNegNode, Complex internalImpedance)
{
	std::string name = "vcvs" + ccvsName;
	if (elementExists(name))
		return (VCVS*)elementsMap[name];

	VCVS* vcvs = new VCVS(ccvsName, posNode, negNode, factor, controlPosNode, controlNegNode, internalImpedance);
	elementsMap.emplace(name, vcvs);
	return vcvs;
}

VCVS* VCVS::createVCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Node* controlPosNode, Node* controlNegNode, Complex internalImpedance)
{
	std::string name = "vcvs" + ccvsName;
	if (elementExists(name))
		return (VCVS*)elementsMap[name];

	VCVS* vcvs = new VCVS(ccvsName, posNode, negNode, factor, controlPosNode, controlNegNode, internalImpedance);
	elementsMap.emplace(name, vcvs);
	return vcvs;
}