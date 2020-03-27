#ifndef _CCCS_H
#define _CCCS_H

#include "element.h"
#include "voltagesource.h"
#include "currentcontrolled.h"
#include "../Node/node.h"

class CCCS : public Element, public CurrentControlledSource
{
private: //Members
	Node* m_posNode;
	Node* m_negNode;

	Complex m_supplyCurrent; 
	Complex m_voltage; //Will be calculated
	Complex m_internalAdmittance;
	//Controlling element
	Complex m_currentFactor;
	Element* m_controlCurrent;

public: //Static Current controlled Voltage Source creation
	static CCCS* createCCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex factor, Element* controlCurrent, Complex internalAdmittance);
private: //Constructors
	CCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex factor, Element* controlCurrent, Complex internalAdmittance = 0);
	~CCCS();

public: //Setters
	inline void setFactor(Complex factor) { m_currentFactor = factor; }
	inline void setSupplyCurrent(Complex supplyCurrent) { m_supplyCurrent = supplyCurrent; }
	inline void setInternalAdmittance(Complex internalAdmittance) { m_internalAdmittance = internalAdmittance; }

public: //Getters
	inline Node* getposNode() const { return m_posNode; }
	inline Node* getnegNode() const { return m_negNode; }
	inline Complex getVoltageDiff() const { return m_posNode->getNodalVoltage() - m_negNode->getNodalVoltage(); }
	inline Complex getCurrent() const { return (m_supplyCurrent - getVoltageDiff() * m_internalAdmittance); }
	inline Complex getVoltage() const { return m_posNode->getNodalVoltage() - m_negNode->getNodalVoltage(); }
	inline Complex getPowerSupplied() const { return m_supplyCurrent * getVoltageDiff(); }
	inline Complex getPowerDissipated() const { return m_internalAdmittance * getVoltageDiff().getMagnitudeSqr(); }
	inline Complex getTotalPowerSupplied() const { return getPowerSupplied() - getPowerDissipated(); }

	CCCS(const CCCS&) = delete;
	void operator=(const CCCS&) = delete;
};

#endif // !_CCCS_H
