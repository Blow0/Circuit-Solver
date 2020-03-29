#ifndef _CCVS_H
#define _CCVS_H

#include "voltagesource.h"
#include "../Node/node.h"

class CCVS : public VoltageSource
{
private: //Members
	std::string m_controlElement;
	Complex m_currentFactor;

public: //Static Current controlled Voltage Source creation
	static CCVS* createCCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex currentFactor, const std::string& controlElement, Complex internalImpedance);

private: //Constructors
	CCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex currentFactor, const std::string& controlElement, Complex internalImpedance = 0);
	~CCVS();

public: //Matrix Operations
	void injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);

public: //Setters
	inline void setCurrentFactor(Complex currentFactor) { m_currentFactor = currentFactor;  }
	inline void setControlElement(const std::string& controlElement) { m_controlElement = controlElement; }

private: //Blocked Setters
	inline void setSupplyVoltage(Complex supplyVoltage) { }

public: //Getters
	inline Element* getControlElement() const { Element* element = Element::getElement(m_controlElement); if (element == nullptr) throw std::runtime_error("CCVS: Couldn't find ControlElement"); return element; }
	inline Complex getCurrentFactor() const { return m_currentFactor; }
	inline Complex getSupplyVoltage() const { return m_currentFactor * getControlCurrent(); }
	inline Complex getControlCurrent() const { return 0; }

	CCVS(const CCVS&) = delete;
	void operator=(const CCVS&) = delete;
};

#endif //_CCVS_H
