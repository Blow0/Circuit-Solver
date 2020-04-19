#include "short.h"

//Constructors
Short::Short(const std::string& shortName, Node& posNode, Node& negNode)
	: VoltageSource(shortName, ElementType::Short, posNode, negNode, 0.0, 0.0)
{
}

Short::~Short()
{
}

//Static Voltage Source Creation 
Short* Short::createShort(const std::string& shortName, Node& posNode, Node& negNode)
{
	std::string name = elementNameWithType(shortName, ElementType::Short);
	Short* shortElement = (Short*)getElement(name);
	if (shortElement == nullptr)
		shortElement = new Short(shortName, posNode, negNode);
	return shortElement;
}
