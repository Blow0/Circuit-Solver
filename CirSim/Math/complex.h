#ifndef _COMPLEX_H
#define _COMPLEX_H

#include <math.h>
#include <string>
#include <float.h>
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

public: //Creators
	static Complex polarToComplex(double magnitude, double phase)
	{
		Complex complex;
		double magAbs = abs(magnitude);
		if (magAbs >= DBL_EPSILON)
			complex.setPolar(magAbs * cos(phase), magAbs * sin(phase));
		return complex;
	}
	static Complex stringToComplex(const std::string& str)
	{
		Complex complex;
		if (str[0] == '[') //Polar
		{
			size_t commaPos = str.find(',');
			if (commaPos != std::string::npos)
				complex.setPolar(std::stod(str.substr(1, commaPos - 1)), std::stod(str.substr(commaPos + 1, str.length() - commaPos - 2)));
			else
				complex.setPolar(std::stod(str.substr(1, str.length() - 2)), 0);
		}
		else //Cartesian
		{
			size_t plusPos = str.find('+');
			size_t subPos = str.find('-');
			if (plusPos != std::string::npos)
				complex.setCartesian(std::stod(str.substr(0, plusPos)), std::stod(str.substr(plusPos + 1, str.length() - plusPos - 1)));
			else
			{
				if (subPos != std::string::npos)
					complex.setCartesian(std::stod(str.substr(0, subPos)), std::stod(str.substr(subPos + 1, str.length() - subPos - 1)));
				else
					complex.setCartesian(std::stod(str), 0);
			}
		}
		return complex;
	}

public: //Setters
	inline void setCartesian(double real, double imag)		 { m_real = real; m_imag = imag; }
	inline void setReal		(double real)					 { m_real = real; }
	inline void setImag		(double imag)					 { m_imag = imag; }
	inline void setPolar	(double magnitude, double phase) { m_real = magnitude * cos(phase); m_imag = magnitude * sin(phase); }
	inline void setMagnitude(double magnitude)				 { double mag = getMagnitude(); if (mag >= DBL_EPSILON) { double ratio = magnitude / mag; m_real *= ratio; m_imag *= ratio; } }
	inline void setPhase	(double phase)					 { double mag = getMagnitude(); if (mag >= DBL_EPSILON) { m_real = mag * cos(phase); m_imag = mag * sin(phase); } }

public: //Getters
	inline double  getReal()		 const { return m_real; }
	inline double  getImag()		 const { return m_imag; }
	inline double  getMagnitude()	 const { return sqrt(getMagnitudeSqr()); }
	inline double  getMagnitudeSqr() const { return m_real * m_real + m_imag * m_imag; }
	inline double  getPhase()		 const { return atan2(m_imag, m_real); }
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
	inline Complex operator= (const Complex& rhs);
	inline Complex operator+=(const Complex& rhs);
	inline Complex operator-=(const Complex& rhs);
	inline Complex operator*=(const Complex& rhs);
	inline Complex operator*=(double rhs);
	inline Complex operator/=(const Complex& rhs);
	inline Complex operator/=(double rhs);
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
	double rhsMagSqr = rhs.getMagnitudeSqr();
	if (rhsMagSqr >= DBL_EPSILON)
		return ((*this) * rhs.getComplement()) / rhsMagSqr;
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
Complex Complex::operator=(const Complex& rhs)
{
	m_real = rhs.m_real;
	m_imag = rhs.m_imag;
	return (*this);
}
Complex Complex::operator+=(const Complex& rhs)
{
	m_real += rhs.m_real;
	m_imag += rhs.m_imag;
	return (*this);
}
Complex Complex::operator-=(const Complex& rhs)
{
	m_real -= rhs.m_real;
	m_imag -= rhs.m_imag;
	return (*this);
}
Complex Complex::operator*=(const Complex& rhs)
{
	m_real = (m_real * rhs.m_real) - (m_imag * rhs.m_imag);
	m_imag = (m_real * rhs.m_imag) + (m_imag * rhs.m_real);
	return (*this);
}
Complex Complex::operator*=(double rhs)
{
	m_real *= rhs;
	m_imag *= rhs;
	return (*this);
}
Complex Complex::operator/=(const Complex& rhs)
{
	double rhsMagSqr = rhs.getMagnitudeSqr();
	if (rhsMagSqr >= DBL_EPSILON)
		(*this) *= rhs.getComplement() / rhsMagSqr;
	else
		throw std::runtime_error("Can't divide by Zero.");
	return (*this);
}
Complex Complex::operator/=(double rhs)
{
	if (rhs >= DBL_EPSILON)
	{
		m_real /= rhs;
		m_imag /= rhs;
	}
	else
		throw std::runtime_error("Can't divide by Zero.");
	return (*this);
}

#endif //_COMPLEX_H