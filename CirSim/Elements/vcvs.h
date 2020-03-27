#ifndef _VCVS_H
#define _VCVS_H

#include "voltagesource.h"
#include "../Node/node.h"

class VCVS : public VoltageSource
{
private: //Members
	Node* m_controlPosNode;
	Node* m_controlNegNode;
	Complex m_voltageFactor;

public: //Static Current controlled Voltage Source creation
	static VCVS* createVCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex voltageFactor, Node* controlPosNode, Node* controlNegNode, Complex internalImpedance = 0);

private: //Constructors
	VCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex voltageFactor, Node* controlPosNode, Node* controlNegNode, Complex internalImpedance = 0);
	~VCVS();

public: //Setters
	inline void setVoltageFactor(Complex voltageFactor) { m_voltageFactor = voltageFactor;  }

private: //Blocked Setters
	inline void setSupplyVoltage(Complex supplyVoltage) {}

public: //Getters
	inline Complex getSupplyVoltage() const { return m_voltageFactor * getControlVoltage(); }
	inline Complex getControlVoltage() const { return m_controlPosNode->getNodalVoltage() - m_controlNegNode->getNodalVoltage(); }

	VCVS(const VCVS&) = delete;
	void operator=(const VCVS&) = delete;
};

#endif //_VCVS_H