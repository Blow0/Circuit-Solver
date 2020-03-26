#include "voltagesource.h"

//Static Member declaration
std::list <VoltageSource*> VoltageSource::m_voltageSources;

//Constructors
VoltageSource::VoltageSource(const std::string& voltageSrcName, Node& posNode, Node& negNode, Complex voltage)
	: Element(voltageSrcName, ElementType::VS)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_voltage(voltage)
	, m_current(0, 0)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);
}

VoltageSource::~VoltageSource()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;

	for (std::list<VoltageSource*>::iterator it = m_voltageSources.begin(); it != m_voltageSources.end(); it++)
		if (this == *it)
			m_voltageSources.erase(it);

}

//Static Voltage Source Creation 
VoltageSource* VoltageSource::createVoltageSource(const std::string& voltageSrcName, Node& posNode, Node& negNode, Complex voltage)
{
	std::string name = "vs" + voltageSrcName;
	if (elementExists(name))
		return (VoltageSource*)elementsMap[name];


	VoltageSource* voltagesource = new VoltageSource(voltageSrcName, posNode, negNode, voltage);
	elementsMap.emplace(name, voltagesource);
	m_voltageSources.push_back(voltagesource);
	return voltagesource;
}