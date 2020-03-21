#ifndef COMPLEX_H
#define COMPLEX_H



class Complex
{
private: //Memebers

	double m_real;
	double m_imaginary;

public:
	//Constructors
	Complex();
	Complex(double real, double img);
	//Copy constructor
	Complex(const Complex& n);


	//Setters
	void setComplex(double real, double img)
	{
		setCartesian(real, img);
	}
	inline void setCartesian(double mag, float phase);
	inline void setCartesian(double real, double img) { setReal(real); setImg(img); }
	inline void setReal(double real) { m_real = real; }
	inline void setImg(double img)   { m_imaginary = img; }
	//Getters
	double getReal() const { return m_real; }
	double getImg()  const	 { return m_imaginary; }


public:

	//Operator Overloading
	void operator + (Complex n);
	void operator - (Complex n);
	Complex operator * (Complex n);

};





#endif