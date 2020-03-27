#ifndef _CCVS_H
#define _CCVS_H

#include "element.h"
#include "voltagesource.h"
#include "currentcontrolled.h"
#include "../Node/node.h"

class CCVS : public VoltageSource, public CurrentControlledSource
{
private: //Members
	
	//Controlling element
	Complex m_currentFactor;
	Element* m_controlCurrent;

public: //Static Current controlled Voltage Source creation
	static CCVS* createCCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Element* controlCurrent, Complex internalImpedance);
private: //Constructors
	CCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Element* controlCurrent, Complex internalImpedance = 0);
	~CCVS();

public: //Setters
	inline void setVoltageFactor(Complex factor) { m_currentFactor = factor;  }

private://Setters
	inline void setSupplyVoltage(Complex supplyVoltage) {}

public: //Getters

	CCVS(const CCVS&) = delete;
	void operator=(const CCVS&) = delete;
};

#endif // !_CCVS_H
