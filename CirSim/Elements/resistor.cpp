#include "resistor.h"

#include "cccs.h"
#include "ccvs.h"

//Constructors
Resistor::Resistor(const std::string& name, Node& posNode, Node& negNode, double resistance)
	: Element(name, ElementType::Resistor)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_resistance(abs(resistance))
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
	std::string name = elementNameWithType(resistorName, ElementType::Resistor);
	Resistor* resistor = (Resistor*)getElement(name);
	if (resistor == nullptr)
		resistor = new Resistor(resistorName, posNode, negNode, resistance);
	return resistor;
}

//Matrix Operations
void Resistor::injectIntoMatrix(Complex* matrix, size_t matrixWidth, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end())
		throw std::runtime_error("Resistor: Couldn't find a Node.");

	size_t posIdx = nodeIndexMap.at(m_posNode->getName());
	size_t negIdx = nodeIndexMap.at(m_negNode->getName());

	Complex admittance = getAdmittance();
	matrix[posIdx * matrixWidth + posIdx] += admittance;
	matrix[posIdx * matrixWidth + negIdx] -= admittance;
	matrix[negIdx * matrixWidth + posIdx] -= admittance;
	matrix[negIdx * matrixWidth + negIdx] += admittance;
}

void Resistor::injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end())
		throw std::runtime_error("Resistor: Couldn't find a Node.");

	if (voltageIndexMap.find(ccvs->getName()) == voltageIndexMap.end())
		throw std::runtime_error("Resistor: Couldn't find a Voltage Source.");

	size_t controlPosIdx = nodeIndexMap.at(m_posNode->getName());
	size_t controlNegIdx = nodeIndexMap.at(m_negNode->getName());
	size_t voltageIdx = voltageIndexMap.at(ccvs->getName());

	Complex resistorFactor = totalCurrentFactor * getAdmittance();
	matrix[voltageIdx * matrixWidth + controlPosIdx] -= resistorFactor;
	matrix[voltageIdx * matrixWidth + controlNegIdx] += resistorFactor;
}

void Resistor::injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getPosNode()->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getNegNode()->getName()) == nodeIndexMap.end())
		throw std::runtime_error("Resistor: Couldn't find a Node.");

	size_t controlPosIdx = nodeIndexMap.at(m_posNode->getName());
	size_t controlNegIdx = nodeIndexMap.at(m_negNode->getName());
	size_t posIdx = nodeIndexMap.at(cccs->getPosNode()->getName());
	size_t negIdx = nodeIndexMap.at(cccs->getNegNode()->getName());

	Complex resistorFactor = totalCurrentFactor * getAdmittance();
	matrix[posIdx * matrixWidth + controlPosIdx] -= resistorFactor;
	matrix[posIdx * matrixWidth + controlNegIdx] += resistorFactor;
	matrix[negIdx * matrixWidth + controlPosIdx] += resistorFactor;
	matrix[negIdx * matrixWidth + controlNegIdx] -= resistorFactor;
}