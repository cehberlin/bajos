// 2.1.09 nicht mehr benutzen, nur noch die Files in den hw-spezifischen unterdirs !!
//@authors  22.02.2008; Anna Maria Damm; FHW-BA Berlin; Bayer Schering Pharma AG
//	 					H.-Christian Hecht, FHW-BA Berlin; CoMedServ GmbH
//						Adrian Lang,FHW-BA Berlin; Fritz-Haber-Institut
//						Stephan Bauer; FHW-BA Berlin; Bayer Schering Pharma AG
package platform;

import java.graphics.Point;


public class PlatForm	{
public PlatForm()	{}
public native char nativeCharIn();
public native void nativeCharOut(char c);
public native byte getButtons();
public native void setOnBoardLEDs(byte status);
public native void charLCDOut(char c);
public native void controlLCD(int control);
public native void exit (int status);
public native int currentTimeMillis();

/*Grafik-Methoden*/
public native void drawPointRGB(int x,int y, int color);
public native void drawFillRectRGB(int x1, int y1, int x2, int y2, int color);
public native void drawRectangleRGB(int x1, int y1, int x2, int y2, int fgcolor, int bkcolor);
public native void clearScreenRGB(int color);
public native void drawRoundRectRGB(int x1, int y1, int x2, int y2, int a, int b, int fgcolor, int bkcolor);
public native void drawEllipseRGB(int x1, int y1, int x2, int y2, int fgcolor, int bkcolor);
public native void drawCircleRGB(int x, int y, int r, int fgcolor, int bkcolor);
public native void drawLineRGB(int x1, int y1, int x2, int y2, int color);

public native void drawPointHSB(Point p);
public native void drawLineHSB(Point pa, Point pe);
public native void drawTriangleFill(Point p0, Point p1, Point p2);
public native void clearZBuffer();
public native void drawPointHSBZBuffer(Point p);
public native void drawLineHSBZBuffer(Point pa, Point pe);

/*Schrift-Methoden*/
public native void setFont(int index);
public native void setFontWindow(int x1, int y1, int x2, int y2);
public native void setFontAutoLineFeed(int value);
public native void setFontFixedFont(int value);
public native void setFontCursor(int x, int y);
public native void setFontColor(int fgcolor, int bkcolor);
public native int drawChar(char c);
public native int drawCharAt(int x, int y, char c, int fgcolor, int bkcolor);
public native int getCharWidth(char c);

}