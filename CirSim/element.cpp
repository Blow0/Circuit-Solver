#include "element.h"

unsigned int VoltageSource::m_voltagSourcesCount = 0;
unsigned int CurrentCSource::m_currentControlledCount = 0;

//Constructors
Element::Element() : m_label(" "), m_posNode(0), m_negNode(0), m_current(0), m_voltageDiff(0) {}
Element::Element(std::string label, unsigned int posNode, unsigned int negNode)
	: m_label(label), m_posNode(posNode), m_negNode(negNode), m_current(0), m_voltageDiff(0) {}

Resistor::Resistor() : m_val(0) {}
Resistor::Resistor(std::string label, unsigned int posNode, unsigned int negNode, double val) :
	m_val(val)
{
	setLabel(label);
	setNodes(posNode, negNode);
	setComplex(val, 0);
}

Capacitor::Capacitor() : m_val(0), m_freq(1) {}
Capacitor::Capacitor(std::string label, unsigned int posNode, unsigned int negNode, double val, unsigned long freq = 1)
	: m_freq(freq)
{
	m_val = 1 / (val * freq);
	setLabel(label);
	setNodes(posNode, negNode);
	setComplex(0, -m_val);
}

Inductor::Inductor() : m_val(0), m_freq(1) {}
Inductor::Inductor(std::string label, unsigned int posNode, unsigned int negNode, double val, unsigned long freq = 1)
	: m_freq(freq)
{
	m_val = val * freq;
	setLabel(label);
	setNodes(posNode, negNode);
	setComplex(0, m_val);
}

Source::Source() : m_val(0) {  }
Source::Source(std::string label, unsigned int posNode, unsigned int negNode, double val, float phase)
	: m_val(val)
{
	setLabel(label);
	setNodes(posNode, negNode);
	setCartesian(val, phase);
}

VoltageSource::VoltageSource() : Source() { m_voltagSourcesCount++; }
VoltageSource::VoltageSource(std::string label, unsigned int posNode, unsigned int negNode, double val, float phase)
	: Source(label, posNode, negNode, val, phase)
{
	m_voltagSourcesCount++;
}

CurrentCSource::CurrentCSource() : Source() { m_currentControlledCount++; }
CurrentCSource::CurrentCSource(std::string label, unsigned int posNode, unsigned int negNode, double val, float phase)
	: Source(label, posNode, negNode, val, phase)
{
	m_currentControlledCount++;
}