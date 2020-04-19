#include "node.h"
#include "../Elements/element.h"
#include "../Elements/resistor.h"
#include "../Elements/capacitor.h"
#include "../Elements/inductor.h"
#include "../Elements/currentsource.h"
#include "../Elements/voltagesource.h"
#include "../Elements/vcvs.h"
#include "../Elements/vccs.h"
#include "../Elements/cccs.h"
#include "../Elements/ccvs.h"

//Nodes Map
std::map<std::string, Node*> Node::nodesMap;

//Constructors
Node::Node(const std::string& nodeName)
	: m_name(nodeName)
	, m_nodalVoltage(0.0)
{
}

Node::~Node()
{
	//TODO: Remove node from elements
	nodesMap.erase(m_name);
}

Complex Node::getFlowCurrent(double angularFrequency) const
{
	Complex flowCurrent = 0.0;
	for (std::list<Element*>::const_iterator it = m_elements.begin(); it != m_elements.end(); it++)
	{
		Element* element = (*it);
		switch (element->getType())
		{
		case ElementType::Resistor:
			flowCurrent += dynamic_cast<Resistor*>(element)->getCurrent().getComponentAbs();
			break;
		case ElementType::Inductor:
			flowCurrent += dynamic_cast<Inductor*>(element)->getCurrent(angularFrequency).getComponentAbs();
			break;
		case ElementType::Capacitor:
			flowCurrent += dynamic_cast<Capacitor*>(element)->getCurrent(angularFrequency).getComponentAbs();
			break;
		case ElementType::CS:
			flowCurrent += ((CurrentSource*)element)->getSupplyCurrent().getComponentAbs();
			break;
		case ElementType::VS:
			flowCurrent += ((VoltageSource*)element)->getCurrent().getComponentAbs();
			break;
		case ElementType::CCCS:
			flowCurrent += dynamic_cast<CCCS*>(element)->getSupplyCurrent(angularFrequency).getComponentAbs();
			break;
		case ElementType::CCVS:
			flowCurrent += dynamic_cast<CCVS*>(element)->getCurrent().getComponentAbs();
			break;
		case ElementType::VCCS:
			flowCurrent += dynamic_cast<VCCS*>(element)->getSupplyCurrent().getComponentAbs();
			break;
		case ElementType::VCVS:
			flowCurrent += dynamic_cast<VCVS*>(element)->getCurrent().getComponentAbs();
			break;
		}
	}
	return flowCurrent / 2.0;
}

//Elements
bool Node::linkElement(Element* element)
{
	if (Node::isElementLinked(element))
		return false;
	//Didn't find element. Add it.
	m_elements.push_back(element);
	return true;
}

bool Node::unLinkElement(const Element* element)
{
	for (std::list<Element*>::iterator it = m_elements.begin(); it != m_elements.end(); it++)
	{
		if (element->getName() == (*it)->getName())
		{
			//Found element. Remove it.
			m_elements.erase(it);
			return true;
		}
	}
	//Didn't find element.
	return false;
}

bool Node::isElementLinked(const Element* element)
{
	for (std::list<Element*>::iterator it = m_elements.begin(); it != m_elements.end(); it++)
	{
		if (element->getName() == (*it)->getName())
		{
			//Found element.
			return true;
		}
	}
	//Didn't find element.
	return false;
}

//Static Node Creation
Node* Node::createNode(const std::string& nodeName)
{
	if (nodeExists(nodeName))
		return nodesMap[nodeName];

	Node* node = new Node(nodeName);
	nodesMap.emplace(nodeName, node);
	return node;
}

void Node::deleteAllNodes()
{
	std::map<std::string, Node*>::iterator temp;
	for (std::map<std::string, Node*>::iterator it = nodesMap.begin(); it != nodesMap.end(); temp = it++)
		delete temp->second;
}

void Node::fillAllNodesFromVector(const Complex* vector, const std::map<std::string, size_t>& nodeIndexMap)
{
	for (std::map<std::string, Node*>::iterator it = nodesMap.begin(); it != nodesMap.end(); it++)
		it->second->fillFromVector(vector, nodeIndexMap);
}