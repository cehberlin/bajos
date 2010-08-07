
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
}