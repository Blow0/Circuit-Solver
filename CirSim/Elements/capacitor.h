#ifndef _CAPACITOR_H
#define _CAPACITOR_H

#include "element.h"
#include "../Node/node.h"

class Capacitor : public Element
{
private: //Members
	Node* m_posNode;
	Node* m_negNode;
	double m_capacitance;

public: //Static Inductor Creation
	static Capacitor* createCapacitor(const std::string& capacitorName, Node& posNode, Node& negNode, double inductance = 0);

private: //Constructors
	Capacitor(const std::string& capacitorName, Node& posNode, Node& negNode, double inductance = 0);
	~Capacitor();

public: //Setters
	inline void setCapacitance(double capacitance) { m_capacitance = abs(capacitance); }

public: //Getters
	inline double getCapacitance() const { return m_capacitance; }
	inline Node* getPosNode() const { return m_posNode; }
	inline Node* getNegNode() const { return m_negNode; }
	inline Complex getImpedance(double angularFrequency) const { return Complex(small_or_zero(abs(angularFrequency) * m_capacitance), abs(angularFrequency) * m_capacitance).getInverse(); }
	inline Complex getAdmittance(double angularFrequency) const { return Complex(0.0, abs(angularFrequency) * m_capacitance); }
	inline Complex getVoltageDiff() const { return m_posNode->getNodalVoltage() - m_negNode->getNodalVoltage(); }
	inline Complex getCurrent(double angularFrequency) const { return getVoltageDiff() * getAdmittance(angularFrequency); }
	inline Complex getStoredPower(double angularFrequency) const { return getVoltageDiff() * getCurrent(angularFrequency).getComplement(); }

private://Helpers
	void inject(Complex* matrix, size_t width, double angularFrequency)
	{
		size_t posIdx = Node::getIndex(m_posNode);
		size_t negIdx = Node::getIndex(m_negNode);

		matrix[posIdx * width + posIdx] += getAdmittance(angularFrequency);
		matrix[posIdx * width + negIdx] += -getAdmittance(angularFrequency);
		matrix[negIdx * width + posIdx] += -getAdmittance(angularFrequency);
		matrix[negIdx * width + negIdx] += getAdmittance(angularFrequency);
	}

public:
	Capacitor(const Capacitor&) = delete;
	void operator=(const Capacitor&) = delete;
};

#endif //_CAPACITOR_H