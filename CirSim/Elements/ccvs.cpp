#include "ccvs.h"


//Constructors
CCVS::CCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Element* controlCurrent, Complex internalImpedance)
	: VoltageSource(ccvsName, posNode, negNode, 0.0, internalImpedance)
	, CurrentControlledSource()
	, m_currentFactor(factor)
{
	setType(ElementType::CCVS);
}

CCVS::~CCVS()
{
	m_controlCurrent = nullptr;
}

//Static Voltage Source Creation 
CCVS* CCVS::createCCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Element* controlCurrent, Complex internalImpedance)
{
	std::string name = "ccvs" + ccvsName;
	if (elementExists(name))
		return (CCVS*)elementsMap[name];

	CCVS* ccvs = new CCVS(ccvsName, posNode, negNode, factor, controlCurrent, internalImpedance);
	elementsMap.emplace(name, ccvs);
	return ccvs;
}