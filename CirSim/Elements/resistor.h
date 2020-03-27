#ifndef _RESISTOR_H
#define _RESISTOR_H

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
	inline Complex getImpedance() const { return m_resistance; }
	inline Complex getAdmittance() const { return 1.0 / m_resistance; }
	inline Complex getVoltageDiff() const { return m_posNode->getNodalVoltage() - m_negNode->getNodalVoltage(); }
	inline Complex getCurrent() const { return getVoltageDiff() / m_resistance; }
	inline Complex getDissipatedPower() const { return getVoltageDiff() * getCurrent().getComplement(); }

private: //Helpers
	void inject(Complex* matrix, unsigned int width, double angularFrequency = 0)
	{
		size_t posIdx = Node::getIndex(m_posNode);
		size_t negIdx = Node::getIndex(m_negNode);

		matrix[posIdx * width + posIdx] +=  getAdmittance();
		matrix[posIdx * width + negIdx] += -getAdmittance();
		matrix[negIdx * width + posIdx] += -getAdmittance();
		matrix[negIdx * width + negIdx] +=  getAdmittance();
	}

public:
	Resistor(const Resistor&) = delete;
	void operator=(const Resistor&) = delete;
};

#endif //_RESISTOR_H