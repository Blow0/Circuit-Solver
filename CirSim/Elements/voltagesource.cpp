#include "voltagesource.h"

//Static Member definition
unsigned int VoltageSource::voltageSourceCount = 0;

//Constructors
VoltageSource::VoltageSource(const std::string& voltageSrcName, Node& posNode, Node& negNode, double voltage = 0, double phase = 0)
	: Element(voltageSrcName, ElementType::VS)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_voltage(voltage*cos(phase), voltage*sin(phase))
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
VoltageSource* VoltageSource::createVoltageSource(const std::string& voltageSrcName, Node& posNode, Node& negNode, double voltage = 0, double phase = 0)
{
	std::string name = "vs" + voltageSrcName;
	if (elementExists(name))
		return (VoltageSource*)elementsMap[name];

	VoltageSource* voltagesource = new VoltageSource(voltageSrcName, posNode, negNode, voltage, phase);
	elementsMap.emplace(name, voltagesource);
	return voltagesource;
}