#ifndef _VOLTAGESOURCE_H
#define _VOLTAGESOURCE_H

#include <list>
#include "element.h"
#include "../Node/node.h"

class VoltageSource : public Element
{
private: //Members
	Node* m_posNode;
	Node* m_negNode;
	Complex m_supplyVoltage;
	Complex m_current;
	Complex m_internalImpedance;

protected: //Static Members
	static std::list <VoltageSource*> m_voltageSources;

public: //Static Voltage Source creation
	static VoltageSource* createVoltageSource(const std::string& voltageSrcName, Node& posNode, Node& negNode, Complex supplyVoltage, Complex internalImpedance = 0);

protected: //Constructors
	VoltageSource(const std::string& voltageSrcName, Node& posNode, Node& negNode, Complex supplyVoltage, Complex internalImpedance = 0);
	virtual ~VoltageSource();

public: //Setters
	inline virtual void setSupplyVoltage(Complex supplyVoltage) { m_supplyVoltage = supplyVoltage; }
	inline virtual void setInternalImpedance(Complex internalImpedance) { m_internalImpedance = internalImpedance; }

public: //Getters
	inline Node* getposNode() const { return m_posNode; }
	inline Node* getnegNode() const { return m_negNode; }
	inline Complex getInternalImpedance() const { return m_internalImpedance; }
	inline Complex getCurrent() const { return m_current; }
	inline Complex getVoltageDiff() const { return m_posNode->getNodalVoltage() - m_negNode->getNodalVoltage(); }
	inline virtual Complex getSupplyVoltage() const { return m_supplyVoltage; }
	inline Complex getPowerSupplied() const { return m_current * getSupplyVoltage(); }
	inline Complex getPowerDissipated() const { return m_internalImpedance * getCurrent().getMagnitudeSqr(); }
	inline Complex getTotalPowerSupplied() const { return getPowerSupplied() - getPowerDissipated(); }
	size_t getVoltageSourceIndex()
	{
		size_t i = 0;
		for (std::list<VoltageSource*>::iterator it = m_voltageSources.begin(); it != m_voltageSources.end(); it++)
		{
			if (*it == this)
				return i;
			i++;
		}
		return -1;
	}

public: //Static Methods
	static inline size_t getVoltageSrcsCount() { return m_voltageSources.size(); }

private://Helpers
	void virtual inject(Complex* matrix, size_t width, double angularFrequency = 0)
	{
		size_t lastRow = width - 1;
		size_t nodes  = Node::getNodesCount();
		size_t posIdx = Node::getIndex(m_posNode);
		size_t negIdx = Node::getIndex(m_negNode);

		
		//Inject Indep VS
		for (unsigned int i = 1; i <= m_voltageSources.size(); i++)
		{
			matrix[(nodes + i) * width + posIdx]	=  1;
			matrix[(nodes + i) * width + negIdx]	= -1;
			//Inject internal Impedance
			matrix[(nodes + i) * width + nodes + i] = m_internalImpedance;
			matrix[posIdx * width + nodes + i]		= -1;
			matrix[negIdx * width + nodes + i]		=  1;

			
		}
	}

public:
	VoltageSource(const VoltageSource&) = delete;
	void operator=(const VoltageSource&) = delete;
};

#endif //_VOLTAGESOURCE_H
