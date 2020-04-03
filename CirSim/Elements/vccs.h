#ifndef _VCCS_H
#define _VCCS_H

#include "currentsource.h"
#include "../Node/node.h"

class VCCS : public CurrentSource
{
private: //Members
	std::string m_controlPosNode;
	std::string m_controlNegNode;
	Complex m_voltageFactor;

public: //Static Current controlled Voltage Source creation
	static VCCS* createVCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex voltageFactor, const std::string& controlPosNode, const std::string& controlNegNode, Complex internalAdmittance);

private: //Constructors
	VCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex voltageFactor, const std::string& controlPosNode, const std::string& controlNegNode, Complex internalAdmittance);
	~VCCS();

public: //Matrix Operations
	void injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	void injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency = 0.0);
	void injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency = 0.0);

public: //Setters
	inline void setVoltageFactor(Complex voltageFactor) { m_voltageFactor = voltageFactor; }
	inline void setControlPosNode(const std::string& controlPosNode) { m_controlPosNode = controlPosNode; }
	inline void setControlNegNode(const std::string& controlNegNode) { m_controlNegNode = controlNegNode; }

private: //Blocked Setters
	inline void setSupplyCurrent(Complex current) { }

public: //Getters
	inline Node* getControlPosNode() const { Node* node = Node::getNode(m_controlPosNode); if (node == nullptr) throw std::runtime_error("VCCS: Couldn't find ControlPosNode"); return node; }
	inline Node* getControlNegNode() const { Node* node = Node::getNode(m_controlNegNode); if (node == nullptr) throw std::runtime_error("VCCS: Couldn't find ControlNegNode"); return node; }
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