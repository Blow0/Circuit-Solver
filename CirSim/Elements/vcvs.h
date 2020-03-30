#ifndef _VCVS_H
#define _VCVS_H

#include "voltagesource.h"
#include "../Node/node.h"

class VCVS : public VoltageSource
{
private: //Members
	Node* m_controlPosNode;
	Node* m_controlNegNode;
	Complex m_voltageFactor;

public: //Static Current controlled Voltage Source creation
	static VCVS* createVCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex voltageFactor, Node* controlPosNode, Node* controlNegNode, Complex internalImpedance);

private: //Constructors
	VCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex voltageFactor, Node* controlPosNode, Node* controlNegNode, Complex internalImpedance = 0);
	~VCVS();

public: //Matrix Operations
	void injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);

public: //Setters
	inline void setVoltageFactor(Complex voltageFactor) { m_voltageFactor = voltageFactor;  }
	inline void setControlPosNode(Node& controlPosNode) { m_controlPosNode = &controlPosNode; }
	inline void setControlNegNode(Node& controlNegNode) { m_controlNegNode = &controlNegNode; }

private: //Blocked Setters
	inline void setSupplyVoltage(Complex supplyVoltage) { }

public: //Getters
	inline Node* getControlPosNode() const { return m_controlPosNode; }
	inline Node* getControlNegNode() const { return m_controlNegNode; }
	inline Complex getVoltageFactor() const { return m_voltageFactor; }
	inline Complex getSupplyVoltage() const { return m_voltageFactor * getControlVoltage(); }
	inline Complex getControlVoltage() const { return m_controlPosNode->getNodalVoltage() - m_controlNegNode->getNodalVoltage(); }

	VCVS(const VCVS&) = delete;
	void operator=(const VCVS&) = delete;
};

#endif //_VCVS_H