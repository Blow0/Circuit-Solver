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
public:
	static std::map<std::string, size_t> voltageIndexMap;
public: //Static Voltage Source creation
	static VoltageSource* createVoltageSource(const std::string& voltageSrcName, Node& posNode, Node& negNode, Complex supplyVoltage, Complex internalImpedance = 0);
protected: //Constructors
	VoltageSource(const std::string& voltageSrcName, Node& posNode, Node& negNode, Complex supplyVoltage, Complex internalImpedance = 0);
	virtual ~VoltageSource();

public: //Matrix Operations
	virtual void injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);
	inline void fillFromVector(Complex* vector, std::map<std::string, size_t>& voltageIndexMap) { m_current = vector[voltageIndexMap[m_name]]; }

public: //Setters
	inline virtual void setSupplyVoltage(Complex supplyVoltage) { m_supplyVoltage = supplyVoltage; }
	inline virtual void setInternalImpedance(Complex internalImpedance) { m_internalImpedance = internalImpedance; }
	inline void setPosNode(Node& posNode) { m_posNode = &posNode; }
	inline void setNegNode(Node& negNode) { m_negNode = &negNode; }

public: //Getters
	inline Node* getPosNode() const { return m_posNode; }
	inline Node* getNegNode() const { return m_negNode; }
	inline Complex getInternalImpedance() const { return m_internalImpedance; }
	inline Complex getCurrent() const { return m_current; }
	inline Complex getVoltageDiff() const { return m_posNode->getNodalVoltage() - m_negNode->getNodalVoltage(); }
	inline virtual Complex getSupplyVoltage() const { return m_supplyVoltage; }
	inline Complex getPowerSupplied() const { return m_current * getSupplyVoltage(); }
	inline Complex getPowerDissipated() const { return m_internalImpedance * getCurrent().getMagnitudeSqr(); }
	inline Complex getTotalPowerSupplied() const { return getPowerSupplied() - getPowerDissipated(); }

public: //Static Methods
	static inline size_t getVoltageSrcsCount() { return m_voltageSources.size(); }

	VoltageSource(const VoltageSource&) = delete;
	void operator=(const VoltageSource&) = delete;
};

#endif //_VOLTAGESOURCE_H
