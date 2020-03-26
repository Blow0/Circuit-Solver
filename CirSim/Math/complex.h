#ifndef _COMPLEX_H
#define _COMPLEX_H

#include <math.h>
#include <exception>
#include <stdexcept>

class Complex
{
private: //Memebers
	double m_real;
	double m_imag;

public: //Constructors
	Complex()							: m_real(0.0)			, m_imag(0.0) {}
	Complex(double real)				: m_real(real)			, m_imag(0.0) {}
	Complex(double real, double imag)	: m_real(real)			, m_imag(imag) {}
	Complex(const Complex& complex)		: m_real(complex.m_real), m_imag(complex.m_imag) {}

public: //Setters
	inline void setCartesian(double real, double imag)		 { m_real = real; m_imag = imag; }
	inline void setPolar	(double magnitude, double phase) { m_real = magnitude * cos(phase); m_imag = magnitude * sin(phase); }
	inline void setReal		(double real)					 { m_real = real; }
	inline void setImag		(double imag)					 { m_imag = imag; }
	inline void setMagnitude(double magnitude)				 { double mag = getMagnitude(); if (mag >= DBL_EPSILON) { double ratio = magnitude / mag; m_real *= ratio; m_imag *= ratio; } }
	inline void setPhase	(double phase)					 { double mag = getMagnitude(); if (mag >= DBL_EPSILON) { m_real = mag * cos(phase); m_imag = mag * sin(phase); } }

public: //Getters
	inline double  getReal()		 const { return m_real; }
	inline double  getImag()		 const { return m_imag; }
	inline double  getMagnitude()	 const { return sqrt(getMagnitudeSqr()); }
	inline double  getMagnitudeSqr() const { return m_real * m_real + m_imag * m_imag; }
	inline double  getPhase()		 const { return atan(m_imag / m_real); }
	inline Complex getComplement()	 const { return Complex(m_real, -m_imag); }
	inline Complex getNormalized()	 const { double mag = getMagnitude(); return Complex(m_real, m_imag) / (mag >= DBL_EPSILON ? mag : 1.0); }
	inline Complex getInverse()		 const { return getComplement().getNormalized(); }

public: //Methods
	inline void invert();
	inline void normalize();
	inline void complement();

public: //Logic
	inline bool operator!=(const Complex& rhs) const { return (rhs.m_real != m_real || rhs.m_imag != m_imag); }
	inline bool operator==(const Complex& rhs) const { return (rhs.m_real == m_real && rhs.m_imag == m_imag); }
	inline bool operator> (const Complex& rhs) const { return (rhs.getMagnitude() > getMagnitude());  }
	inline bool operator< (const Complex& rhs) const { return (rhs.getMagnitude() < getMagnitude());  }
	inline bool operator>=(const Complex& rhs) const { return (rhs.getMagnitude() >= getMagnitude()); }
	inline bool operator<=(const Complex& rhs) const { return (rhs.getMagnitude() <= getMagnitude()); }

public: //Operations
	inline Complex operator- () const;
	inline Complex operator+ (const Complex& rhs) const;
	inline Complex operator- (const Complex& rhs) const;
	inline Complex operator* (const Complex& rhs) const;
	inline Complex operator* (double rhs) const;
	inline Complex operator/ (const Complex& rhs) const;
	inline Complex operator/ (double rhs) const;
	inline void	   operator= (const Complex& rhs);
	inline void	   operator+=(const Complex& rhs);
	inline void	   operator-=(const Complex& rhs);
	inline void	   operator*=(const Complex& rhs);
	inline void	   operator*=(double rhs);
	inline void	   operator/=(const Complex& rhs);
	inline void	   operator/=(double rhs);
};

//Methods
void Complex::invert()
{
	complement();
	normalize();
}
void Complex::normalize()
{
	double mag = getMagnitude();
	if (mag >= DBL_EPSILON)
	{
		m_real /= mag;
		m_imag /= mag;
	}
}
void Complex::complement()
{
	m_imag = -m_imag;
}

//Operations
Complex Complex::operator-() const
{
	return Complex(-m_real, -m_imag);
}
Complex Complex::operator+(const Complex& rhs) const
{
	return Complex(getReal() + rhs.getReal()
		, getImag() + rhs.getImag());
}
Complex Complex::operator-(const Complex& rhs) const
{
	return Complex(m_real - rhs.m_real, m_imag - rhs.m_imag);
}
Complex Complex::operator*(const Complex& rhs) const
{
	return Complex((m_real * rhs.m_real) - (m_imag * rhs.m_imag)
		, (m_real * rhs.m_imag) + (m_imag * rhs.m_real));
}
Complex Complex::operator*(double rhs) const
{
	return Complex(m_real * rhs, m_imag * rhs);
}
Complex Complex::operator/(const Complex& rhs) const
{
	double rhsMag = rhs.getMagnitude();
	if (rhsMag >= DBL_EPSILON)
		return ((*this) * rhs.getComplement()) / rhsMag;
	else
		throw std::runtime_error("Can't divide by Zero.");
}
Complex Complex::operator/(double rhs) const
{
	if (rhs >= DBL_EPSILON)
		return Complex(m_real, m_imag) * (1.0 / rhs);
	else
		throw std::runtime_error("Can't divide by Zero.");
}
void Complex::operator=(const Complex& rhs)
{
	m_real = rhs.m_real;
	m_imag = rhs.m_imag;
}
void Complex::operator+=(const Complex& rhs)
{
	m_real += rhs.m_real;
	m_imag += rhs.m_imag;
}
void Complex::operator-=(const Complex& rhs)
{
	m_real -= rhs.m_real;
	m_imag -= rhs.m_imag;
}
void Complex::operator*=(const Complex& rhs)
{
	m_real = (m_real * rhs.m_real) - (m_imag * rhs.m_imag);
	m_imag = (m_real * rhs.m_imag) + (m_imag * rhs.m_real);
}
void Complex::operator*=(double rhs)
{
	m_real *= rhs;
	m_imag *= rhs;
}
void Complex::operator/=(const Complex& rhs)
{
	double rhsMag = rhs.getMagnitude();
	if (rhsMag >= DBL_EPSILON)
		(*this) *= rhs.getComplement() / rhs.getMagnitude();
	else
		throw std::runtime_error("Can't divide by Zero.");
}
void Complex::operator/=(double rhs)
{
	if (rhs >= DBL_EPSILON)
	{
		m_real /= rhs;
		m_imag /= rhs;
	}
	else
		throw std::runtime_error("Can't divide by Zero.");
}

#endif //_COMPLEX_H