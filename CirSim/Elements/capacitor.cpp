#include "capacitor.h"

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
	size_t posIdx = nodeIndexMap[m_posNode->getName()];
	size_t negIdx = nodeIndexMap[m_negNode->getName()];
	Complex admittance = getAdmittance(angularFrequency);

	matrix[posIdx * matrixWidth + posIdx] += admittance;
	matrix[posIdx * matrixWidth + negIdx] -= admittance;
	matrix[negIdx * matrixWidth + posIdx] -= admittance;
	matrix[negIdx * matrixWidth + negIdx] += admittance;
}
void Capacitor::injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	Element* controlledElement = (Element*)ccvs;
	size_t controlPosIdx = nodeIndexMap[this->m_posNode->getName()];
	size_t controlNegIdx = nodeIndexMap[this->m_negNode->getName()];
	size_t voltageIdx = voltageIndexMap[controlledElement->getName()];
	Complex admittance = this->getAdmittance(angularFrequency);
	matrix[voltageIdx * matrixWidth + controlPosIdx] -= admittance;
	matrix[voltageIdx * matrixWidth + controlNegIdx] += admittance;
}
void Capacitor::injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	Element* controlElement = (Element*)cccs;
	Capacitor* castedCCCS = static_cast<Capacitor*>(controlElement);
	size_t controlPosIdx = nodeIndexMap[this->getPosNode()->getName()];
	size_t controlNegIdx = nodeIndexMap[this->getNegNode()->getName()];
	size_t posIdx = nodeIndexMap[castedCCCS->getPosNode()->getName()];
	size_t negIdx = nodeIndexMap[castedCCCS->getPosNode()->getName()];
	Complex admittance = this->getAdmittance(angularFrequency);
	matrix[posIdx * matrixWidth + controlPosIdx] -= admittance;
	matrix[posIdx * matrixWidth + controlNegIdx] += admittance;
	matrix[negIdx * matrixWidth + controlPosIdx] += admittance;
	matrix[negIdx * matrixWidth + controlNegIdx] -= admittance;
}