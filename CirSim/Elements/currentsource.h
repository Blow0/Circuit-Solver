#ifndef _CURRENTSOURCE_H
#define _CURRENTSOURCE_H

#include "element.h"
#include "resistor.h"
#include "../Node/node.h"

class CurrentSource : public Element
{
protected: //Members
	Node* m_posNode;
	Node* m_negNode;
	Complex m_supplyCurrent;
	Complex m_internalAdmittance;

public: //Static Voltage Source creation
	static CurrentSource* createCurrentSource(const std::string& currentSrcName, Node& posNode, Node& negNode, Complex supplyCurrent, Complex internalAdmittance = 0.0);

protected: //Constructors
	CurrentSource(const std::string& currentSrcName, ElementType type, Node& posNode, Node& negNode, Complex supplyCurrent, Complex internalAdmittance);
	virtual ~CurrentSource();

private: //Constructors
	CurrentSource(const std::string& currentSrcName, Node& posNode, Node& negNode, Complex supplyCurrent, Complex internalAdmittance);

public: //Matrix Operations
	virtual void injectIntoMatrix(Complex* matrix, size_t matrixWidth, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	virtual void injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	virtual void injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);

public: //Setters
	inline virtual void setSupplyCurrent(Complex supplyCurrent) { m_supplyCurrent = supplyCurrent; }
	inline void setInternalAdmittance(Complex internalAdmittance) { m_internalAdmittance = internalAdmittance; }
	inline void setPosNode(Node& posNode) { m_posNode = &posNode; }
	inline void setNegNode(Node& negNode) { m_negNode = &negNode; }

public: //Getters
	inline Node* getPosNode() const { return m_posNode; }
	inline Node* getNegNode() const { return m_negNode; }
	inline Complex getInternalAdmittance() { return m_internalAdmittance; }
	inline Complex getVoltageDiff() const { return m_posNode->getNodalVoltage() - m_negNode->getNodalVoltage(); }
	inline Complex getCurrent() const { return (getSupplyCurrent() - getVoltageDiff() * m_internalAdmittance); }
	inline Complex getSupplyCurrent() const { return m_supplyCurrent; }
	inline Complex getPowerSupplied() const { return getSupplyCurrent() * getVoltageDiff(); }
	inline Complex getPowerDissipated() const { return m_internalAdmittance * getVoltageDiff().getMagnitudeSqr(); }
	inline Complex getTotalPowerSupplied() const { return getPowerSupplied() - getPowerDissipated(); }

	CurrentSource(const CurrentSource&) = delete;
	void operator=(const CurrentSource&) = delete;
};

#endif //_CURRENTSOURCE_H
