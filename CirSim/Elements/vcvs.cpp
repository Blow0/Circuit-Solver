#include "vcvs.h"

//Constructors
VCVS::VCVS(const std::string& vcvsName, Node& posNode, Node& negNode, double factor, Complex controlVoltage)
	: Element(vcvsName, ElementType::VCVS)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_controlVoltage(controlVoltage)
	, m_factor(factor)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);

	m_voltage = m_controlVoltage * m_factor;
}

VCVS::VCVS(const std::string& vcvsName, Node& posNode, Node& negNode, Complex factor, Complex controlVoltage)
	: Element(vcvsName, ElementType::VCVS)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_controlVoltage(controlVoltage)
	, m_factor(1)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);

	m_voltage = m_controlVoltage * factor;
}

VCVS::~VCVS()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;
}

//Static Voltage Source Creation 
VCVS* VCVS::createVCVS(const std::string& ccvsName, Node& posNode, Node& negNode, double factor, Complex controlVoltage)
{
	std::string name = "vcvs" + ccvsName;
	if (elementExists(name))
		return (VCVS*)elementsMap[name];

	VCVS* vcvs = new VCVS(ccvsName, posNode, negNode, factor, controlVoltage);
	elementsMap.emplace(name, vcvs);
	return vcvs;
}

VCVS* VCVS::createVCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Complex controlVoltage)
{
	std::string name = "vcvs" + ccvsName;
	if (elementExists(name))
		return (VCVS*)elementsMap[name];

	VCVS* vcvs = new VCVS(ccvsName, posNode, negNode, factor, controlVoltage);
	elementsMap.emplace(name, vcvs);
	return vcvs;
}