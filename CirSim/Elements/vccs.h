#ifndef _VCCS_H
#define _VCCS_H

#include "currentsource.h"
#include "../Node/node.h"

class VCCS : public CurrentSource
{
private: //Members
	//Controlling element
	Complex m_voltageFactor;
	Node* m_controlPosNode;
	Node* m_controlNegNode;


public: //Static Current controlled Voltage Source creation
	static VCCS* createVCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Node* controlPosNode, Node* controlNegNode, Complex internalAdmittance);
private: //Constructors
	VCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Node* controlPosNode, Node* controlNegNode, Complex internalAdmittance);
	~VCCS();

public: //Setters
	inline void setVoltageFactor(Complex factor) { m_voltageFactor = factor; }
	inline void setSupplyCurrent(Complex current) { }

public: //Getters
	inline Complex getControlVoltage() const { return m_controlPosNode->getNodalVoltage() - m_controlNegNode->getNodalVoltage(); }
	inline Complex getSupplyCurrent() const { return m_supplyCurrent = m_voltageFactor * getControlVoltage(); }

	VCCS(const VCCS&) = delete;
	void operator=(const VCCS&) = delete;
};

#endif