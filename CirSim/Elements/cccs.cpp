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
CCCS::CCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex currentFactor, Element* controlElement, Complex internalAdmittance)
	: CurrentSource(cccsName, posNode, negNode, 0.0, internalAdmittance)
	, m_controlElement(controlElement)
	, m_currentFactor(currentFactor)
{
	setType(ElementType::CCCS);
}

CCCS::~CCCS()
{
	m_controlElement = nullptr;
}

//Static Voltage Source Creation 
CCCS* CCCS::createCCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex currentFactor, Element* controlElement, Complex internalAdmittance)
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
	size_t posIdx = nodeIndexMap[m_posNode->getName()];
	size_t negIdx = nodeIndexMap[m_negNode->getName()];
	size_t constRow = matrixWidth - 1;

	Complex currentFactor = getCurrentFactor();
	Complex internalAdmittance = getInternalAdmittance();

	switch (m_controlElement->getType())
	{
		case ElementType::Resistor:
		{
			Resistor* resistor = static_cast<Resistor*>(m_controlElement);
			size_t controlPosIdx = nodeIndexMap[resistor->getPosNode()->getName()];
			size_t controlNegIdx = nodeIndexMap[resistor->getNegNode()->getName()];
			Complex resistorFactor = currentFactor * resistor->getAdmittance();
			matrix[posIdx * matrixWidth + controlPosIdx] -= resistorFactor;
			matrix[posIdx * matrixWidth + controlNegIdx] += resistorFactor;
			matrix[negIdx * matrixWidth + controlPosIdx] += resistorFactor;
			matrix[negIdx * matrixWidth + controlNegIdx] -= resistorFactor;
		}
		break;
		case ElementType::Capacitor:
		{
			Capacitor* capacitor = static_cast<Capacitor*>(m_controlElement);
			size_t controlPosIdx = nodeIndexMap[capacitor->getPosNode()->getName()];
			size_t controlNegIdx = nodeIndexMap[capacitor->getNegNode()->getName()];
			Complex capacitorFactor = currentFactor * capacitor->getAdmittance(angularFrequency);
			matrix[posIdx * matrixWidth + controlPosIdx] -= capacitorFactor;
			matrix[posIdx * matrixWidth + controlNegIdx] += capacitorFactor;
			matrix[negIdx * matrixWidth + controlPosIdx] += capacitorFactor;
			matrix[negIdx * matrixWidth + controlNegIdx] -= capacitorFactor;
		}
		break;
		case ElementType::Inductor:
		{
			Inductor* inductor = static_cast<Inductor*>(m_controlElement);
			size_t controlPosIdx = nodeIndexMap[inductor->getPosNode()->getName()];
			size_t controlNegIdx = nodeIndexMap[inductor->getNegNode()->getName()];
			Complex inductorFactor = currentFactor * inductor->getAdmittance(angularFrequency);
			matrix[posIdx * matrixWidth + controlPosIdx] -= inductorFactor;
			matrix[posIdx * matrixWidth + controlNegIdx] += inductorFactor;
			matrix[negIdx * matrixWidth + controlPosIdx] += inductorFactor;
			matrix[negIdx * matrixWidth + controlNegIdx] -= inductorFactor;
		}
		break;
		case ElementType::CS:
		{
			CurrentSource* currentSource = static_cast<CurrentSource*>(m_controlElement);
			size_t currentSourcePosIdx = nodeIndexMap[currentSource->getPosNode()->getName()];
			size_t currentSourceNegIdx = nodeIndexMap[currentSource->getNegNode()->getName()];
			Complex supplyCurrent = currentFactor * currentSource->getSupplyCurrent();
			Complex internalAdmittanceFactor = currentFactor * currentSource->getInternalAdmittance(); //It should be factor independent
			matrix[posIdx * matrixWidth + constRow] += supplyCurrent;
			matrix[posIdx * matrixWidth + currentSourcePosIdx] += internalAdmittanceFactor;
			matrix[posIdx * matrixWidth + currentSourceNegIdx] -= internalAdmittanceFactor;
			matrix[negIdx * matrixWidth + constRow] -= supplyCurrent;
			matrix[negIdx * matrixWidth + currentSourcePosIdx] -= internalAdmittanceFactor;
			matrix[negIdx * matrixWidth + currentSourceNegIdx] += internalAdmittanceFactor;
		}
		break;
		case ElementType::VS:
		{
			VoltageSource* voltageSource = static_cast<VoltageSource*>(m_controlElement);
			size_t voltageSourceIdx = voltageIndexMap[voltageSource->getName()];
			matrix[posIdx * matrixWidth + voltageSourceIdx] -= currentFactor;
			matrix[negIdx * matrixWidth + voltageSourceIdx] += currentFactor;
		}
		break;
		case ElementType::VCVS:
		{
			VCVS* vcvs = static_cast<VCVS*>(m_controlElement);
			size_t vcvsIdx = voltageIndexMap[vcvs->getName()];
			matrix[posIdx * matrixWidth + vcvsIdx] -= currentFactor;
			matrix[negIdx * matrixWidth + vcvsIdx] += currentFactor;
		}
		break;
		case ElementType::CCVS:
		{
			CCVS* ccvs = static_cast<CCVS*>(m_controlElement);
			size_t ccvsIdx = voltageIndexMap[ccvs->getName()];
			matrix[posIdx * matrixWidth + ccvsIdx] -= currentFactor;
			matrix[negIdx * matrixWidth + ccvsIdx] += currentFactor;
		}
		break;
		case ElementType::VCCS:
		{
			VCCS* vccs = static_cast<VCCS*>(m_controlElement);
			size_t vccsPosIdx = nodeIndexMap[vccs->getPosNode()->getName()];
			size_t vccsNegIdx = nodeIndexMap[vccs->getNegNode()->getName()];
			size_t vccsControlPosIdx = nodeIndexMap[vccs->getControlPosNode()->getName()];
			size_t vccsControlNegIdx = nodeIndexMap[vccs->getControlNegNode()->getName()];
			Complex vccsFactor = currentFactor * vccs->getVoltageFactor();
			Complex vccsAdmittanceFactor = vccsFactor * vccs->getInternalAdmittance();
			matrix[posIdx * matrixWidth + vccsControlPosIdx] -= vccsFactor;
			matrix[posIdx * matrixWidth + vccsControlNegIdx] += vccsFactor;
			matrix[negIdx * matrixWidth + vccsControlPosIdx] += vccsFactor;
			matrix[negIdx * matrixWidth + vccsControlNegIdx] -= vccsFactor;
			matrix[posIdx * matrixWidth + vccsPosIdx] += vccsAdmittanceFactor;
			matrix[posIdx * matrixWidth + vccsNegIdx] -= vccsAdmittanceFactor;
			matrix[negIdx * matrixWidth + vccsPosIdx] -= vccsAdmittanceFactor;
			matrix[negIdx * matrixWidth + vccsNegIdx] += vccsAdmittanceFactor;
		}
		break;
		case ElementType::CCCS:
		{
			CCCS* cccs = static_cast<CCCS*>(m_controlElement);
			cccs->injectSupplyCurrentIntoCCCS(matrix, matrixWidth, this, currentFactor, nodeIndexMap, voltageIndexMap, angularFrequency);
		}
		break;
	}

	matrix[posIdx * matrixWidth + posIdx] += internalAdmittance;
	matrix[posIdx * matrixWidth + negIdx] -= internalAdmittance;
	matrix[negIdx * matrixWidth + posIdx] -= internalAdmittance;
	matrix[negIdx * matrixWidth + negIdx] += internalAdmittance;
}

