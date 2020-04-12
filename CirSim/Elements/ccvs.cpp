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
CCVS::CCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex currentFactor, ElementType controlElementType, const std::string& controlElementName, Complex internalImpedance)
	: VoltageSource(ccvsName, ElementType::CCVS, posNode, negNode, 0.0, internalImpedance)
	, m_controlElementType(controlElementType)
	, m_controlElementName(controlElementName)
	, m_currentFactor(currentFactor)
{
}

CCVS::~CCVS()
{
}

//Static Voltage Source Creation 
CCVS* CCVS::createCCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex currentFactor, ElementType controlElementType, const std::string& controlElementName, Complex internalImpedance)
{
	std::string name = elementNameWithType(ccvsName, ElementType::CCVS);
	CCVS* ccvs = (CCVS*)getElement(name);
	if (ccvs == nullptr)
		ccvs = new CCVS(ccvsName, posNode, negNode, currentFactor, controlElementType, controlElementName, internalImpedance);
	return ccvs;
}

//Matrix Operations
void CCVS::injectIntoMatrix(Complex* matrix, size_t matrixWidth, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end())
		throw std::runtime_error("CCVS: Couldn't find a Node.");

	if (voltageIndexMap.find(getName()) == voltageIndexMap.end())
		throw std::runtime_error("CCVS: Couldn't find a Voltage Source.");

	Element* controlElement = getControlElement();
	size_t posIdx = nodeIndexMap.at(m_posNode->getName());
	size_t negIdx = nodeIndexMap.at(m_negNode->getName());
	size_t voltageIdx = voltageIndexMap.at(getName());

	Complex internalImpedance = getInternalImpedance();
	matrix[voltageIdx * matrixWidth + posIdx] = 1;
	matrix[voltageIdx * matrixWidth + negIdx] = -1;
	matrix[voltageIdx * matrixWidth + voltageIdx] = internalImpedance;
	controlElement->injectVSCurrentControlIntoMatrix(matrix, matrixWidth, this, getCurrentFactor(), nodeIndexMap, voltageIndexMap, angularFrequency);
	matrix[posIdx * matrixWidth + voltageIdx] = -1;
	matrix[negIdx * matrixWidth + voltageIdx] = 1;
}

void CCVS::injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (voltageIndexMap.find(getName()) == voltageIndexMap.end()
		|| voltageIndexMap.find(ccvs->getName()) == voltageIndexMap.end())
		throw std::runtime_error("CCVS: Couldn't find a Voltage Source.");

	size_t voltageSourceIdx = voltageIndexMap.at(getName());
	size_t voltageIdx = voltageIndexMap.at(ccvs->getName());

	matrix[voltageIdx * matrixWidth + voltageSourceIdx] = -totalCurrentFactor;
}

void CCVS::injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(cccs->getPosNode()->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getNegNode()->getName()) == nodeIndexMap.end())
		throw std::runtime_error("CCVS: Couldn't find a Node.");

	if (voltageIndexMap.find(getName()) == voltageIndexMap.end())
		throw std::runtime_error("CCVS: Couldn't find a Voltage Source.");

	size_t posIdx = nodeIndexMap.at(cccs->getPosNode()->getName());
	size_t negIdx = nodeIndexMap.at(cccs->getNegNode()->getName());
	size_t voltageSourceIdx = voltageIndexMap.at(getName());

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
		current = dynamic_cast<Resistor*>(controlElement)->getCurrent();
		break;
	case ElementType::Inductor:
		current = dynamic_cast<Inductor*>(controlElement)->getCurrent(angularFrequency);
		break;
	case ElementType::Capacitor:
		current = dynamic_cast<Capacitor*>(controlElement)->getCurrent(angularFrequency);
		break;
	case ElementType::CS:
		current = dynamic_cast<CurrentSource*>(controlElement)->getCurrent();
		break;
	case ElementType::VS:
		current = dynamic_cast<VoltageSource*>(controlElement)->getCurrent();
		break;
	case ElementType::CCCS:
		current = dynamic_cast<CCCS*>(controlElement)->getCurrent(angularFrequency);
		break;
	case ElementType::CCVS:
		current = dynamic_cast<CCVS*>(controlElement)->getCurrent();
		break;
	case ElementType::VCCS:
		current = dynamic_cast<VCCS*>(controlElement)->getCurrent();
		break;
	case ElementType::VCVS:
		current = dynamic_cast<VCVS*>(controlElement)->getCurrent();
		break;
	}
	return getCurrentFactor() * current;
}