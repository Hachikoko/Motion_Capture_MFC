// Code writen by: Vic Hollis 09/07/2003
// I don't mind if you use this class in your own code. All I ask is 
// that you give me credit for it if you do.  And plug NeHe while your
// at it! :P  

#pragma once
#include <math.h>

class Vector  
{
public:
	void operator =(Vector v);            
	Vector operator+(Vector v);         //Add
	Vector operator-(Vector v);         //Subtract
	Vector operator*(float scalar);     
	Vector operator^( Vector v );       //Cross Product
	const float operator%( Vector v );  //Dot product
	void operator *=(float scalar);

	void Normalize(void);
	float Magnitude(void);


	Vector();
	Vector( float I, float J, float K );
	virtual ~Vector();

	float k;
	float j;
	float i;
	float m_Mag;

};

