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
	static CurrentSource* createCurrentSource(const std::string& currentSrcName, Node& posNode, Node& negNode, Complex current);

private: //Constructors
	CurrentSource(const std::string& currentSrcName, Node& posNode, Node& negNode, Complex current);
	~CurrentSource();

public: //Setters
	inline void setVoltage(Complex voltage) { m_voltage = voltage; }
	inline void setCurrent(Complex current) { m_current = current; }

public: //Getters
	inline Node* getposNode() const { return m_posNode; }
	inline Node* getnegNode() const { return m_negNode; }
	inline Complex getCurrent() const { return m_current; }
	inline Complex getVoltageDiff() const { return m_posNode->getNodalVoltage - m_negNode->getNodalVoltage; }
	inline double getPowerDelivered() const { return m_current.getMagnitude() * m_voltage.getMagnitude(); }

};

#endif // !_CURRENTSOURCE_H
