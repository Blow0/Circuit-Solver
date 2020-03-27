#ifndef _CCCS_H
#define _CCCS_H

#include "currentsource.h"
#include "currentcontrolled.h"
#include "../Node/node.h"

class CCCS : public CurrentSource, public CurrentControlledSource
{
private: //Members
	//Controlling element
	Complex m_currentFactor;
	Element* m_controlCurrent;

public: //Static Current controlled Voltage Source creation
	static CCCS* createCCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex factor, Element* controlCurrent, Complex internalAdmittance);
private: //Constructors
	CCCS(const std::string& cccsName, Node& posNode, Node& negNode, Complex factor, Element* controlCurrent, Complex internalAdmittance = 0);
	~CCCS();

public: //Setters
	inline void setFactor(Complex factor) { m_currentFactor = factor; }
	inline void setSupplyCurrent(Complex supplyCurrent) { }

public: //Getters
	inline Complex getSupplyCurrent() const { return m_supplyCurrent; } //To be edited currentFactor * ElementCurrent

	CCCS(const CCCS&) = delete;
	void operator=(const CCCS&) = delete;
};

#endif // !_CCCS_H
