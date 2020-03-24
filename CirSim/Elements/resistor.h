#ifndef _RESISTOR_H
#define _RESISTOR_H

#include <string>
#include "element.h"
#include "../Node/node.h"

class Resistor : public Element
{
private: //Members
	Node* m_posNode;
	Node* m_negNode;
	double m_resistance;

public: //Static Resistor Creation
	static Resistor* createResistor(const std::string& resistorName, Node& posNode, Node& negNode, double resistance = 0);

private: //Constructors
	Resistor(const std::string& resistorName, Node& posNode, Node& negNode, double resistance = 0);
	~Resistor();

public: //Setters
	inline void setResistance(double resistance) { m_resistance = check_not_zero(abs(resistance)); }

public: //Getters
	inline double getResistance() const { return m_resistance; }
	inline Node* getPosNode() const { return m_posNode; }
	inline Node* getNegNode() const { return m_negNode; }
	inline Complex getVoltageDiff() const { return m_posNode->getNodalVoltage() - m_negNode->getNodalVoltage(); }
	inline Complex getCurrent() const { return getVoltageDiff() / m_resistance; }
};

#endif //_RESISTOR_H