#include "inductor.h"

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
	size_t posIdx = nodeIndexMap[m_posNode->getName()];
	size_t negIdx = nodeIndexMap[m_negNode->getName()];
	Complex admittance = getAdmittance(angularFrequency);

	matrix[posIdx * matrixWidth + posIdx] += admittance;
	matrix[posIdx * matrixWidth + negIdx] -= admittance;
	matrix[negIdx * matrixWidth + posIdx] -= admittance;
	matrix[negIdx * matrixWidth + negIdx] += admittance;
}
void Inductor::injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	Element* controlledElement = (Element*)ccvs;
	size_t controlPosIdx = nodeIndexMap[this->m_posNode->getName()];
	size_t controlNegIdx = nodeIndexMap[this->m_negNode->getName()];
	size_t voltageIdx = voltageIndexMap[controlledElement->getName()];
	Complex admittance = this->getAdmittance(angularFrequency);
	matrix[voltageIdx * matrixWidth + controlPosIdx] -= admittance;
	matrix[voltageIdx * matrixWidth + controlNegIdx] += admittance;
}
void Inductor::injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency)
{
	Element* controlElement = (Element*)cccs;
	Inductor* castedCCCS = static_cast<Inductor*>(controlElement);
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
