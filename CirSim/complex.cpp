#include "complex.h"

void Complex::toPolar()
{
	m_mag = sqrt(m_real * m_real + m_img * m_img);
	m_phase = (float) tan(m_img / m_real);
}

void Complex::toCartesian()
{
	m_real = m_mag * cos(m_phase);
	m_img  = m_mag * cos(m_phase);
}

Complex Complex::toComplex(char elementType, double elementVal, unsigned long omega = 1)
{
	Complex cmplx;
	elementVal = omega * elementVal;

	switch (elementType)
	{
	case 'R':
	case 'r':
		cmplx.setComplex(elementVal, 0, elementVal, 0);
		break;
	case 'L':
	case 'l':
		cmplx.setComplex(0, elementVal, elementVal, 90.0f);
		break;
	case 'C':
	case 'c':
		elementVal = 1 / elementVal;
		cmplx.setComplex(0, elementVal, elementVal, -90.0f);
		break;
	}

	return cmplx;
}

Complex Complex::toComplex(char elementType, double elementVal, float phase)
{
	Complex cmplx;
	switch (elementType)
	{
	case 'I':
	case 'i':
	case 'V':
	case 'v':
		cmplx.setPolar(elementVal, phase);
		break;
	}
	return cmplx;
}

