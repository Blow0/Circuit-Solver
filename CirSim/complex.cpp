#include "complex.h"

//Constructors
Complex::Complex() : m_real(0.0), m_imag(0.0) {}
Complex::Complex(double real) : m_real(real), m_imag(0.0) {}
Complex::Complex(double real, double imag) : m_real(real), m_imag(imag) {}
Complex::Complex(const Complex& complex) : m_real(complex.m_real), m_imag(complex.m_imag) {}

//Methods
void Complex::invert()
{
	double mag = getMagnitude();
	m_real /= mag;
	m_imag /= -mag;
}
void Complex::normalize()
{
	double mag = getMagnitude();
	m_real /= mag;
	m_imag /= mag;
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
	return Complex(m_real + rhs.m_real, m_imag + rhs.m_imag);
}
Complex Complex::operator-(const Complex& rhs) const
{
	return Complex(m_real - rhs.m_real, m_imag - rhs.m_imag);
}
Complex Complex::operator*(const Complex& rhs) const
{
	return Complex((m_real * rhs.m_real) - (m_imag * rhs.m_imag), (m_real * rhs.m_imag) + (m_imag * rhs.m_real));
}
Complex Complex::operator/(const Complex& rhs) const
{
	return ((*this) * rhs.getComplement()) / rhs.getMagnitude();
}
void Complex::operator=(const Complex& rhs)
{
	m_real = rhs.m_real;
	m_imag = rhs.m_imag;
}