#ifndef _NODE_H
#define _NODE_H

#include <map>
#include <list>
#include <string>
#include "../Math/complex.h"

class Element;

class Node
{
private: //Nodes Map
	static std::map<std::string, Node*> nodesMap;

private:
	std::string m_name;
	std::list<Element*> m_elements;
	Complex m_nodalVoltage;
	Complex m_flowCurrent;

public: //Static Nodes Map Methods
	static inline bool nodeExists(std::string nodeName) { return (nodesMap.find(nodeName) != nodesMap.end()); }
	static inline size_t getNodesCount() { return nodesMap.size(); }
	static inline Node* getNode(std::string nodeName) { return nodeExists(nodeName) ? nodesMap[nodeName] : nullptr; }

public: //Static Node Creation
	static Node* createNode(std::string nodeName);
	static size_t getIndex(Node* node)
	{
		size_t i = 0;
		for (std::map<std::string, Node*>::iterator it = nodesMap.begin(); it != nodesMap.end(); it++)
		{
			if (node == it->second)
				return i;
			i++;
		}
		return -1;
	}
private: //Constructors
	Node(const std::string& name);
	~Node();

public: //Computers
	void computeFlowCurrent();

public: //Logic
	inline bool operator==(const Node& rhs) const { return rhs.m_name == m_name; }

public: //Linked Elements
	bool linkElement(Element* element);
	bool unLinkElement(const Element* element);
	bool isElementLinked(const Element* element);
	inline const std::list<Element*>& getElements() const { return m_elements; }

public: //Setters
	inline void setName(const std::string& name) { nodesMap.erase(m_name); m_name = name; nodesMap.emplace(m_name, this); }

public: //Getters
	inline const std::string& getName() const { return m_name; }
	inline Complex getNodalVoltage() const { return m_nodalVoltage; }
	inline Complex getFlowCurrent() const { return m_flowCurrent; }
	
	Node(const Node&) = delete;
	void operator=(const Node&) = delete;
};

#endif //_NODE_H