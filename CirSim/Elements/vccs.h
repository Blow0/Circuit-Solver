#ifndef _VCCS_H
#define _VCCS_H

#include "currentsource.h"
#include "../Node/node.h"

class VCCS : public CurrentSource
{
private: //Members
	std::string m_controlPosNodeName;
	std::string m_controlNegNodeName;
	Complex m_voltageFactor;

public: //Static Current controlled Voltage Source creation
	static VCCS* createVCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex voltageFactor, const std::string& controlPosNodeName, const std::string& controlNegNodeName, Complex internalAdmittance = 0.0);

private: //Constructors
	VCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex voltageFactor, const std::string& controlPosNodeName, const std::string& controlNegNodeName, Complex internalAdmittance);

public: //Destructor
	~VCCS();

public: //Matrix Operations
	void injectIntoMatrix(Complex* matrix, size_t matrixWidth, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	void injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	void injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);

public: //Setters
	inline void setVoltageFactor(Complex voltageFactor) { m_voltageFactor = voltageFactor; }
	inline void setControlPosNodeName(const std::string& controlPosNodeName) { m_controlPosNodeName = controlPosNodeName; }
	inline void setControlNegNodeName(const std::string& controlNegNodeName) { m_controlNegNodeName = controlNegNodeName; }

private: //Blocked Setters
	inline void setSupplyCurrent(Complex current) { }

public: //Getters
	inline Node* getControlPosNode() const { Node* node = Node::getNode(m_controlPosNodeName); if (node == nullptr) throw std::runtime_error("VCVS: Couldn't find ControlPosNode"); return node; }
	inline Node* getControlNegNode() const { Node* node = Node::getNode(m_controlNegNodeName); if (node == nullptr) throw std::runtime_error("VCVS: Couldn't find ControlNegNode"); return node; }
	inline Node* getControlPosNodeName() const { m_controlPosNodeName; }
	inline Node* getControlNegNodeName() const { m_controlNegNodeName; }
	inline Complex getVoltageFactor() const { return m_voltageFactor; }
	inline Complex getControlVoltage() const { return getControlPosNode()->getNodalVoltage() - getControlNegNode()->getNodalVoltage(); }
	inline Complex getCurrent() const { return (getSupplyCurrent() - getVoltageDiff() * m_internalAdmittance); }
	inline Complex getSupplyCurrent() const { return m_voltageFactor * getControlVoltage(); }
	inline Complex getPowerSupplied() const { return getSupplyCurrent() * getVoltageDiff(); }
	inline Complex getPowerDissipated() const { return m_internalAdmittance * getVoltageDiff().getMagnitudeSqr(); }
	inline Complex getTotalPowerSupplied() const { return getPowerSupplied() - getPowerDissipated(); }

	VCCS(const VCCS&) = delete;
	void operator=(const VCCS&) = delete;
};

#endif //_VCCS_H