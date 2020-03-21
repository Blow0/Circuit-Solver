#include "node.h"

//Constructors
Node::Node(const std::string& name)
	:m_name(name)
{

}

Node::~Node()
{
}

//Computers
void Node::computeFlowCurrent()
{
	/*
	m_flowCurrent = 0;
	for (Element* var : m_elements)
	{
		m_flowCurrent += var.getCurrent() * (var.getPosNode().equals(this) ? 1.0 : -1.0);
	}
	*/
}

//Logic
bool Node::equals(const Node& rhs) const
{
	//Compare names. Be careful!!
	return rhs.m_name == m_name;
}

bool Node::operator==(const Node& rhs) const
{
	//Compare names. Be careful!!
	return rhs.m_name == m_name;
}

//Elements
bool Node::linkElement(Element* element)
{
	for (std::list<Element*>::iterator it = m_elements.begin(); it != m_elements.end(); it++)
	{
		//Found element already.
		if (element == (*it))
			return false;
	}
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
