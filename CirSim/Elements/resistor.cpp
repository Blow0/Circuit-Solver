#include "resistor.h"

#include "cccs.h"
#include "ccvs.h"

//Constructors
Resistor::Resistor(const std::string& name, Node& posNode, Node& negNode, double resistance)
	: Element(name, ElementType::Resistor)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_resistance(check_not_zero(abs(resistance)))
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);
}

Resistor::~Resistor()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;
}

//Static Resistor Creation
Resistor* Resistor::createResistor(const std::string& resistorName, Node& posNode, Node& negNode, double resistance)
{
	std::string name = "r" + resistorName;
	if (elementExists(name))
		return (Resistor*)elementsMap[name];

	Resistor* resistor = new Resistor(resistorName, posNode, negNode, resistance);
	elementsMap.emplace(name, resistor);
	return resistor;
}

//Matrix Operations
void Resistor::injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end())
		throw std::runtime_error("Resistor: Couldn't find a Node.");

	size_t posIdx = nodeIndexMap[m_posNode->getName()];
	size_t negIdx = nodeIndexMap[m_negNode->getName()];

	Complex admittance = getAdmittance();
	matrix[posIdx * matrixWidth + posIdx] += admittance;
	matrix[posIdx * matrixWidth + negIdx] -= admittance;
	matrix[negIdx * matrixWidth + posIdx] -= admittance;
	matrix[negIdx * matrixWidth + negIdx] += admittance;
}

void Resistor::injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end())
		throw std::runtime_error("Resistor: Couldn't find a Node.");

	if (voltageIndexMap.find(ccvs->getName()) == voltageIndexMap.end())
		throw std::runtime_error("Resistor: Couldn't find a Voltage Source.");

	size_t controlPosIdx = nodeIndexMap[m_posNode->getName()];
	size_t controlNegIdx = nodeIndexMap[m_negNode->getName()];
	size_t voltageIdx = voltageIndexMap[ccvs->getName()];

	Complex resistorFactor = totalCurrentFactor * getAdmittance();
	matrix[voltageIdx * matrixWidth + controlPosIdx] -= resistorFactor;
	matrix[voltageIdx * matrixWidth + controlNegIdx] += resistorFactor;
}

void Resistor::injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getPosNode()->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getNegNode()->getName()) == nodeIndexMap.end())
		throw std::runtime_error("Resistor: Couldn't find a Node.");

	size_t controlPosIdx = nodeIndexMap[m_posNode->getName()];
	size_t controlNegIdx = nodeIndexMap[m_negNode->getName()];
	size_t posIdx = nodeIndexMap[cccs->getPosNode()->getName()];
	size_t negIdx = nodeIndexMap[cccs->getNegNode()->getName()];

	Complex resistorFactor = totalCurrentFactor * getAdmittance();
	matrix[posIdx * matrixWidth + controlPosIdx] -= resistorFactor;
	matrix[posIdx * matrixWidth + controlNegIdx] += resistorFactor;
	matrix[negIdx * matrixWidth + controlPosIdx] += resistorFactor;
	matrix[negIdx * matrixWidth + controlNegIdx] -= resistorFactor;
}