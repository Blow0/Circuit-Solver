#include "currentsource.h"

#include "cccs.h"
#include "ccvs.h"

//Constructors
CurrentSource::CurrentSource(const std::string& currentSrcName, ElementType type, Node& posNode, Node& negNode, Complex supplyCurrent, Complex internalAdmittance)
	: Element(currentSrcName, type)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_supplyCurrent(supplyCurrent)
	, m_internalAdmittance(internalAdmittance)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);
}

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
	std::string name = elementNameWithType(currentSrcName, ElementType::CS);
	CurrentSource* currentsource = (CurrentSource*)getElement(name);
	if (currentsource == nullptr)
		currentsource = new CurrentSource(currentSrcName, posNode, negNode, supplyCurrent, internalAdmittance);
	return currentsource;
}

//Matrix Operations
void CurrentSource::injectIntoMatrix(Complex* matrix, size_t matrixWidth, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end())
		throw std::runtime_error("CurrentSource: Couldn't find a Node.");

	size_t posIdx = nodeIndexMap.at(m_posNode->getName());
	size_t negIdx = nodeIndexMap.at(m_negNode->getName());
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

void CurrentSource::injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end())
		throw std::runtime_error("CurrentSource: Couldn't find a Node.");

	if (voltageIndexMap.find(ccvs->getName()) == voltageIndexMap.end())
		throw std::runtime_error("CurrentSource: Couldn't find a Voltage Source.");

	size_t currentSourcePosIdx = nodeIndexMap.at(m_posNode->getName());
	size_t currentSourceNegIdx = nodeIndexMap.at(m_negNode->getName());
	size_t constRow = matrixWidth - 1;
	size_t voltageIdx = voltageIndexMap.at(ccvs->getName());

	Complex supplyCurrentFactor = totalCurrentFactor * getSupplyCurrent();
	Complex internalAdmittanceFactor = totalCurrentFactor * getInternalAdmittance();
	matrix[voltageIdx * matrixWidth + constRow] = supplyCurrentFactor;
	matrix[voltageIdx * matrixWidth + currentSourcePosIdx] += internalAdmittanceFactor;
	matrix[voltageIdx * matrixWidth + currentSourceNegIdx] -= internalAdmittanceFactor;
}

void CurrentSource::injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	if (nodeIndexMap.find(m_posNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(m_negNode->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getPosNode()->getName()) == nodeIndexMap.end()
		|| nodeIndexMap.find(cccs->getNegNode()->getName()) == nodeIndexMap.end())
		throw std::runtime_error("CurrentSource: Couldn't find a Node.");

	size_t currentSourcePosIdx = nodeIndexMap.at(m_posNode->getName());
	size_t currentSourceNegIdx = nodeIndexMap.at(m_negNode->getName());
	size_t posIdx = nodeIndexMap.at(cccs->getPosNode()->getName());
	size_t negIdx = nodeIndexMap.at(cccs->getNegNode()->getName());
	size_t constRow = matrixWidth - 1;

	Complex supplyCurrentFactor = totalCurrentFactor * getSupplyCurrent();
	Complex internalAdmittanceFactor = totalCurrentFactor * getInternalAdmittance();
	matrix[posIdx * matrixWidth + constRow] += supplyCurrentFactor;
	matrix[posIdx * matrixWidth + currentSourcePosIdx] += internalAdmittanceFactor;
	matrix[posIdx * matrixWidth + currentSourceNegIdx] -= internalAdmittanceFactor;
	matrix[negIdx * matrixWidth + constRow] -= supplyCurrentFactor;
	matrix[negIdx * matrixWidth + currentSourcePosIdx] -= internalAdmittanceFactor;
	matrix[negIdx * matrixWidth + currentSourceNegIdx] += internalAdmittanceFactor;
}