#ifndef _CURRENTSOURCE_H
#define _CURRENTSOURCE_H

#include "element.h"
#include "../Node/node.h"

class CurrentSource : public Element
{
private: //Members
	Node* m_posNode;
	Node* m_negNode;
	Complex m_current;
	Complex m_voltage; //Will be calculated


public: //Static Voltage Source creation
	static CurrentSource* createCurrentSource(const std::string& voltageSrcName, Node& posNode, Node& negNode, double voltage = 0, double phase = 0);

private: //Constructors
	CurrentSource(const std::string& voltageSrcName, Node& posNode, Node& negNode, double voltage = 0, double phase = 0);
	~CurrentSource();

public: //Setters
	inline void setVoltage(double voltage) { m_voltage = voltage; }
	inline void setCurrent(double current) { m_current = current; }

public: //Getters
	inline Node* getposNode() const { return m_posNode; }
	inline Node* getnegNode() const { return m_negNode; }
	inline Complex getCurrent() const { return m_current; }
	inline Complex getVoltageDiff() const { return m_posNode->getNodalVoltage - m_negNode->getNodalVoltage; }

};

#endif // !_CURRENTSOURCE_H
