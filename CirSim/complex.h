#ifndef COMPLEX_H
#define COMPLEX_H

#include<cmath>


class Complex
{
private: //Memebers

	double m_real;
	double m_img;
	double m_mag;
	float  m_phase;

public:
	//Constructors
	Complex() : m_real(0), m_img(0), m_mag(0), m_phase(0) {}
	Complex(const double& real, const double& img) : m_real(real), m_img(img)
	{
		toPolar();
	}
	Complex(const double& mag, const float& phase) : m_mag(mag), m_phase(phase)
	{
		toCartesian();
	}
	//Copy constructor
	Complex(const Complex& n) : m_real(n.m_real), m_img(n.m_img), m_mag(n.m_mag), m_phase(n.m_phase) {}


	//Setters
	void setComplex(const double& real, const double& img, const double& mag, const float& phase)
	{
		setCartesian(real, img);
	}
	void setCartesian(const double& real, const double& img) { setReal(real); setImg(img); toPolar(); }
	void setReal(const double& real) { m_real = real; }
	void setImg(const double& img)   { m_img = img; }
	void setPolar(const double& mag, const float& phase) { setMagnitude(mag); setPhase(phase); toCartesian(); }
	void setMagnitude(const double& mag) { m_mag = mag; }
	void setPhase(const float& phase) { m_phase = phase; }

	//Getters
	double getReal() const { return m_real; }
	double getImg()  const	 { return m_img; }
	double getMagnitude() const{ return m_mag; }
	double getPhase() const	 { return m_phase; }


public:
	//Helpers
	void toPolar();
	void toCartesian();
	Complex toComplex(char elementType, double elementVal, unsigned long omega);
	Complex toComplex(char elementType, double elementVal, float phase);

	//Operator Overloading
	void operator + (const Complex& n)
	{
		this->m_real += n.m_real;
		this->m_img  += n.m_img;
	}
	void operator - (const Complex& n)
	{
		this->m_real -= n.m_real;
		this->m_img  -= n.m_img;
	}
	Complex operator * (const Complex& n)
	{
		Complex n1;
		n1.m_real = (this->m_real * n.m_real) - (this->m_img * n.m_img);
		n1.m_img  = (this->m_real * n.m_img) + (this->m_img * n.m_real);

		return n1;
	}

};





#endif