#include "cccs.h"

//Static Member definition
unsigned int VoltageSource::voltageSourceCount = 0;

//Constructors
CCCS::CCCS(const std::string& cccsName, Node& posNode, Node& negNode, double factor = 1, double controlCurrent, double phase)
	: Element(cccsName, ElementType::CCCS)
	, CurrentControlledSource()
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_controlCurrent(controlCurrent* cos(phase), controlCurrent* sin(phase))
	, m_factor(factor)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);

	m_current = m_controlCurrent * m_factor;
}

CCCS::CCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex factor, double controlCurrent, double phase)
	: Element(cccsName, ElementType::CCCS)
	, CurrentControlledSource()
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_controlCurrent(controlCurrent* cos(phase), controlCurrent* sin(phase))
	, m_factor(1)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);

	m_current = m_controlCurrent * factor;
}

CCCS::~CCCS()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;
}

//Static Voltage Source Creation 
CCCS* CCCS::createCCCS(const std::string& ccvsName, Node& posNode, Node& negNode, double factor = 1, double controlCurrent, double phase)
{
	std::string name = "cccs" + ccvsName;
	if (elementExists(name))
		return (CCCS*)elementsMap[name];

	CCCS* cccs = new CCCS(ccvsName, posNode, negNode, factor, controlCurrent, phase);
	elementsMap.emplace(name, cccs);
	return cccs;
}

CCCS* CCCS::createCCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, double controlCurrent, double phase)
{
	std::string name = "cccs" + ccvsName;
	if (elementExists(name))
		return (CCCS*)elementsMap[name];

	CCCS* cccs = new CCCS(ccvsName, posNode, negNode, factor, controlCurrent, phase);
	elementsMap.emplace(name, cccs);
	return cccs;
}