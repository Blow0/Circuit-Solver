#include "element.h"

//Elements Map
std::map<std::string, Element*> Element::elementsMap;

//Constructors
Element::Element(const std::string& name, ElementType type)
	: m_name(name)
	, m_type(type)
{
}

Element::~Element()
{
	elementsMap.erase(m_name);
}
void Element::InjectMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	std::string name = "defaultElement";
	Element element(name, ElementType::Resistor);
	element.injectIntoMatrix(matrix, matrixWidth, nodeIndexMap, voltageIndexMap, angularFrequency);
}
void Element::injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency)
{
	std::map<std::string, Element*> ::iterator it = elementsMap.begin();
	Element* element;
	while (it != elementsMap.end())
	{
		element = it->second;
		element->injectIntoMatrix(matrix, matrixWidth, nodeIndexMap, voltageIndexMap, angularFrequency);
		it++;
	}
}