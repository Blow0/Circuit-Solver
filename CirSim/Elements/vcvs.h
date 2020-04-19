#ifndef _VCVS_H
#define _VCVS_H

#include "voltagesource.h"
#include "../Node/node.h"

class VCVS : public VoltageSource
{
private: //Members
	std::string m_controlPosNodeName;
	std::string m_controlNegNodeName;
	Complex m_voltageFactor;

public: //Static Current controlled Voltage Source creation
	static VCVS* createVCVS(const std::string& vcvsName, Node& posNode, Node& negNode, Complex voltageFactor, const std::string& controlPosNodeName, const std::string& controlNegNodeName, Complex internalImpedance = 0.0);

private: //Constructors
	VCVS(const std::string& vcvsName, Node& posNode, Node& negNode, Complex voltageFactor, const std::string& controlPosNodeName, const std::string& controlNegNodeName, Complex internalImpedance);

public: //Destructor
	~VCVS();

public: //Matrix Operations
	void injectIntoMatrix(Complex* matrix, size_t matrixWidth, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	void injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	void injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);

public: //Setters
	inline void setVoltageFactor(Complex voltageFactor) { m_voltageFactor = voltageFactor;  }
	inline void setControlPosNodeName(const std::string& controlPosNodeName) { m_controlPosNodeName = controlPosNodeName; }
	inline void setControlNegNodeName(const std::string& controlNegNodeName) { m_controlNegNodeName = controlNegNodeName; }

private: //Blocked Setters
	inline void setSupplyVoltage(Complex supplyVoltage) { }

public: //Getters
	inline Node* getControlPosNode() const { Node* node = Node::getNode(m_controlPosNodeName); if (node == nullptr) throw std::runtime_error("VCVS: Couldn't find ControlPosNode"); return node; }
	inline Node* getControlNegNode() const { Node* node = Node::getNode(m_controlNegNodeName); if (node == nullptr) throw std::runtime_error("VCVS: Couldn't find ControlNegNode"); return node; }
	inline const std::string& getControlPosNodeName() const { return m_controlPosNodeName; }
	inline const std::string& getControlNegNodeName() const { return m_controlNegNodeName; }
	inline Complex getVoltageFactor() const { return m_voltageFactor; }
	inline Complex getControlVoltage() const { return getControlPosNode()->getNodalVoltage() - getControlNegNode()->getNodalVoltage(); }
	inline Complex getSupplyVoltage() const { return m_voltageFactor * getControlVoltage(); }
	inline Complex getPowerSupplied() const { return m_current * getSupplyVoltage(); }
	inline Complex getPowerDissipated() const { return m_internalImpedance * getCurrent().getMagnitudeSqr(); }
	inline Complex getTotalPowerSupplied() const { return getPowerSupplied() - getPowerDissipated(); }

	VCVS(const VCVS&) = delete;
	void operator=(const VCVS&) = delete;
};

#endif //_VCVS_H