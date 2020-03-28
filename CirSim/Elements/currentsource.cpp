#include "currentsource.h"

size_t m_currentControlledCount = 0;
//Constructors
CurrentSource::CurrentSource(const std::string& currentSrcName, Node& posNode, Node& negNode, Complex supplyCurrent, Complex internalAdmittance)
	: Element(currentSrcName, ElementType::CS)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_supplyCurrent(supplyCurrent)
	, m_internalAdmittance(internalAdmittance)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);
}

CurrentSource::~CurrentSource()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;
}

//Static Voltage Source Creation 
CurrentSource* CurrentSource::createCurrentSource(const std::string& currentSrcName, Node& posNode, Node& negNode, Complex supplyCurrent, Complex internalAdmittance)
{
	std::string name = "cs" + currentSrcName;
	if (elementExists(name))
		return (CurrentSource*)elementsMap[name];

	CurrentSource* currentsource = new CurrentSource(currentSrcName, posNode, negNode, supplyCurrent, internalAdmittance);
	elementsMap.emplace(name, currentsource);
	return currentsource;
}

//Matrix Operations
void CurrentSource::injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	size_t posIdx = nodeIndexMap[m_posNode->getName()];
	size_t negIdx = nodeIndexMap[m_negNode->getName()];
	size_t constRow = matrixWidth - 1;

	Complex supplyCurrent = getSupplyCurrent();
	Complex internalAdmittance = getInternalAdmittance();
	matrix[posIdx * matrixWidth + constRow] += supplyCurrent;
	matrix[negIdx * matrixWidth + constRow] -= supplyCurrent;
	matrix[posIdx * matrixWidth + posIdx] += internalAdmittance;
	matrix[posIdx * matrixWidth + negIdx] -= internalAdmittance;
	matrix[negIdx * matrixWidth + posIdx] -= internalAdmittance;
	matrix[negIdx * matrixWidth + negIdx] += internalAdmittance;
}