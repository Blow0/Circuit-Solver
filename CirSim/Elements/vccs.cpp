#include "vccs.h"

#include "cccs.h"
#include "ccvs.h"

//Constructors
VCCS::VCCS(const std::string& vcvsName, Node& posNode, Node& negNode, Complex factor, const std::string& controlPosNode, const std::string& controlNegNode, Complex internalAdmittance)
	: CurrentSource(vcvsName, posNode, negNode, 0.0, internalAdmittance)
	, m_controlPosNode(controlPosNode)
	, m_controlNegNode(controlNegNode)
	, m_voltageFactor(factor)
{
	setType(ElementType::VCCS);
}

VCCS::~VCCS()
{
	m_controlPosNode = m_controlNegNode = nullptr;
}

//Static Voltage Source Creation 
VCCS* VCCS::createVCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, const std::string& controlPosNode, const std::string& controlNegNode, Complex internalAdmittance)
{
	std::string name = "vccs" + ccvsName;
	if (elementExists(name))
		return (VCCS*)elementsMap[name];

	VCCS* vccs = new VCCS(ccvsName, posNode, negNode, factor, controlPosNode, controlNegNode, internalAdmittance);
	elementsMap.emplace(name, vccs);
	return vccs;
}

//Matrix Operations
void VCCS::injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_controlPosNode) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_controlNegNode) == nodeIndexMap.end())
		throw std::runtime_error("VCCS: Couldn't find a Node.");

	size_t posIdx = nodeIndexMap[m_posNode->getName()];
	size_t negIdx = nodeIndexMap[m_negNode->getName()];
	size_t controlPosIdx = nodeIndexMap[m_controlPosNode];
	size_t controlNegIdx = nodeIndexMap[m_controlNegNode];
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

void VCCS::injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_controlPosNode) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_controlNegNode) == nodeIndexMap.end())
		throw std::runtime_error("VCCS: Couldn't find a Node.");

	if (voltageIndexMap.find(ccvs->getName()) == voltageIndexMap.end())
		throw std::runtime_error("VCCS: Couldn't find a Voltage Source.");

	size_t currentSourcePosIdx = nodeIndexMap[m_posNode->getName()];
	size_t currentSourceNegIdx = nodeIndexMap[m_negNode->getName()];
	size_t voltageControlPosIdx = nodeIndexMap[m_controlPosNode];
	size_t voltageControlNegIdx = nodeIndexMap[m_controlNegNode];
	size_t constRow = matrixWidth - 1;
	size_t voltageIdx = voltageIndexMap[ccvs->getName()];

	Complex supplyCurrentVoltageFactor = totalCurrentFactor * getVoltageFactor();
	Complex internalAdmittanceFactor = totalCurrentFactor * getInternalAdmittance();
	matrix[voltageIdx * matrixWidth + voltageControlPosIdx] -= supplyCurrentVoltageFactor;
	matrix[voltageIdx * matrixWidth + voltageControlNegIdx] += supplyCurrentVoltageFactor;
	matrix[voltageIdx * matrixWidth + currentSourcePosIdx] += internalAdmittanceFactor;
	matrix[voltageIdx * matrixWidth + currentSourceNegIdx] -= internalAdmittanceFactor;
}

void VCCS::injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_controlPosNode) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_controlNegNode) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getPosNode()->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getNegNode()->getName()) == nodeIndexMap.end())
		throw std::runtime_error("CurrentSource: Couldn't find a Node.");

	size_t currentSourcePosIdx = nodeIndexMap[m_posNode->getName()];
	size_t currentSourceNegIdx = nodeIndexMap[m_negNode->getName()];
	size_t voltageControlPosIdx = nodeIndexMap[m_controlPosNode];
	size_t voltageControlNegIdx = nodeIndexMap[m_controlNegNode];
	size_t posIdx = nodeIndexMap[cccs->getPosNode()->getName()];
	size_t negIdx = nodeIndexMap[cccs->getNegNode()->getName()];
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