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

public: //Static Nodes Map Methods
	static inline bool nodeExists(const std::string& nodeName) { return (nodesMap.find(nodeName) != nodesMap.end()); }
	static inline Node* getNode(const std::string& nodeName) { return nodeExists(nodeName) ? nodesMap[nodeName] : nullptr; }
	static inline size_t getNodesCount() { return nodesMap.size(); }
	static const std::map<std::string, Node*> getNodesMap() { return nodesMap; }
	static inline void clearNodes() { nodesMap.clear(); }
	static void deleteAllNodes();
	static void fillNodesFromVector(Complex* vector, std::map<std::string, size_t>& nodeIndexMap);

public: //Static Node Creation
	static Node* createNode(const std::string& nodeName);

private: //Constructors
	Node(const std::string& name);
	~Node();

public: //Matrix Operations
	void fillFromVector(Complex* vector, std::map<std::string, size_t>& nodeIndexMap) { m_nodalVoltage = vector[nodeIndexMap[m_name]]; }

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
	Complex getFlowCurrent(double angularFrequency) const;
	Node(const Node&) = delete;
	void operator=(const Node&) = delete;
};

#endif //_NODE_H