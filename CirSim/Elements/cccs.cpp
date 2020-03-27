#include "cccs.h"

//Constructors

CCCS::CCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex factor, Element* controlCurrent, Complex internalAdmittance)
	: CurrentSource(cccsName, posNode, negNode, 0.0, internalAdmittance)
	, CurrentControlledSource()
	, m_controlCurrent(controlCurrent)
	, m_currentFactor(factor)
{
	setType(ElementType::CCCS);
}

CCCS::~CCCS()
{
	m_controlCurrent = nullptr;
}

//Static Voltage Source Creation 
CCCS* CCCS::createCCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex factor, Element* controlCurrent, Complex internalAdmittance)
{
	std::string name = "cccs" + cccsName;
	if (elementExists(name))
		return (CCCS*)elementsMap[name];

	CCCS* cccs = new CCCS(cccsName, posNode, negNode, factor, controlCurrent, internalAdmittance);
	elementsMap.emplace(name, cccs);
	return cccs;
}