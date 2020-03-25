#include "voltagesource.h"

//Static Member definition
unsigned int VoltageSource::voltageSourceCount = 0;

//Constructors
VoltageSource::VoltageSource(const std::string& voltageSrcName, Node& posNode, Node& negNode, Complex voltage)
	: Element(voltageSrcName, ElementType::VS)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_voltage(voltage)
	, m_current(0, 0)
{
	voltageSourceCount++;
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);
}

VoltageSource::~VoltageSource()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;
}

//Static Voltage Source Creation 
VoltageSource* VoltageSource::createVoltageSource(const std::string& voltageSrcName, Node& posNode, Node& negNode, Complex voltage)
{
	std::string name = "vs" + voltageSrcName;
	if (elementExists(name))
		return (VoltageSource*)elementsMap[name];

	VoltageSource* voltagesource = new VoltageSource(voltageSrcName, posNode, negNode, voltage);
	elementsMap.emplace(name, voltagesource);
	return voltagesource;
}