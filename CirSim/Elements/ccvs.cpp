#include "ccvs.h"

#include "resistor.h"
#include "inductor.h"
#include "capacitor.h"
#include "currentsource.h"
#include "voltagesource.h"
#include "vcvs.h"
#include "vccs.h"
#include "cccs.h"
#include "cccs.h"

//Constructors
CCVS::CCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex currentFactor, const std::string& controlElement, Complex internalImpedance)
	: VoltageSource(ccvsName, posNode, negNode, 0.0, internalImpedance)
	, m_controlElement(controlElement)
	, m_currentFactor(currentFactor)
{
	setType(ElementType::CCVS);
}

CCVS::~CCVS()
{
	m_controlElement = nullptr;
}

//Static Voltage Source Creation 
CCVS* CCVS::createCCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex currentFactor, const std::string& controlCurrent, Complex internalImpedance)
{
	std::string name = "ccvs" + ccvsName;
	if (elementExists(name))
		return (CCVS*)elementsMap[name];

	CCVS* ccvs = new CCVS(ccvsName, posNode, negNode, currentFactor, controlCurrent, internalImpedance);
	elementsMap.emplace(name, ccvs);
	return ccvs;
}

//Matrix Operations
void CCVS::injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end())
		throw std::runtime_error("CCVS: Couldn't find a Node.");

	if (voltageIndexMap.find(m_name) == voltageIndexMap.end())
		throw std::runtime_error("CCVS: Couldn't find a Voltage Source.");

	Element* controlElement = getControlElement();
	size_t posIdx = nodeIndexMap[m_posNode->getName()];
	size_t negIdx = nodeIndexMap[m_negNode->getName()];
	size_t voltageIdx = voltageIndexMap[m_name];

	Complex internalImpedance = getInternalImpedance();
	matrix[voltageIdx * matrixWidth + posIdx] = 1;
	matrix[voltageIdx * matrixWidth + negIdx] = -1;
	matrix[voltageIdx * matrixWidth + voltageIdx] = internalImpedance;
	controlElement->injectVSCurrentControlIntoMatrix(matrix, matrixWidth, this, getCurrentFactor(), nodeIndexMap, voltageIndexMap, angularFrequency);
	matrix[posIdx * matrixWidth + voltageIdx] = -1;
	matrix[negIdx * matrixWidth + voltageIdx] = 1;
}

void CCVS::injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	if (voltageIndexMap.find(m_name) == voltageIndexMap.end()
		|| voltageIndexMap.find(ccvs->getName()) == voltageIndexMap.end())
		throw std::runtime_error("CCVS: Couldn't find a Voltage Source.");

	size_t voltageSourceIdx = voltageIndexMap[m_name];
	size_t voltageIdx = voltageIndexMap[ccvs->getName()];

	matrix[voltageIdx * matrixWidth + voltageSourceIdx] = -totalCurrentFactor;
}

void CCVS::injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(cccs->getPosNode()->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getNegNode()->getName()) == nodeIndexMap.end())
		throw std::runtime_error("CCVS: Couldn't find a Node.");

	if (voltageIndexMap.find(m_name) == voltageIndexMap.end())
		throw std::runtime_error("CCVS: Couldn't find a Voltage Source.");

	size_t posIdx = nodeIndexMap[cccs->getPosNode()->getName()];
	size_t negIdx = nodeIndexMap[cccs->getNegNode()->getName()];
	size_t voltageSourceIdx = voltageIndexMap[m_name];

	matrix[posIdx * matrixWidth + voltageSourceIdx] -= totalCurrentFactor;
	matrix[negIdx * matrixWidth + voltageSourceIdx] += totalCurrentFactor;
}

//Getters
Complex CCVS::getControlCurrent(double angularFrequency) const
{
	Complex current;
	Element* controlElement = getControlElement();
	switch (controlElement->getType())
	{
	case ElementType::Resistor:
		current = ((Resistor*)controlElement)->getCurrent();
		break;
	case ElementType::Inductor:
		current = ((Inductor*)controlElement)->getCurrent(angularFrequency);
		break;
	case ElementType::Capacitor:
		current = ((Capacitor*)controlElement)->getCurrent(angularFrequency);
		break;
	case ElementType::VS:
		current = ((VoltageSource*)controlElement)->getCurrent();
		break;
	case ElementType::CS:
		current = ((CurrentSource*)controlElement)->getCurrent();
		break;
	case ElementType::VCVS:
		current = ((VCVS*)controlElement)->getCurrent();
		break;
	case ElementType::VCCS:
		current = ((VCCS*)controlElement)->getCurrent();
		break;
	case ElementType::CCVS:
		current = ((CCVS*)controlElement)->getCurrent();
		break;
	case ElementType::CCCS:
		current = ((CCCS*)controlElement)->getCurrent();
		break;
	}
	return getCurrentFactor() * current;
}