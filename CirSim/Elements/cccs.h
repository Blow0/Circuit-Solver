#ifndef _CCCS_H
#define _CCCS_H

#include "currentsource.h"
#include "../Node/node.h"

class CCVS;

class CCCS : public CurrentSource
{
	friend class CCVS;

private: //Members
	std::string m_controlElement;
	Complex m_currentFactor;

public: //Static Current controlled Voltage Source creation
	static CCCS* createCCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex currentFactor, const std::string& controlElement, Complex internalAdmittance);

private: //Constructors
	CCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex currentFactor, const std::string& controlElement, Complex internalAdmittance = 0);
	~CCCS();

public: //Matrix Operations
	void injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);

private: //Matrix Operations Helpers
	void injectSupplyCurrentIntoCCVS(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	void injectSupplyCurrentIntoCCCS(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);

public: //Setters
	inline void setCurrentFactor(Complex currentFactor) { m_currentFactor = currentFactor; }
	inline void setControlElement(const std::string& controlElement) { m_controlElement = controlElement; }

private: //Blocked Setters
	inline void setSupplyCurrent(Complex supplyCurrent) { }

public: //Getters
	inline Element* getControlElement() const { Element* element = Element::getElement(m_controlElement); if (element == nullptr) throw std::runtime_error("CCCS: Couldn't find ControlElement"); return element; }
	inline Complex getCurrentFactor() const { return m_currentFactor; }
	inline Complex getSupplyCurrent() const { return m_currentFactor * getControlCurrent(); }
	inline Complex getControlCurrent() const { return 0; }

	CCCS(const CCCS&) = delete;
	void operator=(const CCCS&) = delete;
};

#endif //_CCCS_H
