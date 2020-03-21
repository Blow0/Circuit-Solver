#ifndef ELEMENT_H
#define ELEMENT_H

#include<iostream>
#include "complex.h"

using namespace std;

class Element : public Complex
{
private:
	//Members
	string m_label;
	unsigned int m_posNode;
	unsigned int m_negNode;
	float m_current;
	float m_voltageDiff;

public:
	//Constructors
	Element() : m_label(" "), m_posNode(0), m_negNode(0), m_current(0), m_voltageDiff(0) {}
	Element(const string& label, const unsigned int& posNode, const unsigned int& negNode, const float& current, const float& vDiff)
		: m_label(label), m_posNode(posNode), m_negNode(negNode), m_current(current), m_voltageDiff(vDiff) {}

	//Setters
	void setLabel(const string& label) { m_label = label; }
	void setNodes(const unsigned int& posNode, const unsigned int& negNode) { setPosNode(posNode); setNegNode(negNode); }
	void setPosNode(const unsigned int& posNode) { m_posNode = posNode; }
	void setNegNode(const unsigned int& negNode) { m_negNode = negNode; }
	void setCurrent(const float& current) { m_current = current; }
	void setVoltageDiff(const float& vDiff) { m_voltageDiff = vDiff; }

	//Getters
	string getLabel() const { return m_label; }
	unsigned int getPosNode() const { return m_posNode; }
	unsigned int getNegNode() const { return m_negNode; }
	float getcurrent() const { return m_current; }
	float getVoltageDiff() const { return m_voltageDiff; }
	
	//Helpers
	virtual bool isSeries(const Element& element1) = 0;
	virtual bool isParallel(const Element& element1, const Element& element2) = 0;
	virtual void calcSeries(const bool& isSeries, const Element& element1) = 0;
	virtual void calcParallel(const bool& isParallel, const Element& element1) = 0;
	virtual void printElement() = 0;
};

class Resistor : public Element
{
private:
	double m_val;

public: 
	//Constructors
	Resistor() : m_val(0) {}
	Resistor(const string& label,const unsigned int& posNode, const unsigned int& negNode, const double& val):
		m_val(val) 
	{
		setLabel(label);
		setNodes(posNode, negNode);
		setComplex(val, 0, val, 0);
	}

	//Setters
	void setAttributes(const string& label, const unsigned int& posNode, const unsigned int& negNode, const double& val)
	{
		setLabel(label);
		setNodes(posNode, negNode);
		setVal(val);
	}
	void setVal(const double& val) { m_val = val; setComplex(val, 0, val, 0); }

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
	Capacitor() : m_val(0), m_freq(1) {}
	Capacitor(const string& label, const unsigned int& posNode, const unsigned int& negNode, const double& val, const unsigned long& freq = 1)
		: m_freq(freq)
	{
		m_val = 1 / (val * freq);
		setLabel(label);
		setNodes(posNode, negNode);
		setComplex(0, -m_val, m_val, -90.0f);
	}

	//Setters
	void setAttributes(const string& label, const unsigned int& posNode, const unsigned int& negNode, const double& val, const unsigned long& freq)
	{
		setLabel(label);
		setNodes(posNode, negNode);
		setFreq(freq);
		setVal(val);
	}
	void setVal(const double& val) { m_val = 1/(val* m_freq); setComplex(0, -m_val, m_val, -90.0f); }
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
	Inductor() : m_val(0), m_freq(1) {}
	Inductor(const string& label, const unsigned int& posNode, const unsigned int& negNode, const double& val, const unsigned long& freq = 1)
		: m_freq(freq)
	{
		m_val = val * freq;
		setLabel(label);
		setNodes(posNode, negNode);
		setComplex(0, m_val, m_val, 90.0f);
	}

	//Setters
	void setAttributes(const string& label, const unsigned int& posNode, const unsigned int& negNode, const double& val, const unsigned long& freq)
	{
		setLabel(label);
		setNodes(posNode, negNode);
		setFreq(freq);
		setVal(val);
	}
	void setVal(const double& val) { m_val = val * m_freq; setComplex(0, m_val, m_val, 90.0f); }
	void setFreq(const unsigned long& freq) { m_freq = freq; }
	//Getters
	double getVal() const { return m_val; }
};

class Sources : public Element 
{
private:
	double m_val;

public:
	//Constructors
	Sources() : m_val(0) { setPhase(0); }
	Sources(const string& label, const unsigned int& posNode, const unsigned int& negNode, const double& val, const float& phase)
		: m_val(val)
	{
		setLabel(label);
		setNodes(posNode, negNode);
		setPolar(val, phase);
	}

	//Setters
	void setAttributes(const string& label, const unsigned int& posNode, const unsigned int& negNode, const double& val, const float& phase)
	{
		setLabel(label);
		setNodes(posNode, negNode);
		setPhase(phase);
		setPolar(val, phase);
		setVal(val);
	}
	void setVal(const double& val) { m_val = val; }

	//Getters
	double getVal() const { return m_val; }

	//Helpers
	void printElement()
	{
		cout << getMagnitude() <<"<"<<getPhase();
	}
};
#endif
