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

public: //Setters
	inline void setVoltageFactor(Complex voltageFactor) { m_voltageFactor = voltageFactor; }

private: //Blocked Setters
	inline void setSupplyCurrent(Complex current) { }

public: //Getters
	inline Complex getSupplyCurrent() const { return m_voltageFactor * getControlVoltage(); }
	inline Complex getControlVoltage() const { return m_controlPosNode->getNodalVoltage() - m_controlNegNode->getNodalVoltage(); }

private: //Helpers
	void inject(Complex* matrix, size_t width, double angularFrequency = 0)
	{
		size_t lastRow = width - 1;
		size_t posIdx = Node::getIndex(getposNode());
		size_t negIdx = Node::getIndex(getnegNode());

		//Inject Current
		Complex sum = getInternalAdmittance() + m_voltageFactor;
		//Inject internal Admittance + voltageFactor
		matrix[posIdx * width + posIdx] +=  sum;
		matrix[posIdx * width + negIdx] += -sum;
		matrix[negIdx * width + posIdx] += -sum;
		matrix[negIdx * width + negIdx] +=  sum;
	}

public:
	VCCS(const VCCS&) = delete;
	void operator=(const VCCS&) = delete;
};

#endif //_VCCS_H