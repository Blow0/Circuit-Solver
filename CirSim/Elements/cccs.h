#ifndef _CCCS_H
#define _CCCS_H

#include "currentsource.h"
#include "../Node/node.h"

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

public: //Getters
	inline Complex getSupplyCurrent() const { return m_currentFactor * getControlCurrent(); }
	inline Complex getControlCurrent() const { return 0; }

	CCCS(const CCCS&) = delete;
	void operator=(const CCCS&) = delete;
};

#endif //_CCCS_H