//Matrix Operations Helpers Operations
void CCCS::injectSupplyCurrentIntoCCVS(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	size_t posIdx = nodeIndexMap[m_posNode->getName()];
	size_t negIdx = nodeIndexMap[m_negNode->getName()];
	size_t constRow = matrixWidth - 1;
	size_t voltageIdx = voltageIndexMap[ccvs->getName()];

	Complex currentFactor = totalCurrentFactor * getCurrentFactor();
	Complex internalAdmittance = getInternalAdmittance();

	switch (m_controlElement->getType())
	{
		case ElementType::Resistor:
		{
			Resistor* resistor = static_cast<Resistor*>(m_controlElement);
			size_t controlPosIdx = nodeIndexMap[resistor->getPosNode()->getName()];
			size_t controlNegIdx = nodeIndexMap[resistor->getNegNode()->getName()];
			Complex resistorFactor = currentFactor * resistor->getAdmittance();
			matrix[voltageIdx * matrixWidth + controlPosIdx] -= resistorFactor;
			matrix[voltageIdx * matrixWidth + controlNegIdx] += resistorFactor;
		}
		break;
		case ElementType::Capacitor:
		{
			Capacitor* capacitor = static_cast<Capacitor*>(m_controlElement);
			size_t controlPosIdx = nodeIndexMap[capacitor->getPosNode()->getName()];
			size_t controlNegIdx = nodeIndexMap[capacitor->getNegNode()->getName()];
			Complex capacitorFactor = currentFactor * capacitor->getAdmittance(angularFrequency);
			matrix[voltageIdx * matrixWidth + controlPosIdx] -= capacitorFactor;
			matrix[voltageIdx * matrixWidth + controlNegIdx] += capacitorFactor;
		}
		break;
		case ElementType::Inductor:
		{
			Inductor* inductor = static_cast<Inductor*>(m_controlElement);
			size_t controlPosIdx = nodeIndexMap[inductor->getPosNode()->getName()];
			size_t controlNegIdx = nodeIndexMap[inductor->getNegNode()->getName()];
			Complex inductorFactor = currentFactor * inductor->getAdmittance(angularFrequency);
			matrix[voltageIdx * matrixWidth + controlPosIdx] -= inductorFactor;
			matrix[voltageIdx * matrixWidth + controlNegIdx] += inductorFactor;
		}
		break;
		case ElementType::CS:
		{
			CurrentSource* currentSource = static_cast<CurrentSource*>(m_controlElement);
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
			VoltageSource* voltageSource = static_cast<VoltageSource*>(m_controlElement);
			size_t voltageSourceIdx = voltageIndexMap[voltageSource->getName()];
			matrix[voltageIdx * matrixWidth + voltageSourceIdx] = -currentFactor;
		}
		break;
		case ElementType::VCVS:
		{
			VCVS* vcvs = static_cast<VCVS*>(m_controlElement);
			size_t vcvsIdx = voltageIndexMap[vcvs->getName()];
			matrix[voltageIdx * matrixWidth + vcvsIdx] = -currentFactor;
		}
		break;
		case ElementType::CCVS:
		{
			CCVS* ccvs = static_cast<CCVS*>(m_controlElement);
			size_t ccvsIdx = voltageIndexMap[ccvs->getName()];
			matrix[voltageIdx * matrixWidth + ccvsIdx] = -currentFactor;
		}
		break;
		case ElementType::VCCS:
		{
			VCCS* vccs = static_cast<VCCS*>(m_controlElement);
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
			CCCS* cccs = static_cast<CCCS*>(m_controlElement);
			cccs->injectSupplyCurrentIntoCCVS(matrix, matrixWidth, ccvs, currentFactor, nodeIndexMap, voltageIndexMap, angularFrequency);
		}
		break;
	}

	Complex internalAdmittanceFactor = totalCurrentFactor * internalAdmittance;
	matrix[voltageIdx * matrixWidth + posIdx] += internalAdmittanceFactor;
	matrix[voltageIdx * matrixWidth + negIdx] -= internalAdmittanceFactor;
}

