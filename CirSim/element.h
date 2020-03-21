#ifndef ELEMENT_H
#define ELEMENT_H

#include<iostream>
#include "complex.h"


class Element : public Complex
{
private:
	//Members
	std::string m_label;
	unsigned int m_posNode;
	unsigned int m_negNode;
	float m_current;
	float m_voltageDiff;

public:
	//Constructors
	Element();
	Element(std::string label, unsigned int posNode, unsigned int negNode);

	//Setters
	inline void setLabel(std::string label) { m_label = label; }
	inline void setNodes(unsigned int posNode, unsigned int negNode) { setPosNode(posNode); setNegNode(negNode); }
	inline void setPosNode(unsigned int posNode) { m_posNode = posNode; }
	inline void setNegNode(unsigned int negNode) { m_negNode = negNode; }
	inline void setCurrent(float current) { m_current = current; }
	inline void setVoltageDiff(float vDiff) { m_voltageDiff = vDiff; }

	//Getters
	std::string getLabel() const { return m_label; }
	unsigned int getPosNode() const { return m_posNode; }
	unsigned int getNegNode() const { return m_negNode; }
	float getcurrent() const { return m_current; }
	float getVoltageDiff() const { return m_voltageDiff; }
	
	//Helpers
	virtual bool isSeries(const Element& element1) = 0;
	virtual bool isParallel(const Element& element1, const Element& element2) = 0;
	virtual void calcSeries(bool isSeries, const Element& element1) = 0;
	virtual void calcParallel( bool isParallel, const Element& element1) = 0;
};

class Resistor : public Element
{
private:
	double m_val;

public: 
	//Constructors
	Resistor();
	Resistor(std::string label, unsigned int posNode, unsigned int negNode, double val);

	//Setters
	void setAttributes(std::string label, unsigned int posNode, unsigned int negNode, double val, unsigned long freq = 1)
	{
		setLabel(label);
		setNodes(posNode, negNode);
		setVal(val);
	}
	void setVal(const double& val) { m_val = val; setComplex(val, 0); }

	//Getters
	double getVal() const { return m_val; }

};

class Capacitor : public Element
{
private:
	double m_val;
	unsigned long m_freq;
public:
	//Constructors
	Capacitor();
	Capacitor(std::string label, unsigned int posNode, unsigned int negNode, double val, unsigned long freq = 1);

	//Setters
	void setAttributes(std::string label, unsigned int posNode, unsigned int negNode, double val, unsigned long freq = 1)
	{
		setLabel(label);
		setNodes(posNode, negNode);
		setFreq(freq);
		setVal(val);
	}
	void setVal(const double& val) { m_val = 1/(val* m_freq); setComplex(0, -m_val); }
	void setFreq(const unsigned long& freq) { m_freq = freq; }
	//Getters
	double getVal() const { return m_val; }
};

class Inductor : public Element
{
private:
	double m_val;
	unsigned long m_freq;
public:
	//Constructors
	Inductor();
	Inductor(std::string label, unsigned int posNode, unsigned int negNode, double val, unsigned long freq = 1);

	//Setters
	void setAttributes(std::string label, unsigned int posNode, unsigned int negNode, double val, unsigned long freq = 1)
	{
		setLabel(label);
		setNodes(posNode, negNode);
		setFreq(freq);
		setVal(val);
	}
	void setVal(const double& val) { m_val = val * m_freq; setComplex(0, m_val); }
	void setFreq(const unsigned long& freq) { m_freq = freq; }
	//Getters
	double getVal() const { return m_val; }
};

class Source : public Element 
{
private:
	double m_val;

public:
	//Constructors
	Source();
	Source(std::string label, unsigned int posNode, unsigned int negNode, double val, float phase);

	//Setters
	void setAttributes(std::string label, unsigned int posNode, unsigned int negNode, double val, float phase)
	{
		setLabel(label);
		setNodes(posNode, negNode);
		setCartesian(val, phase);
		setVal(val);
	}
	void setVal(const double& val) { m_val = val; }

	//Getters
	double getVal() const { return m_val; }

};

class VoltageSource : public Source
{
	static unsigned int m_voltagSourcesCount;

public:
	//Constructor
	VoltageSource();
	VoltageSource(std::string label, unsigned int posNode, unsigned int negNode, double val, float phase);

	//Getters
	static unsigned int getVoltageSrcCount() { return m_voltagSourcesCount; }
};

class CurrentCSource : public Source
{
	static unsigned int m_currentControlledCount;

public:
	//Constructor
	CurrentCSource();
	CurrentCSource(std::string label, unsigned int posNode, unsigned int negNode, double val, float phase);

	//Getters
	static unsigned int getCurrentContrlCount() { return m_currentControlledCount; }

};
#endif
