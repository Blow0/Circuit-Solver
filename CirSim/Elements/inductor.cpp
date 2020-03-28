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