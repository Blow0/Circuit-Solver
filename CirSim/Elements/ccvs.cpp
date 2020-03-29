#include "ccvs.h"

#include "resistor.h"
#include "inductor.h"
#include "capacitor.h"
#include "currentsource.h"
#include "voltagesource.h"
#include "cccs.h"
#include "vcvs.h"
#include "vccs.h"

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
		throw std::logic_error("CCVS: Couldn't find a Node.");

	if (voltageIndexMap.find(m_name) == voltageIndexMap.end())
		throw std::logic_error("CCVS: Couldn't find a Voltage Source.");

	size_t posIdx = nodeIndexMap[m_posNode->getName()];
	size_t negIdx = nodeIndexMap[m_negNode->getName()];
	size_t constRow = matrixWidth - 1;
	size_t voltageIdx = voltageIndexMap[m_name];
	Element* controlElement = getControlElement();

	Complex currentFactor = getCurrentFactor();
	Complex internalImpedance = getInternalImpedance();
	matrix[voltageIdx * matrixWidth + posIdx] = 1;
	matrix[voltageIdx * matrixWidth + negIdx] = -1;
	matrix[voltageIdx * matrixWidth + voltageIdx] = internalImpedance;

	switch (controlElement->getType())
	{
		case ElementType::Resistor:
		{
			Resistor* resistor = static_cast<Resistor*>(controlElement);
			size_t controlPosIdx = nodeIndexMap[resistor->getPosNode()->getName()];
			size_t controlNegIdx = nodeIndexMap[resistor->getNegNode()->getName()];
			Complex resistorFactor = currentFactor * resistor->getAdmittance();
			matrix[voltageIdx * matrixWidth + controlPosIdx] -= resistorFactor;
			matrix[voltageIdx * matrixWidth + controlNegIdx] += resistorFactor;
		}
		break;
		case ElementType::Capacitor:
		{
			Capacitor* capacitor = static_cast<Capacitor*>(controlElement);
			size_t controlPosIdx = nodeIndexMap[capacitor->getPosNode()->getName()];
			size_t controlNegIdx = nodeIndexMap[capacitor->getNegNode()->getName()];
			Complex capacitorFactor = currentFactor * capacitor->getAdmittance(angularFrequency);
			matrix[voltageIdx * matrixWidth + controlPosIdx] -= capacitorFactor;
			matrix[voltageIdx * matrixWidth + controlNegIdx] += capacitorFactor;
		}
		break;
		case ElementType::Inductor:
		{
			Inductor* inductor = static_cast<Inductor*>(controlElement);
			size_t controlPosIdx = nodeIndexMap[inductor->getPosNode()->getName()];
			size_t controlNegIdx = nodeIndexMap[inductor->getNegNode()->getName()];
			Complex inductorFactor = currentFactor * inductor->getAdmittance(angularFrequency);
			matrix[voltageIdx * matrixWidth + controlPosIdx] -= inductorFactor;
			matrix[voltageIdx * matrixWidth + controlNegIdx] += inductorFactor;
		}
		break;
		case ElementType::CS:
		{
			CurrentSource* currentSource = static_cast<CurrentSource*>(controlElement);
			size_t currentSourcePosIdx = nodeIndexMap[currentSource->getPosNode()->getName()];
			size_t currentSourceNegIdx = nodeIndexMap[currentSource->getNegNode()->getName()];
			Complex supplyVoltage = currentFactor * currentSource->getSupplyCurrent();
			Complex internalAdmittanceFactor = currentFactor * currentSource->getInternalAdmittance();
			matrix[voltageIdx * matrixWidth + constRow] += supplyVoltage;
			matrix[voltageIdx * matrixWidth + currentSourcePosIdx] += internalAdmittanceFactor;
			matrix[voltageIdx * matrixWidth + currentSourceNegIdx] -= internalAdmittanceFactor;
		}
		break;
		case ElementType::VS:
		{
			VoltageSource* voltageSource = static_cast<VoltageSource*>(controlElement);
			size_t voltageSourceIdx = voltageIndexMap[voltageSource->getName()];
			matrix[voltageIdx * matrixWidth + voltageSourceIdx] = -currentFactor;
		}
		break;
		case ElementType::VCVS:
		{
			VCVS* vcvs = static_cast<VCVS*>(controlElement);
			size_t vcvsIdx = voltageIndexMap[vcvs->getName()];
			matrix[voltageIdx * matrixWidth + vcvsIdx] = -currentFactor;
		}
		break;
		case ElementType::CCVS:
		{
			CCVS* ccvs = static_cast<CCVS*>(controlElement);
			size_t ccvsIdx = voltageIndexMap[ccvs->getName()];
			matrix[voltageIdx * matrixWidth + ccvsIdx] = -currentFactor;
		}
		break;
		case ElementType::VCCS:
		{
			VCCS* vccs = static_cast<VCCS*>(controlElement);
			size_t vccsPosIdx = nodeIndexMap[vccs->getPosNode()->getName()];
			size_t vccsNegIdx = nodeIndexMap[vccs->getNegNode()->getName()];
			size_t vccsControlPosIdx = nodeIndexMap[vccs->getControlPosNode()->getName()];
			size_t vccsControlNegIdx = nodeIndexMap[vccs->getControlNegNode()->getName()];
			Complex vccsFactor = currentFactor * vccs->getVoltageFactor();
			Complex vccsAdmittanceFactor = vccsFactor * vccs->getInternalAdmittance();
			matrix[voltageIdx * matrixWidth + vccsControlPosIdx] -= vccsFactor;
			matrix[voltageIdx * matrixWidth + vccsControlNegIdx] += vccsFactor;
			matrix[voltageIdx * matrixWidth + vccsPosIdx] += vccsAdmittanceFactor;
			matrix[voltageIdx * matrixWidth + vccsNegIdx] -= vccsAdmittanceFactor;
		}
		break;
		case ElementType::CCCS:
		{
			CCCS* cccs = static_cast<CCCS*>(controlElement);
			cccs->injectSupplyCurrentIntoCCVS(matrix, matrixWidth, this, currentFactor, nodeIndexMap, voltageIndexMap, angularFrequency);
		}
		break;
	}

	matrix[posIdx * matrixWidth + voltageIdx] = -1;
	matrix[negIdx * matrixWidth + voltageIdx] = 1;
}