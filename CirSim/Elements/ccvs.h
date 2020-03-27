#ifndef _CCVS_H
#define _CCVS_H

#include "voltagesource.h"
#include "../Node/node.h"

class CCVS : public VoltageSource
{
private: //Members
	Element* m_controlElement;
	Complex m_currentFactor;

public: //Static Current controlled Voltage Source creation
	static CCVS* createCCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex currentFactor, Element* controlElement, Complex internalImpedance);

private: //Constructors
	CCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex currentFactor, Element* controlElement, Complex internalImpedance = 0);
	~CCVS();

public: //Setters
	inline void setCurrentFactor(Complex currentFactor) { m_currentFactor = currentFactor;  }

private: //Blocked Setters
	inline void setSupplyVoltage(Complex supplyVoltage) {}

public: //Getters
	inline Complex getSupplyVoltage() const { return m_currentFactor * getControlCurrent(); }
	inline Complex getControlCurrent() const { return 0; }

	CCVS(const CCVS&) = delete;
	void operator=(const CCVS&) = delete;
};

#endif //_CCVS_H
