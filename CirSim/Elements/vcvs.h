#ifndef _VCVS_H
#define _VCVS_H

#include "element.h"
#include "../Node/node.h"

class VCVS : public Element
{
private: //Members
	Node* m_controlPosNode;
	Node* m_controlNegNode;
	Node* m_posNode;
	Node* m_negNode;

	Complex m_voltage; 
	Complex m_current;//Will be calculated
	//Controlling element
	double m_factor;
	Complex m_controlVoltage;

public: //Static Current controlled Voltage Source creation
	static VCVS* createVCVS(const std::string& ccvsName, Node& posNode, Node& negNode, double factor, Complex controlVoltage);
	static VCVS* createVCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Complex controlVoltage);
private: //Constructors
	VCVS(const std::string& ccvsName, Node& posNode, Node& negNode, double factor, Complex controlVoltage);
	VCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Complex controlVoltage);
	~VCVS();

public: //Setters
	inline void setCurrent(Complex current) { m_current = current; }

public: //Getters
	inline Node* getposNode() const { return m_posNode; }
	inline Node* getnegNode() const { return m_negNode; }
	inline Complex getCurrent() const { return m_current; }
	inline Complex getVoltage() const { return m_voltage; }
	inline double getPowerDelivered() const { return m_current.getMagnitude() * m_voltage.getMagnitude(); }
};

#endif // !_VCVS_H