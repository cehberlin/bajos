
//@authors  22.02.2008; Anna Maria Damm; FHW-BA Berlin; Bayer Schering Pharma AG
//	 					H.-Christian Hecht, FHW-BA Berlin; CoMedServ GmbH
//						Adrian Lang,FHW-BA Berlin; Fritz-Haber-Institut
//						Stephan Bauer; FHW-BA Berlin; Bayer Schering Pharma AG
package platform;

import java.graphics.Point;


public class PlatForm	{
public PlatForm()	{}
public static native char nativeCharIn();
public static native void nativeCharOut(char c);
public static native byte getButtons();
public static native void setOnBoardLEDs(byte status);
public static native void charLCDOut(char c);
public static native void controlLCD(int control);
public static native void exit (int status);
public static native int currentTimeMillis();

/*Grafik-Methoden*/
public static native void drawPointRGB(int x,int y, int color);
public static native void drawFillRectRGB(int x1, int y1, int x2, int y2, int color);
public static native void drawRectangleRGB(int x1, int y1, int x2, int y2, int fgcolor, int bkcolor);
public static native void clearScreenRGB(int color);
public static native void drawRoundRectRGB(int x1, int y1, int x2, int y2, int a, int b, int fgcolor, int bkcolor);
public static native void drawEllipseRGB(int x1, int y1, int x2, int y2, int fgcolor, int bkcolor);
public static native void drawCircleRGB(int x, int y, int r, int fgcolor, int bkcolor);
public static native void drawLineRGB(int x1, int y1, int x2, int y2, int color);

public static native void drawPointHSB(Point p);
public static native void drawLineHSB(Point pa, Point pe);
public static native void drawTriangleFill(Point p0, Point p1, Point p2);
public static native void clearZBuffer();
public static native void drawPointHSBZBuffer(Point p);
public static native void drawLineHSBZBuffer(Point pa, Point pe);

/*Schrift-Methoden*/
public static native void setFont(byte index);
public static native void setFontWindow(int x1, int y1, int x2, int y2);
public static native void setFontAutoLineFeed(byte value);
public static native void setFontFixedFont(byte value);
public static native void setFontCursor(int x, int y);
public static native void setFontColor(int fgcolor, int bkcolor);
public static native int drawChar(char c);
public static native int drawCharAt(int x, int y, char c, int fgcolor, int bkcolor);
public static native int getCharWidth(char c);
/*public static native int drawString(String s);
public static native int drawStringAt(int x, int y, String s);
public static native int getStringWidth(String s);*/

}
