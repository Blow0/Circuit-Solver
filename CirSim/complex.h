#ifndef _COMPLEX_H
#define _COMPLEX_H

#include <math.h>

class Complex
{
private: //Memebers
	double m_real;
	double m_imag;

public: //Constructors
	Complex();
	Complex(double real);
	Complex(double real, double imag);
	Complex(const Complex& complex);

public: //Setters
	inline void setCartesian(double real, double imag) { m_real = real; m_imag = imag; }
	inline void setPolar(double magnitude, double phase) { m_real = magnitude * cos(phase); m_imag = magnitude * sin(phase); }
	inline void setReal(double real) { m_real = real; }
	inline void setImag(double imag) { m_imag = imag; }
	inline void setMagnitude(double magnitude) { double ratio = magnitude / getMagnitude(); m_real *= ratio; m_imag *= ratio; }
	inline void setPhase(double phase) { double mag = getMagnitude(); m_real = mag * cos(phase); m_imag = mag * sin(phase); }

public: //Getters
	inline double getReal() const { return m_real; }
	inline double getImg() const { return m_imag; }
	inline double getMagnitude() const { return sqrt(m_real * m_real + m_imag * m_imag); }
	inline double getMagnitudeSqr() const { return m_real * m_real + m_imag * m_imag; }
	inline double getPhase() const { return atan(m_imag / m_real); }
	inline Complex getNormalized() const { return Complex(m_real, m_imag) / getMagnitude(); }
	inline Complex getComplement() const { return Complex(m_real, -m_imag); }
	inline Complex getInverse() const { return getComplement().getNormalized(); }

public: //Methods
	inline void invert();
	inline void normalize();
	inline void complement();

public: //Operations
	inline Complex operator-() const;
	inline Complex operator+(const Complex& rhs) const;
	inline Complex operator-(const Complex& rhs) const;
	inline Complex operator*(const Complex& rhs) const;
	inline Complex operator/(const Complex& rhs) const;
	inline void operator=(const Complex& rhs);
};

#endif //_COMPLEX_H