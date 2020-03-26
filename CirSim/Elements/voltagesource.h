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
	Complex m_voltage;
	Complex m_current; //Will be calculated


private: //Static Members
	static std::list <VoltageSource*> m_voltageSources;

public: //Static Voltage Source creation
	static VoltageSource* createVoltageSource(const std::string& voltageSrcName, Node& posNode, Node& negNode, Complex voltage);

private: //Constructors
	VoltageSource(const std::string& voltageSrcName, Node& posNode, Node& negNode, Complex voltage);
	~VoltageSource();

public: //Setters
	inline void setVoltage(double voltage, double phase) { m_voltage = Complex(voltage*cos(phase), voltage*sin(phase)); }
	inline void setCurrent(Complex current) { m_current = current; }

public: //Getters
	inline Node* getposNode() const { return m_posNode; }
	inline Node* getnegNode() const { return m_negNode; }
	inline Complex getVoltage() const { return m_voltage; }
	inline Complex getCurrent() const { return m_current; }
	inline double getPowerDelivered() const { return m_current.getMagnitude() * m_voltage.getMagnitude(); }

public: //Static Methods
	static inline unsigned int getVoltageSrcsCount() { return m_voltageSources.size(); }

};

#endif // !_VOLTAGESOURCE_H
