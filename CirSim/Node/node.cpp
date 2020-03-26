#include "node.h"
#include "../Elements/element.h"

//Nodes Map
std::map<std::string, Node*> Node::nodesMap;

//Constructors
Node::Node(const std::string& name)
	: m_name(name)
	, m_nodalVoltage(0.0)
	, m_flowCurrent(0.0)
{
}

Node::~Node()
{
	//Remove node from elements
	nodesMap.erase(m_name);
}

void Node::computeFlowCurrent()
{
	/*m_flowCurrent = 0.0;
	for (Element* element : m_elements)
	{
		m_flowCurrent = m_flowCurrent + element->getCurrent();
	}*/
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
		if (element == (*it))
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
		if (element == (*it))
		{
			//Found element.
			return true;
		}
	}
	//Didn't find element.
	return false;
}

//Static Node Creation
Node* Node::createNode(std::string nodeName)
{
	if (nodeExists(nodeName))
		return nodesMap[nodeName];

	Node* node = new Node(nodeName);
	nodesMap.emplace(nodeName, node);
	return node;
}