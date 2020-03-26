#include "currentsource.h"


//Constructors
CurrentSource::CurrentSource(const std::string& currentSrcName, Node& posNode, Node& negNode, Complex current)
	: Element(currentSrcName, ElementType::CS)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_current(current)
	, m_voltage(0, 0)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);
}

CurrentSource::~CurrentSource()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;
}

//Static Voltage Source Creation 
CurrentSource* CurrentSource::createCurrentSource(const std::string& currentSrcName, Node& posNode, Node& negNode, Complex current)
{
	std::string name = "cs" + currentSrcName;
	if (elementExists(name))
		return (CurrentSource*)elementsMap[name];

	CurrentSource* currentsource = new CurrentSource(currentSrcName, posNode, negNode, current);
	elementsMap.emplace(name, currentsource);
	return currentsource;
}