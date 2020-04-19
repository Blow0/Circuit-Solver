#ifndef _CCCS_H
#define _CCCS_H

#include "currentsource.h"
#include "../Node/node.h"

class CCVS;

class CCCS : public CurrentSource
{
	friend class CCVS;

private: //Members
	ElementType m_controlElementType;
	std::string m_controlElementName;
	Complex m_currentFactor;

public: //Static Current controlled Voltage Source creation
	static CCCS* createCCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex currentFactor, ElementType controlElementType, const std::string& controlElementName, Complex internalAdmittance = 0.0);

private: //Constructors
	CCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex currentFactor, ElementType controlElementType, const std::string& controlElementName, Complex internalAdmittance);

public: //Destructor
	~CCCS();

public: //Matrix Operations
	void injectIntoMatrix(Complex* matrix, size_t matrixWidth, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);

private: //Matrix Operations Helpers
	void injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	void injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);

public: //Setters
	inline void setCurrentFactor(Complex currentFactor) { m_currentFactor = currentFactor; }
	inline void setControlElementType(ElementType controlElementType) { m_controlElementType = controlElementType; }
	inline void setControlElementName(const std::string& controlElementName) { m_controlElementName = controlElementName; }

private: //Blocked Setters
	inline void setSupplyCurrent(Complex supplyCurrent) { }

public: //Getters
	inline Element* getControlElement() const { Element* element = Element::getElement(elementNameWithType(m_controlElementName, m_controlElementType)); if (element == nullptr) throw std::runtime_error("CCCS: Couldn't find ControlElement"); return element; }
	inline ElementType getControlElementType() const { return m_controlElementType; }
	inline const std::string& getControlElementName() const { return m_controlElementName; }
	inline Complex getCurrentFactor() const { return m_currentFactor; }
	Complex getControlCurrent(double angularFrequency) const;
	inline Complex getCurrent(double angularFrequency) const { return (getSupplyCurrent(angularFrequency) - getVoltageDiff() * m_internalAdmittance); }
	inline Complex getSupplyCurrent(double angularFrequency) const { return m_currentFactor * getControlCurrent(angularFrequency); }
	inline Complex getPowerSupplied(double angularFrequency) const { return getSupplyCurrent(angularFrequency) * getVoltageDiff(); }
	inline Complex getPowerDissipated() const { return m_internalAdmittance * getVoltageDiff().getMagnitudeSqr(); }
	inline Complex getTotalPowerSupplied(double angularFrequency) const { return getPowerSupplied(angularFrequency) - getPowerDissipated(); }

private: //Blocked Getters
	inline Complex getCurrent() const { }
	inline Complex getSupplyCurrent() const { }
	inline Complex getPowerSupplied() const { }
	inline Complex getTotalPowerSupplied() const { }

	CCCS(const CCCS&) = delete;
	void operator=(const CCCS&) = delete;
};

#endif //_CCCS_H
