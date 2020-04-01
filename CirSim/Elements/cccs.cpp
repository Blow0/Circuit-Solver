#include "cccs.h"

#include "resistor.h"
#include "inductor.h"
#include "capacitor.h"
#include "currentsource.h"
#include "voltagesource.h"
#include "ccvs.h"
#include "vcvs.h"
#include "vccs.h"

//Constructors
CCCS::CCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex currentFactor, const std::string& controlElement, Complex internalAdmittance)
	: CurrentSource(cccsName, posNode, negNode, 0.0, internalAdmittance)
	, m_controlElement(controlElement)
	, m_currentFactor(currentFactor)
{
	setType(ElementType::CCCS);
}

CCCS::~CCCS()
{
}

//Static Voltage Source Creation 
CCCS* CCCS::createCCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex currentFactor, const std::string& controlElement, Complex internalAdmittance)
{
	std::string name = "cccs" + cccsName;
	if (elementExists(name))
		return (CCCS*)elementsMap[name];

	CCCS* cccs = new CCCS(cccsName, posNode, negNode, currentFactor, controlElement, internalAdmittance);
	elementsMap.emplace(name, cccs);
	return cccs;
}

//Matrix Operations
void CCCS::injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end())
		throw std::runtime_error("CCCS: Couldn't find a Node.");

	Element* controlElement = getControlElement();
	size_t posIdx = nodeIndexMap[m_posNode->getName()];
	size_t negIdx = nodeIndexMap[m_negNode->getName()];

	Complex internalAdmittance = getInternalAdmittance();
	controlElement->injectCSCurrentControlIntoMatrix(matrix, matrixWidth, this, getCurrentFactor(), nodeIndexMap, voltageIndexMap, angularFrequency);
	matrix[posIdx * matrixWidth + posIdx] += internalAdmittance;
	matrix[posIdx * matrixWidth + negIdx] -= internalAdmittance;
	matrix[negIdx * matrixWidth + posIdx] -= internalAdmittance;
	matrix[negIdx * matrixWidth + negIdx] += internalAdmittance;
}

void CCCS::injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end())
		throw std::runtime_error("CCCS: Couldn't find a Node.");

	if (voltageIndexMap.find(ccvs->getName()) == voltageIndexMap.end())
		throw std::runtime_error("CCCS: Couldn't find a Voltage Source.");

	Element* controlElement = getControlElement();
	size_t currentSourcePosIdx = nodeIndexMap[m_posNode->getName()];
	size_t currentSourceNegIdx = nodeIndexMap[m_negNode->getName()];
	size_t voltageIdx = voltageIndexMap[ccvs->getName()];

	Complex internalAdmittanceFactor = totalCurrentFactor * getInternalAdmittance();
	controlElement->injectVSCurrentControlIntoMatrix(matrix, matrixWidth, ccvs, totalCurrentFactor * getCurrentFactor(), nodeIndexMap, voltageIndexMap, angularFrequency);
	matrix[voltageIdx * matrixWidth + currentSourcePosIdx] += internalAdmittanceFactor;
	matrix[voltageIdx * matrixWidth + currentSourceNegIdx] -= internalAdmittanceFactor;
}

void CCCS::injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getPosNode()->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getNegNode()->getName()) == nodeIndexMap.end())
		throw std::runtime_error("CCCS: Couldn't find a Node.");

	Element* controlElement = getControlElement();
	size_t currentSourcePosIdx = nodeIndexMap[m_posNode->getName()];
	size_t currentSourceNegIdx = nodeIndexMap[m_negNode->getName()];
	size_t posIdx = nodeIndexMap[cccs->getPosNode()->getName()];
	size_t negIdx = nodeIndexMap[cccs->getNegNode()->getName()];

	Complex internalAdmittanceFactor = totalCurrentFactor * getInternalAdmittance();
	controlElement->injectCSCurrentControlIntoMatrix(matrix, matrixWidth, cccs, totalCurrentFactor * getCurrentFactor(), nodeIndexMap, voltageIndexMap, angularFrequency);
	matrix[posIdx * matrixWidth + currentSourcePosIdx] += internalAdmittanceFactor;
	matrix[posIdx * matrixWidth + currentSourceNegIdx] -= internalAdmittanceFactor;
	matrix[negIdx * matrixWidth + currentSourcePosIdx] -= internalAdmittanceFactor;
	matrix[negIdx * matrixWidth + currentSourceNegIdx] += internalAdmittanceFactor;
}

//Getters
Complex CCCS::getControlCurrent(double angularFrequency) const
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