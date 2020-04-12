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

public: //Matrix Operations
	void injectIntoMatrix(Complex* matrix, size_t matrixWidth, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	void injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	void injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);

public: //Setters
	inline void setResistance(double resistance) { m_resistance = abs(resistance); }
	inline void setPosNode(Node& posNode) { m_posNode = &posNode; }
	inline void setNegNode(Node& negNode) { m_negNode = &negNode; }

public: //Getters
	inline double getResistance() const { return m_resistance; }
	inline Node* getPosNode() const { return m_posNode; }
	inline Node* getNegNode() const { return m_negNode; }
	inline Complex getImpedance() const { return m_resistance; }
	inline Complex getAdmittance() const { return 1.0 / check_not_zero(m_resistance); }
	inline Complex getVoltageDiff() const { return m_posNode->getNodalVoltage() - m_negNode->getNodalVoltage(); }
	inline Complex getCurrent() const { return getVoltageDiff() / m_resistance; }
	inline Complex getPowerDissipated() const { return getVoltageDiff() * getCurrent().getComplement(); }

	Resistor(const Resistor&) = delete;
	void operator=(const Resistor&) = delete;
};

#endif //_RESISTOR_H