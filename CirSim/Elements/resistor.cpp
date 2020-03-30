#include "resistor.h"

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
	size_t posIdx = nodeIndexMap[m_posNode->getName()];
	size_t negIdx = nodeIndexMap[m_negNode->getName()];
	Complex admittance = getAdmittance();

	matrix[posIdx * matrixWidth + posIdx] += admittance;
	matrix[posIdx * matrixWidth + negIdx] -= admittance;
	matrix[negIdx * matrixWidth + posIdx] -= admittance;
	matrix[negIdx * matrixWidth + negIdx] += admittance;
}

void Resistor::injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	Element* controlledElement = (Element*)ccvs;
	size_t controlPosIdx = nodeIndexMap[this->m_posNode->getName()];
	size_t controlNegIdx = nodeIndexMap[this->m_negNode->getName()];
	size_t voltageIdx	 = voltageIndexMap[controlledElement->getName()];

	Complex admittance = this->getAdmittance();
	matrix[voltageIdx * matrixWidth + controlPosIdx] -= admittance;
	matrix[voltageIdx * matrixWidth + controlNegIdx] += admittance;
}

void Resistor::injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	Element* controlledElement = (Element*)cccs;
	Resistor* castedCCCS = static_cast<Resistor*>(controlledElement);
	size_t controlPosIdx = nodeIndexMap[this->getPosNode()->getName()];
	size_t controlNegIdx = nodeIndexMap[this->getNegNode()->getName()];
	size_t posIdx = nodeIndexMap[castedCCCS->getPosNode()->getName()];
	size_t negIdx = nodeIndexMap[castedCCCS->getPosNode()->getName()];
	Complex admittance = this->getAdmittance();
	matrix[posIdx * matrixWidth + controlPosIdx] -= admittance;
	matrix[posIdx * matrixWidth + controlNegIdx] += admittance;
	matrix[negIdx * matrixWidth + controlPosIdx] += admittance;
	matrix[negIdx * matrixWidth + controlNegIdx] -= admittance;
}