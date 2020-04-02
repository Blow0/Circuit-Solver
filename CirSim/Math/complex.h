#ifndef _COMPLEX_H
#define _COMPLEX_H

#include <math.h>
#include <sstream>
#include <string>
#include <float.h>
#include <exception>
#include <stdexcept>

#define PI 3.14159265359

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
			complex.setPolar(magnitude * cos(phase), magnitude * sin(phase));
		return complex;
	}
	static Complex stringToComplex(const std::string& str)
	{
		Complex complex;
		size_t anglePos = str.find('<');
		if (anglePos == std::string::npos) //Cartesian
		{
			size_t length = str.length();
			switch (str.back())
			{
			case 'p':
			case 'n':
			case 'u':
			case 'm':
			case 'K':
			case 'M':
			case 'G':
			case 'T':
				length--;
			}

			size_t iPos = str.find('i');
			if (iPos == std::string::npos)
			{
				iPos = str.find('I');
				if (iPos == std::string::npos)
				{
					iPos = str.find('j');
					if (iPos == std::string::npos)
						iPos = str.find('J');
				}
			}

			if (iPos == std::string::npos) //Only Real
			{
				complex.setCartesian(stod(str.substr(0, length)), 0.0);
			}
			else //Complex or Imaginary
			{
				size_t plusPos = str.find('+', 1);
				size_t subPos = str.find('-', 1);

				if (plusPos != std::string::npos) //Complex
				{
					std::string real;
					std::string imag;
					if (subPos != std::string::npos)
					{
						if (plusPos < subPos)
						{
							if (iPos > plusPos)
							{
								real = str.substr(0, plusPos);
								imag = str.substr(plusPos + 1, length - plusPos - 1);
								imag.erase(iPos - plusPos - 1, 1);
							}
							else
							{
								imag = str.substr(0, plusPos);
								real = str.substr(plusPos + 1, length - plusPos - 1);
								imag.erase(iPos, 1);
							}
						}
						else
						{
							if (iPos > subPos)
							{
								real = str.substr(0, subPos);
								imag = str.substr(subPos + 1, length - subPos - 1);
								imag.erase(iPos - subPos - 1, 1);
							}
							else
							{
								imag = str.substr(0, subPos);
								real = str.substr(subPos + 1, length - subPos - 1);
								imag.erase(iPos, 1);
							}
						}
					}
					else
					{
						if (iPos > plusPos)
						{
							real = str.substr(0, plusPos);
							imag = str.substr(plusPos + 1, length - plusPos - 1);
							imag.erase(iPos - plusPos - 1, 1);
						}
						else
						{
							imag = str.substr(0, plusPos);
							real = str.substr(plusPos + 1, length - plusPos - 1);
							imag.erase(iPos, 1);
						}
					}
					complex.setCartesian(stod(real), stod(imag));
				}
				else
				{
					if (subPos != std::string::npos) //Complex
					{
						std::string real;
						std::string imag;
						if (iPos > subPos)
						{
							real = str.substr(0, subPos);
							imag = str.substr(subPos + 1, length - subPos - 1);
							imag.erase(iPos - subPos - 1, 1);
						}
						else
						{
							imag = str.substr(0, subPos);
							real = str.substr(subPos + 1, length - subPos - 1);
							imag.erase(iPos, 1);
						}
						complex.setCartesian(stod(real), stod(imag));
					}
					else //Only Imaginary
					{
						std::string imag = str.substr(0, length);
						imag.erase(iPos, 1);
						complex.setCartesian(0.0, stod(imag));
					}
				}
			}
		}
		else //Polar
		{
			std::string magSt = str.substr(0, anglePos);
			double mag = 0.0;
			if (magSt.length() > 1)
			{ 
				mag = stod(magSt.substr(0, anglePos - 1));
				switch (magSt.back())
				{
				case 'p':
					mag *= 1.0e-12;
					break;
				case 'n':
					mag *= 1.0e-9;
					break;
				case 'u':
					mag *= 1.0e-6;
					break;
				case 'm':
					mag *= 1.0e-3;
					break;
				case 'K':
					mag *= 1.0e3;
					break;
				case 'M':
					mag *= 1.0e6;
					break;
				case 'G':
					mag *= 1.0e9;
					break;
				case 'T':
					mag *= 1.0e12;
					break;
				default:
					mag = stod(magSt);
				}
			}
			else
				mag = stod(magSt);

			std::string angleSt = str.substr(anglePos + 1);
			double angle;
			size_t radPos = angleSt.find("rad");
			size_t degPos = angleSt.find("deg");
			if (radPos != std::string::npos)
			{
				if (degPos != std::string::npos) //first
				{
					if (degPos < radPos)
						angle = stod(angleSt.substr(0, degPos)) * (PI / 180.0);
					else
						angle = stod(angleSt.substr(0, radPos));
				}
				else //rad
					angle = stod(angleSt.substr(0, radPos));
			}
			else
			{
				if (degPos != std::string::npos) //deg
					angle = stod(angleSt.substr(0, degPos)) * (PI / 180.0);
				else //default - deg
					angle = stod(angleSt.substr(0, degPos)) * (PI / 180.0);
			}

			complex.setPolar(mag, angle);
		}

		return complex;
	}
	static std::string complexToString(Complex complex)
	{
		double real = complex.getReal();
		double imag = complex.getImag();
		double mag = complex.getMagnitude();

		std::ostringstream stream;
		stream.precision(2);
		if (mag >= DBL_EPSILON)
		{
			int power = ((int)log10(mag)) / 3;
			switch (power)
			{
			case -4:
				real *= 1.0e12;
				imag *= 1.0e12;
				break;
			case -3:
				real *= 1.0e9;
				imag *= 1.0e9;
				break;
			case -2:
				real *= 1.0e6;
				imag *= 1.0e6;
				break;
			case -1:
				real *= 1.0e3;
				imag *= 1.0e3;
				break;
			case 1:
				real *= 1.0e-3;
				imag *= 1.0e-3;
				break;
			case 2:
				real *= 1.0e-6;
				imag *= 1.0e-6;
				break;
			case 3:
				real *= 1.0e-9;
				imag *= 1.0e-9;
				break;
			case 4:
				real *= 1.0e-12;
				imag *= 1.0e-12;
				break;
			}

			if (abs(real) >= DBL_EPSILON)
			{
				stream << std::fixed << real;
				if (abs(imag) >= DBL_EPSILON)
				{
					stream << (imag < 0.0 ? "-" : "+");
					stream << std::fixed << abs(imag);
					stream << "i";
				}
			}
			else
			{
				if (abs(imag) >= DBL_EPSILON)
				{
					stream << std::fixed << imag;
					stream << "i";
				}
			}

			switch (power)
			{
			case -4:
				stream << "p";
				break;
			case -3:
				stream << "n";
				break;
			case -2:
				stream << "u";
				break;
			case -1:
				stream << "m";
				break;
			case 1:
				stream << "K";
				break;
			case 2:
				stream << "M";
				break;
			case 3:
				stream << "G";
				break;
			case 4:
				stream << "T";
				break;
			}
		}
		else
			stream << std::fixed << 0.0;

		return stream.str();
	}
	static std::string complexToPolarString(Complex complex, bool deg)
	{
		double mag = complex.getMagnitude();
		double phase = complex.getPhase();

		std::ostringstream stream;
		stream.precision(2);
		if (mag >= DBL_EPSILON)
		{
			switch (((int)log10(mag)) / 3)
			{
			case -4:
				stream << std::fixed << mag * 1.0e12;
				stream << "p";
				break;
			case -3:
				stream << std::fixed << mag * 1.0e9;
				stream << "n";
				break;
			case -2:
				stream << std::fixed << mag * 1.0e6;
				stream << "u";
				break;
			case -1:
				stream << std::fixed << mag * 1.0e3;
				stream << "m";
				break;
			case 0:
				stream << std::fixed << mag;
				break;
			case 1:
				stream << std::fixed << mag * 1.0e-3;
				stream << "K";
				break;
			case 2:
				stream << std::fixed << mag * 1.0e-6;
				stream << "M";
				break;
			case 3:
				stream << std::fixed << mag * 1.0e-9;
				stream << "G";
				break;
			case 4:
				stream << std::fixed << mag * 1.0e-12;
				stream << "T";
				break;
			}
			stream << "<";
			stream << std::fixed << (phase * (deg ? (180.0 / PI) : 1.0));
			stream << (deg ? "deg" : "rad");
		}
		else
			stream << std::fixed << 0.0;
		return stream.str();
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
	inline Complex getComponentWiseAbs()	const { return Complex(abs(m_real), abs(m_imag)); }

public: //Methods
	inline void invert();
	inline void normalize();
	inline void complement();
	inline void componentWiseAbs();

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
void Complex::componentWiseAbs()
{
	m_real = abs(m_real);
	m_imag = abs(m_imag);
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