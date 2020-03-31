#include "capacitor.h"

#include "cccs.h"
#include "ccvs.h"

//Constructors
Capacitor::Capacitor(const std::string& capacitorName, Node& posNode, Node& negNode, double capacitance)
	: Element(capacitorName, ElementType::Capacitor)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_capacitance(abs(capacitance))
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);
}

Capacitor::~Capacitor()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;
}

//Static Resistor Creation
Capacitor* Capacitor::createCapacitor(const std::string& capacitorName, Node& posNode, Node& negNode, double capacitance)
{
	std::string name = "c" + capacitorName;
	if (elementExists(name))
		return (Capacitor*)elementsMap[name];

	Capacitor* capacitor = new Capacitor(capacitorName, posNode, negNode, capacitance);
	elementsMap.emplace(name, capacitor);
	return capacitor;
}

//Matrix Operations
void Capacitor::injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if(nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end())
		throw std::runtime_error("Capacitor: Couldn't find a Node.");

	size_t posIdx = nodeIndexMap[m_posNode->getName()];
	size_t negIdx = nodeIndexMap[m_negNode->getName()];

	Complex admittance = getAdmittance(angularFrequency);
	matrix[posIdx * matrixWidth + posIdx] += admittance;
	matrix[posIdx * matrixWidth + negIdx] -= admittance;
	matrix[negIdx * matrixWidth + posIdx] -= admittance;
	matrix[negIdx * matrixWidth + negIdx] += admittance;
}

void Capacitor::injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end())
		throw std::runtime_error("Capacitor: Couldn't find a Node.");

	if (voltageIndexMap.find(ccvs->getName()) == voltageIndexMap.end())
		throw std::runtime_error("Capacitor: Couldn't find a Voltage Source.");

	size_t controlPosIdx = nodeIndexMap[m_posNode->getName()];
	size_t controlNegIdx = nodeIndexMap[m_negNode->getName()];
	size_t voltageIdx = voltageIndexMap[ccvs->getName()];

	Complex resistorFactor = totalCurrentFactor * getAdmittance(angularFrequency);
	matrix[voltageIdx * matrixWidth + controlPosIdx] -= resistorFactor;
	matrix[voltageIdx * matrixWidth + controlNegIdx] += resistorFactor;
}

void Capacitor::injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getPosNode()->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getNegNode()->getName()) == nodeIndexMap.end())
		throw std::runtime_error("Capacitor: Couldn't find a Node.");

	size_t controlPosIdx = nodeIndexMap[m_posNode->getName()];
	size_t controlNegIdx = nodeIndexMap[m_negNode->getName()];
	size_t posIdx = nodeIndexMap[cccs->getPosNode()->getName()];
	size_t negIdx = nodeIndexMap[cccs->getNegNode()->getName()];

	Complex capacitorFactor = totalCurrentFactor * getAdmittance(angularFrequency);
	matrix[posIdx * matrixWidth + controlPosIdx] -= capacitorFactor;
	matrix[posIdx * matrixWidth + controlNegIdx] += capacitorFactor;
	matrix[negIdx * matrixWidth + controlPosIdx] += capacitorFactor;
	matrix[negIdx * matrixWidth + controlNegIdx] -= capacitorFactor;
}