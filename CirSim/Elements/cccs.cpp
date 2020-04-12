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
CCCS::CCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex currentFactor, ElementType controlElementType, const std::string& controlElementName, Complex internalAdmittance)
	: CurrentSource(cccsName, ElementType::CCCS, posNode, negNode, 0.0, internalAdmittance)
	, m_controlElementType(controlElementType)
	, m_controlElementName(controlElementName)
	, m_currentFactor(currentFactor)
{
}

CCCS::~CCCS()
{
}

//Static Voltage Source Creation 
CCCS* CCCS::createCCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex currentFactor, ElementType controlElementType, const std::string& controlElementName, Complex internalAdmittance)
{
	std::string name = elementNameWithType(cccsName, ElementType::CCCS);
	CCCS* cccs = (CCCS*)getElement(name);
	if (cccs == nullptr)
		cccs = new CCCS(cccsName, posNode, negNode, currentFactor, controlElementType, controlElementName, internalAdmittance);
	return cccs;
}

//Matrix Operations
void CCCS::injectIntoMatrix(Complex* matrix, size_t matrixWidth, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end())
		throw std::runtime_error("CCCS: Couldn't find a Node.");

	Element* controlElement = getControlElement();
	size_t posIdx = nodeIndexMap.at(m_posNode->getName());
	size_t negIdx = nodeIndexMap.at(m_negNode->getName());

	Complex internalAdmittance = getInternalAdmittance();
	controlElement->injectCSCurrentControlIntoMatrix(matrix, matrixWidth, this, getCurrentFactor(), nodeIndexMap, voltageIndexMap, angularFrequency);
	matrix[posIdx * matrixWidth + posIdx] += internalAdmittance;
	matrix[posIdx * matrixWidth + negIdx] -= internalAdmittance;
	matrix[negIdx * matrixWidth + posIdx] -= internalAdmittance;
	matrix[negIdx * matrixWidth + negIdx] += internalAdmittance;
}

void CCCS::injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end())
		throw std::runtime_error("CCCS: Couldn't find a Node.");

	if (voltageIndexMap.find(ccvs->getName()) == voltageIndexMap.end())
		throw std::runtime_error("CCCS: Couldn't find a Voltage Source.");

	Element* controlElement = getControlElement();
	size_t currentSourcePosIdx = nodeIndexMap.at(m_posNode->getName());
	size_t currentSourceNegIdx = nodeIndexMap.at(m_negNode->getName());
	size_t voltageIdx = voltageIndexMap.at(ccvs->getName());

	Complex internalAdmittanceFactor = totalCurrentFactor * getInternalAdmittance();
	controlElement->injectVSCurrentControlIntoMatrix(matrix, matrixWidth, ccvs, totalCurrentFactor * getCurrentFactor(), nodeIndexMap, voltageIndexMap, angularFrequency);
	matrix[voltageIdx * matrixWidth + currentSourcePosIdx] += internalAdmittanceFactor;
	matrix[voltageIdx * matrixWidth + currentSourceNegIdx] -= internalAdmittanceFactor;
}

void CCCS::injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getPosNode()->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getNegNode()->getName()) == nodeIndexMap.end())
		throw std::runtime_error("CCCS: Couldn't find a Node.");

	Element* controlElement = getControlElement();
	size_t currentSourcePosIdx = nodeIndexMap.at(m_posNode->getName());
	size_t currentSourceNegIdx = nodeIndexMap.at(m_negNode->getName());
	size_t posIdx = nodeIndexMap.at(cccs->getPosNode()->getName());
	size_t negIdx = nodeIndexMap.at(cccs->getNegNode()->getName());

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