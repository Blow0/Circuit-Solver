#ifndef _VCCS_H
#define _VCCS_H

#include "currentsource.h"
#include "../Node/node.h"

class VCCS : public CurrentSource
{
private: //Members
	Node* m_controlPosNode;
	Node* m_controlNegNode;
	Complex m_voltageFactor;

public: //Static Current controlled Voltage Source creation
	static VCCS* createVCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex voltageFactor, Node* controlPosNode, Node* controlNegNode, Complex internalAdmittance);

private: //Constructors
	VCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex voltageFactor, Node* controlPosNode, Node* controlNegNode, Complex internalAdmittance);
	~VCCS();

public: //Matrix Operations
	void injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);

public: //Setters
	inline void setVoltageFactor(Complex voltageFactor) { m_voltageFactor = voltageFactor; }
	inline void setControlPosNode(Node& controlPosNode) { m_controlPosNode = &controlPosNode; }
	inline void setControlNegNode(Node& controlNegNode) { m_controlNegNode = &controlNegNode; }

private: //Blocked Setters
	inline void setSupplyCurrent(Complex current) { }

public: //Getters
	inline Node* getControlPosNode() const { return m_controlPosNode; }
	inline Node* getControlNegNode() const { return m_controlNegNode; }
	inline Complex getVoltageFactor() const { return m_voltageFactor; }
	inline Complex getSupplyCurrent() const { return m_voltageFactor * getControlVoltage(); }
	inline Complex getControlVoltage() const { return m_controlPosNode->getNodalVoltage() - m_controlNegNode->getNodalVoltage(); }

	VCCS(const VCCS&) = delete;
	void operator=(const VCCS&) = delete;
};

#endif //_VCCS_H