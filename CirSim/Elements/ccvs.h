#ifndef _CCVS_H
#define _CCVS_H

#include "element.h"
#include "currentcontrolled.h"
#include "../Node/node.h"

class CCVS : public Element, public CurrentControlledSource
{
private: //Members
	Node* m_controlPosNode;
	Node* m_controlNegNode;
	Node* m_posNode;
	Node* m_negNode;
	
	Complex m_voltage;
	Complex m_current; //Will be calculated
	//Controlling element
	double m_factor;
	Complex m_controlCurrent;

public: //Static Current controlled Voltage Source creation
	static CCVS* createCCVS(const std::string& ccvsName, Node& posNode, Node& negNode, double factor, double controlCurrent, double phase);
	static CCVS* createCCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, double controlCurrent, double phase);
private: //Constructors
	CCVS(const std::string& ccvsName, Node& posNode, Node& negNode, double factor, double controlCurrent, double phase);
	CCVS(const std::string& ccvsName, Node& posNode, Node& negNode, Complex factor, double controlCurrent, double phase);
	~CCVS();

public: //Setters
	inline void setFactor(Complex factor) { m_voltage = factor * m_controlCurrent; }
	inline void setCurrent(Complex current) { m_current = current; }

public: //Getters
	inline Node* getposNode() const { return m_posNode; }
	inline Node* getnegNode() const { return m_negNode; }
	inline Complex getCurrent() const { return m_current; }
	inline Complex getVoltage() const { return m_voltage; }
};

#endif // !_CCVS_H
