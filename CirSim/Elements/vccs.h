#ifndef _VCCS_H
#define _VCCS_H

#include "element.h"
#include "../Node/node.h"

class VCCS : public Element
{
private: //Members
	Node* m_controlPosNode;
	Node* m_controlNegNode;
	Node* m_posNode;
	Node* m_negNode;

	Complex m_current;
	Complex m_voltage; //Will be calculated
	//Controlling element
	double m_factor;
	Complex m_controlVoltage;

public: //Static Current controlled Voltage Source creation
	static VCCS* createVCCS(const std::string& ccvsName, Node& posNode, Node& negNode, double factor, Complex controlVoltage);
	static VCCS* createVCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Complex controlVoltage);
private: //Constructors
	VCCS(const std::string& ccvsName, Node& posNode, Node& negNode, double factor, Complex controlVoltage);
	VCCS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, Complex controlVoltage);
	~VCCS();

public: //Setters
	inline void setCurrent(Complex current) { m_current = current; }

public: //Getters
	inline Node* getposNode() const { return m_posNode; }
	inline Node* getnegNode() const { return m_negNode; }
	inline Complex getCurrent() const { return m_current; }
	inline Complex getVoltage() const { return m_posNode->getNodalVoltage() - m_negNode->getNodalVoltage(); }
	inline double getPowerDelivered() const { return m_current.getMagnitude() * m_voltage.getMagnitude(); }
};

#endif