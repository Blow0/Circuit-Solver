#ifndef _INDUCTOR_H
#define _INDUCTOR_H

#include <string>
#include "element.h"
#include "../Node/node.h"

class Inductor : public Element
{
private: //Members
	Node* m_posNode;
	Node* m_negNode;
	double m_inductance;

public: //Static Inductor Creation
	static Inductor* createInductor(const std::string& inductorName, Node& posNode, Node& negNode, double inductance = 0);

private: //Constructors
	Inductor(const std::string& inductorName, Node& posNode, Node& negNode, double inductance = 0);
	~Inductor();

public: //Setters
	inline void setInductance(double inductance) { m_inductance = abs(inductance); }

public: //Getters
	inline double getInductance() const { return m_inductance; }
	inline Node* getPosNode() const { return m_posNode; }
	inline Node* getNegNode() const { return m_negNode; }
	inline Complex getVoltageDiff() const { return m_posNode->getNodalVoltage() - m_negNode->getNodalVoltage(); }
	inline Complex getCurrent(double angularFrequency) const { return getVoltageDiff() / Complex(small_or_zero(abs(angularFrequency) * m_inductance), abs(angularFrequency) * m_inductance); }
};

#endif //_INDUCTOR_H