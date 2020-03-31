#ifndef _VCVS_H
#define _VCVS_H

#include "voltagesource.h"
#include "../Node/node.h"

class VCVS : public VoltageSource
{
private: //Members
	std::string m_controlPosNode;
	std::string m_controlNegNode;
	Complex m_voltageFactor;

public: //Static Current controlled Voltage Source creation
	static VCVS* createVCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex voltageFactor, const std::string& controlPosNode, const std::string& controlNegNode, Complex internalImpedance = 0);

private: //Constructors
	VCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex voltageFactor, const std::string& controlPosNode, const std::string& controlNegNode, Complex internalImpedance = 0);
	~VCVS();

public: //Matrix Operations
	void injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	void injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency = 0.0);
	void injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency = 0.0);

public: //Setters
	inline void setVoltageFactor(Complex voltageFactor) { m_voltageFactor = voltageFactor;  }
	inline void setControlPosNode(const std::string& controlPosNode) { m_controlPosNode = controlPosNode; }
	inline void setControlNegNode(const std::string& controlNegNode) { m_controlNegNode = controlNegNode; }

private: //Blocked Setters
	inline void setSupplyVoltage(Complex supplyVoltage) { }

public: //Getters
	inline Node* getControlPosNode() const { Node* node = Node::getNode(m_controlPosNode); if (node == nullptr) throw std::runtime_error("VCVS: Couldn't find ControlPosNode"); return node; }
	inline Node* getControlNegNode() const { Node* node = Node::getNode(m_controlNegNode); if (node == nullptr) throw std::runtime_error("VCVS: Couldn't find ControlNegNode"); return node; }
	inline Complex getVoltageFactor() const { return m_voltageFactor; }
	inline Complex getSupplyVoltage() const { return m_voltageFactor * getControlVoltage(); }
	inline Complex getControlVoltage() const { return getControlPosNode()->getNodalVoltage() - getControlNegNode()->getNodalVoltage(); }

	VCVS(const VCVS&) = delete;
	void operator=(const VCVS&) = delete;
};

#endif //_VCVS_H