

package java.graphics; 
public class Point	{
public float x,y,z;
public float r,g,b;
/*
public float 	x,y,z;			// die Koordinaten
public float	h,s,b;			// 0.<= float <= 255. 
					// int Farbe als 32 Bit Wert 0000 0000 hhhh hhhh ssss ssss bbbb bbbb

	int getX()  { return (int)(x);}
	int getY()  { return (int)(y);}
	int getZ()  { return (int)(z);}
	int getH()	{ return (int)(h);}
	int getS()	{ return (int)(s);}
	int getB()	{ return (int)(b);}
	
	void   setPoint(float x, float y,float z )	{
		h=System.graphics.hue;s=System.graphics.saturation;b=System.graphics.brightness;
		this.x=x;this.y=y;this.z=z;				}
	
	void   setPoint(float x, float y )			{
		setPoint(x,y,0.);							}

	Point(float x,float y, float z)			{
		setPoint(x,y,z);							}	

	Point()				{		this(0.,0.,0.);		}

	Point(float x,float y)   {	this(x,y,0.);	}

	Point(Point p)				{	// nur fuer internen Gebrauch
		setPoint(p.x,p.y,p.z);
		HSBColor(p.h,p.s,p.b);	}

	public void HSBColor(float hue, float saturation, float brightness)	{  //nur fuer internen Gebrauch
		h	=hue;
		s	=saturation;
		b	=brightness;															}

	public static void setHSBColor(float hue, float saturation, float brightness)	{
		Point.hue	=hue;
		Point.saturation=saturation;
		Point.brightness=brightness;												}

	public String toString()			{				// debug
		return  (x+" "+y+" "+z+" ");	}	
*/
}
