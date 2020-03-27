#include "cccs.h"

//Constructors

CCCS::CCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex currentFactor, Element* controlElement, Complex internalAdmittance)
	: CurrentSource(cccsName, posNode, negNode, 0.0, internalAdmittance)
	, m_controlElement(controlElement)
	, m_currentFactor(currentFactor)
{
	setType(ElementType::CCCS);
	m_currentControlledCount++;
}

CCCS::~CCCS()
{
	m_controlElement = nullptr;
}

//Static Voltage Source Creation 
CCCS* CCCS::createCCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex currentFactor, Element* controlElement, Complex internalAdmittance)
{
	std::string name = "cccs" + cccsName;
	if (elementExists(name))
		return (CCCS*)elementsMap[name];

	CCCS* cccs = new CCCS(cccsName, posNode, negNode, currentFactor, controlElement, internalAdmittance);
	elementsMap.emplace(name, cccs);
	return cccs;
}