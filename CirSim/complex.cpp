#include "complex.h"
#include<cmath>

//Constructors
Complex::Complex() : m_real(0), m_imaginary(0) {}
Complex::Complex(double real, double img) : m_real(real), m_imaginary(img)
{

}

//Copy constructor
Complex::Complex(const Complex& n) : m_real(n.m_real), m_imaginary(n.m_imaginary) {}

//Setters
inline void Complex::setCartesian(double mag, float phase)
{
	setReal(mag * cos(phase));
	setImg(mag * sin(phase));
}

//Operator Overloading
void Complex::operator + (Complex n)
{
	this->m_real += n.m_real;
	this->m_imaginary += n.m_imaginary;
}
void  Complex::operator - (Complex n)
{
	this->m_real -= n.m_real;
	this->m_imaginary -= n.m_imaginary;
}
Complex  Complex::operator * (Complex n)
{
	Complex n1;
	n1.m_real = (this->m_real * n.m_real) - (this->m_imaginary * n.m_imaginary);
	n1.m_imaginary = (this->m_real * n.m_imaginary) + (this->m_imaginary * n.m_real);

	return n1;
}