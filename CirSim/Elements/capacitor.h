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

public: //Destructor
	~Capacitor();

public: //Matrix Operations
	void injectIntoMatrix(Complex* matrix, size_t matrixWidth, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	void injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	void injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);

public: //Setters
	inline void setCapacitance(double capacitance) { m_capacitance = abs(capacitance); }
	inline void setPosNode(Node& posNode) { m_posNode = &posNode; }
	inline void setNegNode(Node& negNode) { m_negNode = &negNode; }

public: //Getters
	inline double getCapacitance() const { return m_capacitance; }
	inline Node* getPosNode() const { return m_posNode; }
	inline Node* getNegNode() const { return m_negNode; }
	inline Complex getImpedance(double angularFrequency) const { return Complex(0.0, check_not_zero(abs(angularFrequency) * m_capacitance)).getInverse(); }
	inline Complex getAdmittance(double angularFrequency) const { return Complex(0.0, abs(angularFrequency) * m_capacitance); }
	inline Complex getVoltageDiff() const { return m_posNode->getNodalVoltage() - m_negNode->getNodalVoltage(); }
	inline Complex getCurrent(double angularFrequency) const { return getVoltageDiff() * getAdmittance(angularFrequency); }
	inline Complex getPowerStored(double angularFrequency) const { return getVoltageDiff() * getCurrent(angularFrequency).getComplement(); }

public:
	Capacitor(const Capacitor&) = delete;
	void operator=(const Capacitor&) = delete;
};

#endif //_CAPACITOR_H