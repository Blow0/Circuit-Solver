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

private:
	static std::list <VCVS*> m_VCVS;
public: //Static Current controlled Voltage Source creation
	static VCVS* createVCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex voltageFactor, Node* controlPosNode, Node* controlNegNode, Complex internalImpedance = 0);

private: //Constructors
	VCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex voltageFactor, Node* controlPosNode, Node* controlNegNode, Complex internalImpedance = 0);
	~VCVS();

public: //Setters
	inline void setVoltageFactor(Complex voltageFactor) { m_voltageFactor = voltageFactor;  }

private: //Blocked Setters
	inline void setSupplyVoltage(Complex supplyVoltage) {}

public: //Getters
	inline Complex getSupplyVoltage() const { return m_voltageFactor * getControlVoltage(); }
	inline Complex getControlVoltage() const { return m_controlPosNode->getNodalVoltage() - m_controlNegNode->getNodalVoltage(); }

private:
	void virtual inject(Complex* matrix, size_t width, double angularFrequency = 0)
	{
		size_t lastRow = width - 1;
		size_t nodes = Node::getNodesCount() + getVoltageSrcsCount();
		size_t posIdx = Node::getIndex(getposNode());
		size_t negIdx = Node::getIndex(getnegNode());
		size_t controlPosIdx = Node::getIndex(m_controlPosNode);
		size_t controlNegIdx = Node::getIndex(m_controlNegNode);

		//Inject Indep VS
		for (unsigned int i = 0; i < m_VCVS.size(); i++)
		{
			matrix[(nodes + i) * width + posIdx] = 1;
			matrix[(nodes + i) * width + negIdx] = -1;
			//Inject internal Impedance
			matrix[(nodes + i) * width + nodes + i] = getInternalImpedance();
			matrix[posIdx * width + nodes + i] = -1;
			matrix[negIdx * width + nodes + i] = 1;
			matrix[(nodes + i) * width + nodes + controlPosIdx] = -m_voltageFactor;
			matrix[(nodes + i) * width + nodes + controlNegIdx] =  m_voltageFactor;

		}
	}

public:
	VCVS(const VCVS&) = delete;
	void operator=(const VCVS&) = delete;
};

#endif //_VCVS_H