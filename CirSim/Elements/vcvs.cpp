#include "vcvs.h"

#include "cccs.h"
#include "ccvs.h"

//Constructors
VCVS::VCVS(const std::string& vcvsName, Node& posNode, Node& negNode, Complex factor, const std::string& controlPosNode, const std::string& controlNegNode, Complex internalImpedance)
	: VoltageSource(vcvsName, posNode, negNode, 0.0, internalImpedance)
	, m_controlPosNode(controlPosNode)
	, m_controlNegNode(controlNegNode)
	, m_voltageFactor(factor)
{ 
	setType(ElementType::VCVS);
}

VCVS::~VCVS()
{
	m_controlPosNode = m_controlNegNode = nullptr;
}

//Static Voltage Source Creation 
VCVS* VCVS::createVCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, const std::string& controlPosNode, const std::string& controlNegNode, Complex internalImpedance)
{
	std::string name = "vcvs" + ccvsName;
	if (elementExists(name))
		return (VCVS*)elementsMap[name];

	VCVS* vcvs = new VCVS(ccvsName, posNode, negNode, factor, controlPosNode, controlNegNode, internalImpedance);
	elementsMap.emplace(name, vcvs);
	return vcvs;
}

//Matrix Operations
void VCVS::injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_controlPosNode) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_controlNegNode) == nodeIndexMap.end())
		throw std::runtime_error("VCVS: Couldn't find a Node.");

	if (voltageIndexMap.find(m_name) == voltageIndexMap.end())
		throw std::runtime_error("VCVS: Couldn't find a Voltage Source.");

	size_t posIdx = nodeIndexMap[m_posNode->getName()];
	size_t negIdx = nodeIndexMap[m_negNode->getName()];
	size_t controlPosIdx = nodeIndexMap[m_controlPosNode];
	size_t controlNegIdx = nodeIndexMap[m_controlNegNode];
	size_t constRow = matrixWidth - 1;
	size_t voltageIdx = voltageIndexMap[m_name];

	Complex voltageFactor = getVoltageFactor();
	Complex internalImpedance = getInternalImpedance();
	matrix[voltageIdx * matrixWidth + posIdx] = 1;
	matrix[voltageIdx * matrixWidth + negIdx] = -1;
	matrix[voltageIdx * matrixWidth + voltageIdx] = internalImpedance;
	matrix[voltageIdx * matrixWidth + controlPosIdx] = -voltageFactor;
	matrix[voltageIdx * matrixWidth + controlNegIdx] = voltageFactor;
	matrix[posIdx * matrixWidth + voltageIdx] = -1;
	matrix[negIdx * matrixWidth + voltageIdx] = 1;
}

void VCVS::injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	if (voltageIndexMap.find(m_name) == voltageIndexMap.end()
		|| voltageIndexMap.find(ccvs->getName()) == voltageIndexMap.end())
		throw std::runtime_error("VCVS: Couldn't find a Voltage Source.");

	size_t voltageSourceIdx = voltageIndexMap[m_name];
	size_t voltageIdx = voltageIndexMap[ccvs->getName()];

	matrix[voltageIdx * matrixWidth + voltageSourceIdx] = -totalCurrentFactor;
}

void VCVS::injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(cccs->getPosNode()->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getNegNode()->getName()) == nodeIndexMap.end())
		throw std::runtime_error("VCVS: Couldn't find a Node.");

	if (voltageIndexMap.find(m_name) == voltageIndexMap.end())
		throw std::runtime_error("VCVS: Couldn't find a Voltage Source.");

	size_t posIdx = nodeIndexMap[cccs->getPosNode()->getName()];
	size_t negIdx = nodeIndexMap[cccs->getNegNode()->getName()];
	size_t voltageSourceIdx = voltageIndexMap[m_name];

	matrix[posIdx * matrixWidth + voltageSourceIdx] -= totalCurrentFactor;
	matrix[negIdx * matrixWidth + voltageSourceIdx] += totalCurrentFactor;
}