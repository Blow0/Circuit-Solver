#include "ccvs.h"


//Constructors
CCVS::CCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex currentFactor, Element* controlElement, Complex internalImpedance)
	: VoltageSource(ccvsName, posNode, negNode, 0.0, internalImpedance)
	, m_controlElement(controlElement)
	, m_currentFactor(currentFactor)
{
	setType(ElementType::CCVS);
}

CCVS::~CCVS()
{
	m_controlElement = nullptr;
}

//Static Voltage Source Creation 
CCVS* CCVS::createCCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex currentFactor, Element* controlCurrent, Complex internalImpedance)
{
	std::string name = "ccvs" + ccvsName;
	if (elementExists(name))
		return (CCVS*)elementsMap[name];

	CCVS* ccvs = new CCVS(ccvsName, posNode, negNode, currentFactor, controlCurrent, internalImpedance);
	elementsMap.emplace(name, ccvs);
	return ccvs;
}