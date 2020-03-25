#include "ccvs.h"

//Static Member definition
unsigned int VoltageSource::voltageSourceCount = 0;

//Constructors
CCVS::CCVS(const std::string& ccvsName, Node& posNode, Node& negNode, double factor = 0, double controlCurrent, double phase)
	: Element(ccvsName, ElementType::CCVS)
	, CurrentControlledSource()
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_controlCurrent(controlCurrent*cos(phase), controlCurrent*sin(phase))
	, m_factor(factor)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);

	m_voltage = m_controlCurrent * m_factor;
}

CCVS::CCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, double controlCurrent, double phase)
	: Element(ccvsName, ElementType::CCVS)
	, CurrentControlledSource()
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_controlCurrent(controlCurrent* cos(phase), controlCurrent* sin(phase))
	, m_factor(1)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);

	m_voltage = m_controlCurrent * factor;
}

CCVS::~CCVS()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;
}

//Static Voltage Source Creation 
CCVS* CCVS::createCCVS(const std::string& ccvsName, Node& posNode, Node& negNode, double factor = 1, double controlCurrent, double phase)
{
	std::string name = "ccvs" + ccvsName;
	if (elementExists(name))
		return (CCVS*)elementsMap[name];

	CCVS* ccvs = new CCVS(ccvsName, posNode, negNode, factor, controlCurrent, phase);
	elementsMap.emplace(name, ccvs);
	return ccvs;
}

CCVS* CCVS::createCCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, double controlCurrent, double phase)
{
	std::string name = "ccvs" + ccvsName;
	if (elementExists(name))
		return (CCVS*)elementsMap[name];

	CCVS* ccvs = new CCVS(ccvsName, posNode, negNode, factor, controlCurrent, phase);
	elementsMap.emplace(name, ccvs);
	return ccvs;
}