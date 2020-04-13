#include "vccs.h"

#include "cccs.h"
#include "ccvs.h"

//Constructors
VCCS::VCCS(const std::string& vcvsName, Node& posNode, Node& negNode, Complex factor, const std::string& controlPosNodeName, const std::string& controlNegNodeName, Complex internalAdmittance)
	: CurrentSource(vcvsName, ElementType::VCCS, posNode, negNode, 0.0, internalAdmittance)
	, m_controlPosNodeName(controlPosNodeName)
	, m_controlNegNodeName(controlNegNodeName)
	, m_voltageFactor(factor)
{
}

VCCS::~VCCS()
{
}

//Static Voltage Source Creation 
VCCS* VCCS::createVCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, const std::string& controlPosNodeName, const std::string& controlNegNodeName, Complex internalAdmittance)
{
	std::string name = elementNameWithType(ccvsName, ElementType::VCCS);
	VCCS* vccs = (VCCS*)getElement(name);
	if (vccs == nullptr)
		vccs = new VCCS(ccvsName, posNode, negNode, factor, controlPosNodeName, controlNegNodeName, internalAdmittance);
	return vccs;
}

//Matrix Operations
void VCCS::injectIntoMatrix(Complex* matrix, size_t matrixWidth, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_controlPosNodeName) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_controlNegNodeName) == nodeIndexMap.end())
		throw std::runtime_error("VCCS: Couldn't find a Node.");

	size_t posIdx = nodeIndexMap.at(m_posNode->getName());
	size_t negIdx = nodeIndexMap.at(m_negNode->getName());
	size_t controlPosIdx = nodeIndexMap.at(m_controlPosNodeName);
	size_t controlNegIdx = nodeIndexMap.at(m_controlNegNodeName);
	size_t constRow = matrixWidth - 1;

	Complex voltageFactor = getVoltageFactor();
	Complex internalAdmittance = getInternalAdmittance();
	matrix[posIdx * matrixWidth + controlPosIdx] -= voltageFactor;
	matrix[posIdx * matrixWidth + controlNegIdx] += voltageFactor;
	matrix[negIdx * matrixWidth + controlPosIdx] += voltageFactor;
	matrix[negIdx * matrixWidth + controlNegIdx] -= voltageFactor;
	matrix[posIdx * matrixWidth + posIdx] += internalAdmittance;
	matrix[posIdx * matrixWidth + negIdx] -= internalAdmittance;
	matrix[negIdx * matrixWidth + posIdx] -= internalAdmittance;
	matrix[negIdx * matrixWidth + negIdx] += internalAdmittance;
}

void VCCS::injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_controlPosNodeName) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_controlNegNodeName) == nodeIndexMap.end())
		throw std::runtime_error("VCCS: Couldn't find a Node.");

	if (voltageIndexMap.find(ccvs->getName()) == voltageIndexMap.end())
		throw std::runtime_error("VCCS: Couldn't find a Voltage Source.");

	size_t currentSourcePosIdx = nodeIndexMap.at(m_posNode->getName());
	size_t currentSourceNegIdx = nodeIndexMap.at(m_negNode->getName());
	size_t voltageControlPosIdx = nodeIndexMap.at(m_controlPosNodeName);
	size_t voltageControlNegIdx = nodeIndexMap.at(m_controlNegNodeName);
	size_t constRow = matrixWidth - 1;
	size_t voltageIdx = voltageIndexMap.at(ccvs->getName());

	Complex supplyCurrentVoltageFactor = totalCurrentFactor * getVoltageFactor();
	Complex internalAdmittanceFactor = totalCurrentFactor * getInternalAdmittance();
	matrix[voltageIdx * matrixWidth + voltageControlPosIdx] -= supplyCurrentVoltageFactor;
	matrix[voltageIdx * matrixWidth + voltageControlNegIdx] += supplyCurrentVoltageFactor;
	matrix[voltageIdx * matrixWidth + currentSourcePosIdx] += internalAdmittanceFactor;
	matrix[voltageIdx * matrixWidth + currentSourceNegIdx] -= internalAdmittanceFactor;
}

void VCCS::injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_controlPosNodeName) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_controlNegNodeName) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getPosNode()->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getNegNode()->getName()) == nodeIndexMap.end())
		throw std::runtime_error("CurrentSource: Couldn't find a Node.");

	size_t currentSourcePosIdx = nodeIndexMap.at(m_posNode->getName());
	size_t currentSourceNegIdx = nodeIndexMap.at(m_negNode->getName());
	size_t voltageControlPosIdx = nodeIndexMap.at(m_controlPosNodeName);
	size_t voltageControlNegIdx = nodeIndexMap.at(m_controlNegNodeName);
	size_t posIdx = nodeIndexMap.at(cccs->getPosNode()->getName());
	size_t negIdx = nodeIndexMap.at(cccs->getNegNode()->getName());
	size_t constRow = matrixWidth - 1;

	Complex supplyCurrentVoltageFactor = totalCurrentFactor * getVoltageFactor();
	Complex internalAdmittanceFactor = totalCurrentFactor * getInternalAdmittance();
	matrix[posIdx * matrixWidth + voltageControlPosIdx] -= supplyCurrentVoltageFactor;
	matrix[posIdx * matrixWidth + voltageControlNegIdx] += supplyCurrentVoltageFactor;
	matrix[negIdx * matrixWidth + voltageControlPosIdx] += supplyCurrentVoltageFactor;
	matrix[negIdx * matrixWidth + voltageControlNegIdx] -= supplyCurrentVoltageFactor;
	matrix[posIdx * matrixWidth + currentSourcePosIdx] += internalAdmittanceFactor;
	matrix[posIdx * matrixWidth + currentSourceNegIdx] -= internalAdmittanceFactor;
	matrix[negIdx * matrixWidth + currentSourcePosIdx] -= internalAdmittanceFactor;
	matrix[negIdx * matrixWidth + currentSourceNegIdx] += internalAdmittanceFactor;
}