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
	static CurrentSource* createCurrentSource(const std::string& currentSrcName, Node& posNode, Node& negNode, Complex supplyCurrent, Complex internalAdmittance = 0);

protected: //Constructors
	CurrentSource(const std::string& currentSrcName, Node& posNode, Node& negNode, Complex supplyCurrent, Complex internalAdmittance = 0);
	virtual~CurrentSource();

public: //Setters
	inline virtual void setSupplyCurrent(Complex supplyCurrent) { m_supplyCurrent = supplyCurrent; }
	inline void setInternalAdmittance(Complex internalAdmittance) { m_internalAdmittance = internalAdmittance; }

public: //Getters
	inline Node* getposNode() const { return m_posNode; }
	inline Node* getnegNode() const { return m_negNode; }
	inline Complex getVoltageDiff() const { return m_posNode->getNodalVoltage() - m_negNode->getNodalVoltage(); }
	inline Complex getCurrent() const { return (m_supplyCurrent - getVoltageDiff() * m_internalAdmittance); }
	inline virtual Complex getSupplyCurrent() const { return m_supplyCurrent; }
	inline Complex getPowerSupplied() const { return m_supplyCurrent * getVoltageDiff(); }
	inline Complex getPowerDissipated() const { return m_internalAdmittance * getVoltageDiff().getMagnitudeSqr(); }
	inline Complex getTotalPowerSupplied() const { return getPowerSupplied() - getPowerDissipated(); }

	CurrentSource(const CurrentSource&) = delete;
	void operator=(const CurrentSource&) = delete;
};

#endif // !_CURRENTSOURCE_H
