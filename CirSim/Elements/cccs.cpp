#include "cccs.h"

//Constructors
CCCS::CCCS(const std::string& cccsName, Node& posNode, Node& negNode, double factor, Element* controlCurrent)
	: Element(cccsName, ElementType::CCCS)
	, CurrentControlledSource()
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_controlCurrent(controlCurrent)
	, m_factor(factor)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);

}

CCCS::CCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex factor, Element* controlCurrent)
	: Element(cccsName, ElementType::CCCS)
	, CurrentControlledSource()
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_controlCurrent(controlCurrent)
	, m_factor(1)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);
}

CCCS::~CCCS()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;
}

//Static Voltage Source Creation 
CCCS* CCCS::createCCCS(const std::string& cccsName, Node& posNode, Node& negNode, double factor, Element* controlCurrent)
{
	std::string name = "cccs" + cccsName;
	if (elementExists(name))
		return (CCCS*)elementsMap[name];

	CCCS* cccs = new CCCS(cccsName, posNode, negNode, factor, controlCurrent);
	elementsMap.emplace(name, cccs);
	return cccs;
}

CCCS* CCCS::createCCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex factor, Element* controlCurrent)
{
	std::string name = "cccs" + cccsName;
	if (elementExists(name))
		return (CCCS*)elementsMap[name];

	CCCS* cccs = new CCCS(cccsName, posNode, negNode, factor, controlCurrent);
	elementsMap.emplace(name, cccs);
	return cccs;
}