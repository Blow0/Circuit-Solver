#ifndef _NODE_H
#define _NODE_H

#include <list>
#include <string>

typedef long double Complex;
class Element;

class Node
{
private:
	std::string m_name;
	std::list<Element*> m_elements;
	Complex m_nodalVoltage;
	Complex m_flowCurrent;

public: //Constructors
	Node(const std::string& name);
	~Node();

public: //Computers
	void computeFlowCurrent();

public: //Logic
	inline bool equals(const Node& rhs) const;
	inline bool operator==(const Node& rhs) const;

public: //Linked Elements
	bool linkElement(Element* element);
	bool unLinkElement(const Element* element);
	bool isElementLinked(const Element* element);
	inline const std::list<Element*>& getElements() const { return m_elements; }

public: //Setters
	inline void setName(const std::string& name) { m_name = name; }

public: //Getters
	inline const std::string& getName() const { return m_name; }
	inline Complex getNodalVoltage() const { return m_nodalVoltage; }
	inline Complex getFlowCurrent() const { return m_flowCurrent; }

	Node(const Node&) = delete;
	void operator=(const Node&) = delete;
};

#endif //_NODE_H