#ifndef _CCCS_H
#define _CCCS_H

#include "currentsource.h"
#include "../Node/node.h"

class Capacitor;
class Inductor;
class CurrentSource;
class VoltageSource;

class CCCS : public CurrentSource
{
private: //Members
	Element* m_controlElement;
	Complex m_currentFactor;

public: //Static Current controlled Voltage Source creation
	static CCCS* createCCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex currentFactor, Element* controlElement, Complex internalAdmittance);

private: //Constructors
	CCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex currentFactor, Element* controlElement, Complex internalAdmittance = 0);
	~CCCS();

public: //Setters
	inline void setCurrentFactor(Complex currentFactor) { m_currentFactor = currentFactor; }

private: //Blocked Setters
	inline void setSupplyCurrent(Complex supplyCurrent) { }

private: //Helpers
	void virtual inject(Complex* matrix, size_t width, double angularFrequency = 0)
	{
		size_t lastRow = width - 1;
		size_t posIdx = Node::getIndex(getposNode());
		size_t negIdx = Node::getIndex(getnegNode());
		//Inject Current
	//	matrix[posIdx * width + lastRow] += -getSupplyCurrent();
	//	matrix[negIdx * width + lastRow] +=  getSupplyCurrent();
		//Inject internal Admittance
		matrix[posIdx * width + posIdx] +=  getInternalAdmittance();
		matrix[posIdx * width + negIdx] += -getInternalAdmittance();
		matrix[negIdx * width + posIdx] += -getInternalAdmittance();
		matrix[negIdx * width + negIdx] +=  getInternalAdmittance();

		switch (m_controlElement->getType)
		{
		case ElementType::Resistor:
			Resistor* resistor = static_cast<Resistor*>(m_controlElement);
			size_t controlposIdx = Node::getIndex(resistor->getPosNode());
			size_t controlnegIdx = Node::getIndex(resistor->getNegNode());
			Complex resistorfactor = m_currentFactor * resistor->getAdmittance();
			matrix[posIdx * width + controlposIdx] += -factor;
			matrix[posIdx * width + controlnegIdx] +=  factor;
			matrix[negIdx * width + controlposIdx] +=  factor;
			matrix[negIdx * width + controlnegIdx] += -factor;
			break;
		case ElementType::Capacitor:
			Capacitor* capacitor = static_cast<Capacitor*>(m_controlElement);
			size_t controlposIdx = Node::getIndex(capacitor->getPosNode());
			size_t controlnegIdx = Node::getIndex(capacitor->getNegNode());
			Complex capacitorfactor = m_currentFactor * capacitor->getAdmittance(angularFrequency);
			matrix[posIdx * width + controlposIdx] += -capacitorfactor;
			matrix[posIdx * width + controlnegIdx] += capacitorfactor;
			matrix[negIdx * width + controlposIdx] += capacitorfactor;
			matrix[negIdx * width + controlnegIdx] += -capacitorfactor;
			break;
		case ElementType::Inductor:
			Inductor* inductor = static_cast<Inductor*>(m_controlElement);
			size_t controlposIdx = Node::getIndex(inductor->getPosNode());
			size_t controlnegIdx = Node::getIndex(inductor->getNegNode());
			Complex inductorfactor = m_currentFactor * inductor->getAdmittance(angularFrequency);
			matrix[posIdx * width + controlposIdx] += -inductorfactor;
			matrix[posIdx * width + controlnegIdx] += inductorfactor;
			matrix[negIdx * width + controlposIdx] += inductorfactor;
			matrix[negIdx * width + controlnegIdx] += -inductorfactor;
			break;
		case ElementType::CS:
			CurrentSource* currentsource = static_cast<CurrentSource*>(m_controlElement);
			Complex current = m_currentFactor * currentsource->getCurrent();
			matrix[posIdx * width + lastRow] += -current;
			matrix[negIdx * width + lastRow] +=  current;
			break;
		case ElementType::VS:
			VoltageSource* voltagesource = static_cast<VoltageSource*>(m_controlElement);
			size_t idx = voltagesource->getVoltageSourceIndex();
			idx++;
			size_t nodes = Node::getNodesCount();
			matrix[posIdx * width + nodes + idx] =  m_currentFactor;
			matrix[negIdx * width + nodes + idx] = -m_currentFactor;
			break;
		}
	}

public: //Getters
	inline Complex getSupplyCurrent() const { return m_currentFactor * getControlCurrent(); }
	inline Complex getControlCurrent() const { return 0; }

	CCCS(const CCCS&) = delete;
	void operator=(const CCCS&) = delete;
};

#endif //_CCCS_H