void CCCS::injectSupplyCurrentIntoCCCS(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	size_t posIdx = nodeIndexMap[m_posNode->getName()];
	size_t negIdx = nodeIndexMap[m_negNode->getName()];
	size_t cccsPosIdx = nodeIndexMap[cccs->getPosNode()->getName()];
	size_t cccsNegIdx = nodeIndexMap[cccs->getNegNode()->getName()];
	size_t constRow = matrixWidth - 1;

	Complex currentFactor = totalCurrentFactor * getCurrentFactor();
	Complex internalAdmittance = getInternalAdmittance();

	switch (m_controlElement->getType())
	{
		case ElementType::Resistor:
		{
			Resistor* resistor = static_cast<Resistor*>(m_controlElement);
			size_t controlPosIdx = nodeIndexMap[resistor->getPosNode()->getName()];
			size_t controlNegIdx = nodeIndexMap[resistor->getNegNode()->getName()];
			Complex resistorFactor = currentFactor * resistor->getAdmittance();
			matrix[cccsPosIdx * matrixWidth + controlPosIdx] -= resistorFactor;
			matrix[cccsPosIdx * matrixWidth + controlNegIdx] += resistorFactor;
			matrix[cccsNegIdx * matrixWidth + controlPosIdx] += resistorFactor;
			matrix[cccsNegIdx * matrixWidth + controlNegIdx] -= resistorFactor;
		}
		break;
		case ElementType::Capacitor:
		{
			Capacitor* capacitor = static_cast<Capacitor*>(m_controlElement);
			size_t controlPosIdx = nodeIndexMap[capacitor->getPosNode()->getName()];
			size_t controlNegIdx = nodeIndexMap[capacitor->getNegNode()->getName()];
			Complex capacitorFactor = currentFactor * capacitor->getAdmittance(angularFrequency);
			matrix[cccsPosIdx * matrixWidth + controlPosIdx] -= capacitorFactor;
			matrix[cccsPosIdx * matrixWidth + controlNegIdx] += capacitorFactor;
			matrix[cccsNegIdx * matrixWidth + controlPosIdx] += capacitorFactor;
			matrix[cccsNegIdx * matrixWidth + controlNegIdx] -= capacitorFactor;
		}
		break;
		case ElementType::Inductor:
		{
			Inductor* inductor = static_cast<Inductor*>(m_controlElement);
			size_t controlPosIdx = nodeIndexMap[inductor->getPosNode()->getName()];
			size_t controlNegIdx = nodeIndexMap[inductor->getNegNode()->getName()];
			Complex inductorFactor = currentFactor * inductor->getAdmittance(angularFrequency);
			matrix[cccsPosIdx * matrixWidth + controlPosIdx] -= inductorFactor;
			matrix[cccsPosIdx * matrixWidth + controlNegIdx] += inductorFactor;
			matrix[cccsNegIdx * matrixWidth + controlPosIdx] += inductorFactor;
			matrix[cccsNegIdx * matrixWidth + controlNegIdx] -= inductorFactor;
		}
		break;
		case ElementType::CS:
		{
			CurrentSource* currentSource = static_cast<CurrentSource*>(m_controlElement);
			size_t currentSourcePosIdx = nodeIndexMap[currentSource->getPosNode()->getName()];
			size_t currentSourceNegIdx = nodeIndexMap[currentSource->getNegNode()->getName()];
			Complex supplyCurrent = currentFactor * currentSource->getSupplyCurrent();
			Complex internalAdmittanceFactor = currentFactor * currentSource->getInternalAdmittance();
			matrix[cccsPosIdx * matrixWidth + constRow] += supplyCurrent;
			matrix[cccsNegIdx * matrixWidth + constRow] -= supplyCurrent;
			matrix[cccsPosIdx * matrixWidth + currentSourcePosIdx] += internalAdmittanceFactor;
			matrix[cccsPosIdx * matrixWidth + currentSourceNegIdx] -= internalAdmittanceFactor;
			matrix[cccsNegIdx * matrixWidth + currentSourcePosIdx] -= internalAdmittanceFactor;
			matrix[cccsNegIdx * matrixWidth + currentSourceNegIdx] += internalAdmittanceFactor;
		}
		break;
		case ElementType::VS:
		{
			VoltageSource* voltageSource = static_cast<VoltageSource*>(m_controlElement);
			size_t voltageSourceIdx = voltageIndexMap[voltageSource->getName()];
			matrix[cccsPosIdx  * matrixWidth + voltageSourceIdx] -= currentFactor;
			matrix[cccsNegIdx  * matrixWidth + voltageSourceIdx] += currentFactor;
		}
		break;
		case ElementType::VCVS:
		{
			VCVS* vcvs = static_cast<VCVS*>(m_controlElement);
			size_t vcvsIdx = voltageIndexMap[vcvs->getName()];
			matrix[cccsPosIdx  * matrixWidth + vcvsIdx] -= currentFactor;
			matrix[cccsNegIdx  * matrixWidth + vcvsIdx] += currentFactor;
		}
		break;
		case ElementType::CCVS:
		{
			CCVS* ccvs = static_cast<CCVS*>(m_controlElement);
			size_t ccvsIdx = voltageIndexMap[ccvs->getName()];
			matrix[cccsPosIdx  * matrixWidth + ccvsIdx] -= currentFactor;
			matrix[cccsNegIdx  * matrixWidth + ccvsIdx] += currentFactor;
		}
		break;
		case ElementType::VCCS:
		{
			VCCS* vccs = static_cast<VCCS*>(m_controlElement);
			size_t vccsPosIdx = nodeIndexMap[vccs->getPosNode()->getName()];
			size_t vccsNegIdx = nodeIndexMap[vccs->getNegNode()->getName()];
			size_t vccsControlPosIdx = nodeIndexMap[vccs->getControlPosNode()->getName()];
			size_t vccsControlNegIdx = nodeIndexMap[vccs->getControlNegNode()->getName()];
			Complex vccsFactor = currentFactor * vccs->getVoltageFactor();
			Complex vccsAdmittanceFactor = vccsFactor * vccs->getInternalAdmittance();
			matrix[cccsPosIdx * matrixWidth + vccsControlPosIdx] -= vccsFactor;
			matrix[cccsPosIdx * matrixWidth + vccsControlNegIdx] += vccsFactor;
			matrix[cccsNegIdx * matrixWidth + vccsControlPosIdx] += vccsFactor;
			matrix[cccsNegIdx * matrixWidth + vccsControlNegIdx] -= vccsFactor;
			matrix[cccsPosIdx * matrixWidth + vccsPosIdx] += vccsAdmittanceFactor;
			matrix[cccsPosIdx * matrixWidth + vccsNegIdx] -= vccsAdmittanceFactor;
			matrix[cccsNegIdx * matrixWidth + vccsPosIdx] -= vccsAdmittanceFactor;
			matrix[cccsNegIdx * matrixWidth + vccsNegIdx] += vccsAdmittanceFactor;
		}
		break;
		case ElementType::CCCS:
		{
			CCCS* cccs = static_cast<CCCS*>(m_controlElement);//Why this and not cccs? this->get(cccs) ?
			cccs->injectSupplyCurrentIntoCCCS(matrix, matrixWidth, this, currentFactor, nodeIndexMap, voltageIndexMap, angularFrequency);
		}
		break;
	}

	Complex internalAdmittanceFactor = totalCurrentFactor * internalAdmittance;
	matrix[posIdx * matrixWidth + posIdx] += internalAdmittanceFactor;
	matrix[posIdx * matrixWidth + negIdx] -= internalAdmittanceFactor;
	matrix[negIdx * matrixWidth + posIdx] -= internalAdmittanceFactor;
	matrix[negIdx * matrixWidth + negIdx] += internalAdmittanceFactor;
}