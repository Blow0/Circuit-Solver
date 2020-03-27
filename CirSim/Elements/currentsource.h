#ifndef _CURRENTSOURCE_H
#define _CURRENTSOURCE_H

#include "element.h"
#include "resistor.h"
#include "../Node/node.h"

class CurrentSource : public Element
{
private: //Members
	Node* m_posNode;
	Node* m_negNode;
	Complex m_supplyCurrent;
	Complex m_internalAdmittance;

public: //Static Voltage Source creation
	static CurrentSource* createCurrentSource(const std::string& currentSrcName, Node& posNode, Node& negNode, Complex supplyCurrent, Complex internalAdmittance = 0);

protected: //Constructors
	CurrentSource(const std::string& currentSrcName, Node& posNode, Node& negNode, Complex supplyCurrent, Complex internalAdmittance = 0);
	virtual ~CurrentSource();

public: //Setters
	inline virtual void setSupplyCurrent(Complex supplyCurrent) { m_supplyCurrent = supplyCurrent; }
	inline void setInternalAdmittance(Complex internalAdmittance) { m_internalAdmittance = internalAdmittance; }

public: //Getters
	inline Node* getposNode() const { return m_posNode; }
	inline Node* getnegNode() const { return m_negNode; }
	inline Complex getInternalAdmittance() { return m_internalAdmittance; }
	inline Complex getVoltageDiff() const { return m_posNode->getNodalVoltage() - m_negNode->getNodalVoltage(); }
	inline Complex getCurrent() const { return (m_supplyCurrent - getVoltageDiff() * m_internalAdmittance); }
	inline virtual Complex getSupplyCurrent() const { return m_supplyCurrent; }
	inline Complex getPowerSupplied() const { return m_supplyCurrent * getVoltageDiff(); }
	inline Complex getPowerDissipated() const { return m_internalAdmittance * getVoltageDiff().getMagnitudeSqr(); }
	inline Complex getTotalPowerSupplied() const { return getPowerSupplied() - getPowerDissipated(); }

private: //Helpers
	void virtual inject(Complex* matrix, size_t width, double angularFrequency = 0)
	{
		size_t lastRow = width - 1;
		size_t posIdx = Node::getIndex(m_posNode);
		size_t negIdx = Node::getIndex(m_negNode);

		//Inject Current
		matrix[posIdx * width + lastRow] += -getSupplyCurrent();
		matrix[negIdx * width + lastRow] +=  getSupplyCurrent();
		//Inject internal Admittance
		matrix[posIdx * width + posIdx] +=  m_internalAdmittance;
		matrix[posIdx * width + negIdx] += -m_internalAdmittance;
		matrix[negIdx * width + posIdx] += -m_internalAdmittance;
		matrix[negIdx * width + negIdx] +=  m_internalAdmittance;
	}

public:
	CurrentSource(const CurrentSource&) = delete;
	void operator=(const CurrentSource&) = delete;
};

#endif //_CURRENTSOURCE_H
