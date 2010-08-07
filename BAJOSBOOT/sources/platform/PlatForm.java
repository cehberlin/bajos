
 //@authors 22.02.2008; Anna Maria Damm; FHW-BA Berlin; Bayer Schering Pharma AG
//	 					H.-Christian Hecht, FHW-BA Berlin;CoMedServ GmbH
//						Adrian Lang,FHW-BA Berlin; Fritz-Haber-Institut
//						Stephan Bauer; FHW-BA Berlin; Bayer Schering Pharma AG
package platform;


public class PlatForm	{
public PlatForm()	{}
public native static char nativeCharIn();
public native static void nativeCharOut(char c);
public native static byte getButtons();
public native static void setOnBoardLEDs(byte status);
public native static void charLCDOut(char c);
public native static void controlLCD(int control);
public native static void exit (int status);
public native static int currentTimeMillis();
public native static void initTimer();
public native static void drawPoint(int x,int y, int color);

public void drawPoint(float x,float y,float r, float g, float b)	{
drawPoint((int)x,(int)y,((((int)r)<<16)&0xff0000)|((((int)g)<<8)&0xff00)|((int)b&0xff));
}
float dz;
float delta, dx, dy, dr, dg, db;
float step=.99999999f;		// selten: einige pkte werden 2* gezeichnet, andere nicht
						// schwierigkeiten mit step=1. , 1.99999-> 1, 1.99999+1.-> 3.
						// -> löcher in line

public void drawLine(int xa,int ya, int za, int ra, int ga, int ba,
int xe, int ye, int ze, int re, int ge, int be)				{
float x=xa;
float y=ya;
float z=za;
float r=ra;
float g=ga;
float b=ba;
//	drawPoint(20,40,0xffffff);
//	System.exit(-1);
//	drawPoint(x,y,r,g,b);
	computeDelta(xa,ya,xe,ye);
//	System.out.println(delta);

	if (delta < 1.f) {drawPoint((int)xa,ya,ra,ga,ba);drawPoint(xe,ye,re,ge,be);return;}	//lass zBuffer entscheiden
	dr=(re-ra)/delta;
	dg=(ge-ga)/delta;
	db=(be-ba)/delta;
	dz=(ze-za)/delta;

do				{
	drawPoint(x,y,r,g,b);
	x+=dx;
	y+=dy;
	z+=dz;
	r+=dr;
	g+=dg;
	b+=db;	} while ((--delta) >.5f);	
}

public void computeDelta(int xa, int ya,int xe, int ye)		{
delta=xe-xa;
	if (Math.abs(delta) >= Math.abs(ye-ya))	{
		if (xe>=xa)	dx		=step;
		else								{
						dx		=-step;
						delta	=-delta;	}
		dy=(ye-ya)/delta;						}
	else											{
		delta=ye-ya;
		if (ye>=ya)	dy		=step;
		else 								{
						dy		=-step;
						delta	=-delta;	}
		dx=(xe-xa)/delta;						}	}
}
