#ifndef _VCCS_H
#define _VCCS_H

#include "element.h"
#include "voltagesource.h"
#include "../Node/node.h"

class VCCS : public Element
{
private: //Members
	Node* m_controlPosNode;
	Node* m_controlNegNode;
	Node* m_posNode;
	Node* m_negNode;

	Complex m_supplyCurrent;
	Complex m_internalAdmittance;
	//Controlling element
	double m_factor;

public: //Static Current controlled Voltage Source creation
	static VCCS* createVCCS(const std::string& ccvsName, Node& posNode, Node& negNode, double factor, Node* controlPosNode, Node* controlNegNode);
	static VCCS* createVCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Node* controlPosNode, Node* controlNegNode);
private: //Constructors
	VCCS(const std::string& ccvsName, Node& posNode, Node& negNode, double factor, Node* controlPosNode, Node* controlNegNode);
	VCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Node* controlPosNode, Node* controlNegNode);
	~VCCS();

public: //Setters
	inline void setCurrent(Complex current) { m_supplyCurrent = current; }

public: //Getters
	inline Node* getposNode() const { return m_posNode; }
	inline Node* getnegNode() const { return m_negNode; }
	inline Complex getCurrent() const { return m_supplyCurrent; }
	inline Complex getVoltageDiff() const { return m_posNode->getNodalVoltage() - m_negNode->getNodalVoltage(); }
	inline Complex getPowerSupplied() const { return m_supplyCurrent * getVoltageDiff(); }
	inline Complex getPowerDissipated() const { return m_internalAdmittance * getVoltageDiff().getMagnitudeSqr(); }
	inline Complex getTotalPowerSupplied() const { return getPowerSupplied() - getPowerDissipated(); }

	VCCS(const VCCS&) = delete;
	void operator=(const VCCS&) = delete;
};

#endif