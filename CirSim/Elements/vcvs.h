#ifndef _VCVS_H
#define _VCVS_H

#include "voltagesource.h"
#include "../Node/node.h"

class VCVS : public VoltageSource
{
private: //Members
	Node* m_controlPosNode;
	Node* m_controlNegNode;

	//Controlling element
	Complex m_voltageFactor;

public: //Static Current controlled Voltage Source creation
	static VCVS* createVCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Node* controlPosNode, Node* controlNegNode, Complex internalImpedance = 0);
private: //Constructors
	VCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Node* controlPosNode, Node* controlNegNode, Complex internalImpedance = 0);
	~VCVS();

public: //Setters
	inline void setVoltageFactor(Complex factor) { m_voltageFactor = factor;  }

private://Setters
	inline void setSupplyVoltage(Complex supplyVoltage) {}
public: //Getters

	VCVS(const VCVS&) = delete;
	void operator=(const VCVS&) = delete;
};

#endif // !_VCVS_H