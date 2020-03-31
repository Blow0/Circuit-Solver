#ifndef _INDUCTOR_H
#define _INDUCTOR_H

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

public: //Matrix Operations
	void injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	void injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency = 0.0);
	void injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency = 0.0);

public: //Setters
	inline void setInductance(double inductance) { m_inductance = abs(inductance); }
	inline void setPosNode(Node& posNode) { m_posNode = &posNode; }
	inline void setNegNode(Node& negNode) { m_negNode = &negNode; }

public: //Getters
	inline double getInductance() const { return m_inductance; }
	inline Node* getPosNode() const { return m_posNode; }
	inline Node* getNegNode() const { return m_negNode; }
	inline Complex getImpedance(double angularFrequency) const { return Complex(0.0, abs(angularFrequency) * m_inductance); }
	inline Complex getAdmittance(double angularFrequency) const { return Complex(small_or_zero(abs(angularFrequency) * m_inductance), abs(angularFrequency) * m_inductance).getInverse(); }
	inline Complex getVoltageDiff() const { return m_posNode->getNodalVoltage() - m_negNode->getNodalVoltage(); }
	inline Complex getCurrent(double angularFrequency) const { return getVoltageDiff() * getAdmittance(angularFrequency); }
	inline Complex getStoredPower(double angularFrequency) const { return getVoltageDiff() * getCurrent(angularFrequency).getComplement(); }

	Inductor(const Inductor&) = delete;
	void operator=(const Inductor&) = delete;
};

#endif //_INDUCTOR_H