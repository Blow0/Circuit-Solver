#include "voltagesource.h"

//Static Member declaration
std::list <VoltageSource*> VoltageSource::voltageSources;
//Constructors
VoltageSource::VoltageSource(const std::string& voltageSrcName, Node& posNode, Node& negNode, Complex supplyVoltage, Complex internalImpedance)
	: Element(voltageSrcName, ElementType::VS)
	, m_posNode(&posNode)
	, m_negNode(&negNode)
	, m_supplyVoltage(supplyVoltage)
	, m_current(0.0, 0.0)
{
	m_posNode->linkElement(this);
	m_negNode->linkElement(this);
}

VoltageSource::~VoltageSource()
{
	m_posNode->unLinkElement(this);
	m_negNode->unLinkElement(this);
	m_posNode = m_negNode = nullptr;

	for (std::list<VoltageSource*>::iterator it = voltageSources.begin(); it != voltageSources.end(); it++)
	{
		if (this == *it)
		{
			voltageSources.erase(it);
			break;
		}
	}
}

//Static Voltage Source Creation 
VoltageSource* VoltageSource::createVoltageSource(const std::string& voltageSrcName, Node& posNode, Node& negNode, Complex supplyVoltage, Complex internalImpedance)
{
	std::string name = "vs" + voltageSrcName;
	if (elementExists(name))
		return (VoltageSource*)elementsMap[name];

	VoltageSource* voltagesource = new VoltageSource(voltageSrcName, posNode, negNode, supplyVoltage, internalImpedance);
	elementsMap.emplace(name, voltagesource);
	voltageSources.push_back(voltagesource);
	return voltagesource;
}

//Matrix Operations
void VoltageSource::injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	size_t posIdx = nodeIndexMap[m_posNode->getName()];
	size_t negIdx = nodeIndexMap[m_negNode->getName()];
	size_t constRow = matrixWidth - 1;
	size_t voltageIdx = voltageIndexMap[m_name];

	Complex supplyVoltage = getSupplyVoltage();
	Complex internalImpedance = getInternalImpedance();
	matrix[voltageIdx * matrixWidth + posIdx] = 1;
	matrix[voltageIdx * matrixWidth + negIdx] = -1;
	matrix[voltageIdx * matrixWidth + voltageIdx] = internalImpedance;
	matrix[voltageIdx * matrixWidth + constRow] = supplyVoltage;
	matrix[posIdx * matrixWidth + voltageIdx] = -1;
	matrix[negIdx * matrixWidth + voltageIdx] = 1;
}
void VoltageSource::injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency = 0.0)
{
	Element* controlledElement = (Element*)ccvs;
	size_t voltageIdx = voltageIndexMap[controlledElement->getName()];
	size_t voltageSourceIdx = voltageIndexMap[this->getName()];
	matrix[voltageIdx * matrixWidth + voltageSourceIdx] = -1;
}
void VoltageSource::injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency = 0.0)
{
	Element* controlElement = (Element*)cccs;
	VoltageSource* castedCCCS = static_cast<VoltageSource*>(controlElement);
	size_t posIdx = nodeIndexMap[castedCCCS->getPosNode()->getName()];
	size_t negIdx = nodeIndexMap[castedCCCS->getPosNode()->getName()];
	size_t voltageSourceIdx = voltageIndexMap[this->getName()];
	matrix[posIdx * matrixWidth + voltageSourceIdx] -= 1;
	matrix[negIdx * matrixWidth + voltageSourceIdx] += 1;
}