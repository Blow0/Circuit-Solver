#include "inductor.h"

#include "cccs.h"
#include "ccvs.h"

//Constructors
Inductor::Inductor(const std::string& inductorName, Node& posNode, Node& negNode, double inductance)
	: Element(inductorName, ElementType::Inductor)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_inductance(abs(inductance))
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);
}

Inductor::~Inductor()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;
}

//Static Resistor Creation
Inductor* Inductor::createInductor(const std::string& inductorName, Node& posNode, Node& negNode, double inductance)
{
	std::string name = "l" + inductorName;
	if (elementExists(name))
		return (Inductor*)elementsMap[name];

	Inductor* inductor = new Inductor(inductorName, posNode, negNode, inductance);
	elementsMap.emplace(name, inductor);
	return inductor;
}

//Matrix Operations
void Inductor::injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end())
		throw std::runtime_error("Inductor: Couldn't find a Node.");

	size_t posIdx = nodeIndexMap[m_posNode->getName()];
	size_t negIdx = nodeIndexMap[m_negNode->getName()];

	Complex admittance = getAdmittance(angularFrequency);
	matrix[posIdx * matrixWidth + posIdx] += admittance;
	matrix[posIdx * matrixWidth + negIdx] -= admittance;
	matrix[negIdx * matrixWidth + posIdx] -= admittance;
	matrix[negIdx * matrixWidth + negIdx] += admittance;
}

void Inductor::injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end())
		throw std::runtime_error("Inductor: Couldn't find a Node.");

	if (voltageIndexMap.find(ccvs->getName()) == voltageIndexMap.end())
		throw std::runtime_error("Inductor: Couldn't find a Voltage Source.");

	size_t controlPosIdx = nodeIndexMap[m_posNode->getName()];
	size_t controlNegIdx = nodeIndexMap[m_negNode->getName()];
	size_t voltageIdx = voltageIndexMap[ccvs->getName()];

	Complex inductorFactor = totalCurrentFactor * getAdmittance(angularFrequency);
	matrix[voltageIdx * matrixWidth + controlPosIdx] -= inductorFactor;
	matrix[voltageIdx * matrixWidth + controlNegIdx] += inductorFactor;
}

void Inductor::injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getPosNode()->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getNegNode()->getName()) == nodeIndexMap.end())
		throw std::runtime_error("Inductor: Couldn't find a Node.");

	size_t controlPosIdx = nodeIndexMap[m_posNode->getName()];
	size_t controlNegIdx = nodeIndexMap[m_negNode->getName()];
	size_t posIdx = nodeIndexMap[cccs->getPosNode()->getName()];
	size_t negIdx = nodeIndexMap[cccs->getNegNode()->getName()];

	Complex inductorFactor = totalCurrentFactor * getAdmittance(angularFrequency);
	matrix[posIdx * matrixWidth + controlPosIdx] -= inductorFactor;
	matrix[posIdx * matrixWidth + controlNegIdx] += inductorFactor;
	matrix[negIdx * matrixWidth + controlPosIdx] += inductorFactor;
	matrix[negIdx * matrixWidth + controlNegIdx] -= inductorFactor;
}
