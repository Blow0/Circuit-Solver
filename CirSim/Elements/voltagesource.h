#ifndef _VOLTAGESOURCE_H
#define _VOLTAGESOURCE_H

#include <list>
#include "element.h"
#include "../Node/node.h"

class VoltageSource : public Element
{
protected: //Members
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
	~VoltageSource();

public: //Setters
	inline virtual void setSupplyVoltage(Complex supplyVoltage) { m_supplyVoltage = supplyVoltage; }
	inline virtual void setInternalImpedance(Complex internalImpedance) { m_internalImpedance = internalImpedance; }

public: //Getters
	inline Node* getposNode() const { return m_posNode; }
	inline Node* getnegNode() const { return m_negNode; }
	inline Complex getCurrent() const { return m_current; }
	inline Complex getVoltageDiff() const { return m_posNode->getNodalVoltage() - m_negNode->getNodalVoltage(); }
	inline Complex getSupplyVoltage() const { return m_supplyVoltage; }
	inline Complex getPowerSupplied() const { return m_current * getSupplyVoltage(); }
	inline Complex getPowerDissipated() const { return m_internalImpedance * getCurrent().getMagnitudeSqr(); }
	inline Complex getTotalPowerSupplied() const { return getPowerSupplied() - getPowerDissipated(); }

public: //Static Methods
	static inline size_t getVoltageSrcsCount() { return m_voltageSources.size(); }

	VoltageSource(const VoltageSource&) = delete;
	void operator=(const VoltageSource&) = delete;
};

#endif //_VOLTAGESOURCE_H
