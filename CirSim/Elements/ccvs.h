#ifndef _CCVS_H
#define _CCVS_H

#include "voltagesource.h"
#include "../Node/node.h"

class CCVS : public VoltageSource
{
private: //Members
	std::string m_controlElementName;
	ElementType m_controlElementType;
	Complex m_currentFactor;

public: //Static Current controlled Voltage Source creation
	static CCVS* createCCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex currentFactor, ElementType controlElementType, const std::string& controlElementName, Complex internalImpedance = 0.0);

private: //Constructors
	CCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex currentFactor, ElementType controlElementType, const std::string& controlElementName, Complex internalImpedance);

public: //Destructor
	~CCVS();

public: //Matrix Operations
	void injectIntoMatrix(Complex* matrix, size_t matrixWidth, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	void injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	void injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);

public: //Setters
	inline void setCurrentFactor(Complex currentFactor) { m_currentFactor = currentFactor;  }
	inline void setControlElementType(ElementType controlElementType) { m_controlElementType = controlElementType; }
	inline void setControlElementName(const std::string& controlElementName) { m_controlElementName = controlElementName; }

private: //Blocked Setters
	inline void setSupplyVoltage(Complex supplyVoltage) { }

public: //Getters
	inline Element* getControlElement() const { Element* element = Element::getElement(elementNameWithType(m_controlElementName, m_controlElementType)); if (element == nullptr) throw std::runtime_error("CCCS: Couldn't find ControlElement"); return element; }
	inline Complex getCurrentFactor() const { return m_currentFactor; }
	Complex getControlCurrent(double angularFrequency) const;
	inline Complex getSupplyVoltage(double angularFrequency) const { return m_currentFactor * getControlCurrent(angularFrequency); }
	inline Complex getPowerSupplied(double angularFrequency) const { return m_current * getSupplyVoltage(angularFrequency); }
	inline Complex getPowerDissipated() const { return m_internalImpedance * getCurrent().getMagnitudeSqr(); }
	inline Complex getTotalPowerSupplied(double angularFrequency) const { return getPowerSupplied(angularFrequency) - getPowerDissipated(); }

private: //Blocked Getters
	inline Complex getSupplyVoltage() const { }
	inline Complex getPowerSupplied() const { }
	inline Complex getTotalPowerSupplied() const { }

	CCVS(const CCVS&) = delete;
	void operator=(const CCVS&) = delete;
};

#endif //_CCVS_H
