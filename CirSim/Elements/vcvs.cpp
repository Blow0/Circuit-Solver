#include "vcvs.h"

#include "cccs.h"
#include "ccvs.h"

//Constructors
VCVS::VCVS(const std::string& vcvsName, Node& posNode, Node& negNode, Complex factor, const std::string& controlPosNodeName, const std::string& controlNegNodeName, Complex internalImpedance)
	: VoltageSource(vcvsName, ElementType::VCVS, posNode, negNode, 0.0, internalImpedance)
	, m_controlPosNodeName(controlPosNodeName)
	, m_controlNegNodeName(controlNegNodeName)
	, m_voltageFactor(factor)
{ 
}

VCVS::~VCVS()
{
}

//Static Voltage Source Creation 
VCVS* VCVS::createVCVS(const std::string& vcvsName, Node& posNode, Node& negNode, Complex factor, const std::string& controlPosNodeName, const std::string& controlNegNodeName, Complex internalImpedance)
{
	std::string name = elementNameWithType(vcvsName, ElementType::VCVS);
	VCVS* vcvs = (VCVS*)getElement(name);
	if (vcvs == nullptr)
		vcvs = new VCVS(vcvsName, posNode, negNode, factor, controlPosNodeName, controlNegNodeName, internalImpedance);
	return vcvs;
}

//Matrix Operations
void VCVS::injectIntoMatrix(Complex* matrix, size_t matrixWidth, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_controlPosNodeName) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_controlNegNodeName) == nodeIndexMap.end())
		throw std::runtime_error("VCVS: Couldn't find a Node.");

	if (voltageIndexMap.find(getName()) == voltageIndexMap.end())
		throw std::runtime_error("VCVS: Couldn't find a Voltage Source.");

	size_t posIdx = nodeIndexMap.at(m_posNode->getName());
	size_t negIdx = nodeIndexMap.at(m_negNode->getName());
	size_t controlPosIdx = nodeIndexMap.at(m_controlPosNodeName);
	size_t controlNegIdx = nodeIndexMap.at(m_controlNegNodeName);
	size_t constRow = matrixWidth - 1;
	size_t voltageIdx = voltageIndexMap.at(getName());

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

void VCVS::injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (voltageIndexMap.find(getName()) == voltageIndexMap.end()
		|| voltageIndexMap.find(ccvs->getName()) == voltageIndexMap.end())
		throw std::runtime_error("VCVS: Couldn't find a Voltage Source.");

	size_t voltageSourceIdx = voltageIndexMap.at(getName());
	size_t voltageIdx = voltageIndexMap.at(ccvs->getName());

	matrix[voltageIdx * matrixWidth + voltageSourceIdx] = -totalCurrentFactor;
}

void VCVS::injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(cccs->getPosNode()->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getNegNode()->getName()) == nodeIndexMap.end())
		throw std::runtime_error("VCVS: Couldn't find a Node.");

	if (voltageIndexMap.find(getName()) == voltageIndexMap.end())
		throw std::runtime_error("VCVS: Couldn't find a Voltage Source.");

	size_t posIdx = nodeIndexMap.at(cccs->getPosNode()->getName());
	size_t negIdx = nodeIndexMap.at(cccs->getNegNode()->getName());
	size_t voltageSourceIdx = voltageIndexMap.at(getName());

	matrix[posIdx * matrixWidth + voltageSourceIdx] -= totalCurrentFactor;
	matrix[negIdx * matrixWidth + voltageSourceIdx] += totalCurrentFactor;
}