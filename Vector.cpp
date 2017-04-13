#include "StdAfx.h"
#include "Vector.h"

// Code writen by: Vic Hollis 09/07/2003
// I don't mind if you use this class in your own code. All I ask is 
// that you give me credit for it if you do.  And plug NeHe while your
// at it! :P  

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Vector::Vector()
{
	i = j = k = m_Mag = 0.0f;										// Initalize the i, j, k, and magnitude to 0.0
}

Vector::Vector( float I, float J, float K )
{
	i = I;
	j = J;
	k = K;
}

Vector::~Vector()
{
}

// Scale the Vector times some number
void Vector::operator *=(float scalar)
{
	i *= scalar;
	j *= scalar;
	k *= scalar;
}

// Computes the magnitude and saves it in m_Mag also returns the 
// Magnitude
float Vector::Magnitude()
{
	float result; 
	
	result = float(sqrt(i * i + j * j + k * k));
	m_Mag = result;
	return(result);
}

// Makes the Vector unit length.
void Vector::Normalize()
{
	if(m_Mag != 0.0f)
	{
		i /= m_Mag;
		j /= m_Mag;
		k /= m_Mag;

		Magnitude();
	}
}

// Multiply this Vector times another and return the result
Vector Vector::operator *(float scalar)
{
	Vector r;

	r.i = i * scalar;
	r.j = j * scalar;
	r.k = k * scalar;

	return(r);
}

// Add this Vector to another and return the result
Vector Vector::operator +(Vector v)
{
	Vector r;

	r.i = i + v.i;
	r.j = j + v.j;
	r.k = k + v.k;

	return(r);
}

Vector Vector::operator -(Vector v)
{
	Vector r;

	r.i = i - v.i;
	r.j = j - v.j;
	r.k = k - v.k;

	return(r);
}

// Assign this Vector to the Vector passed in.
void Vector::operator =(Vector v)
{
	i = v.i;
	j = v.j;
	k = v.k;
	m_Mag = v.m_Mag;
}

//Cross Product
Vector Vector::operator ^( Vector v )
{
	Vector temp;
	temp.i = j * v.k - k * v.j;
	temp.j = k * v.i - i * v.k;
	temp.k = i * v.j - j * v.i;
	return(temp);
}

//Dot Product
const float Vector::operator %( Vector v )
{
	return ( i * v.i + j * v.j + k * v.k );
}

