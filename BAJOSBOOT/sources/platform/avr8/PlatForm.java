/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
//@authors  22.02.2008; Anna Maria Damm; FHW-BA Berlin; Bayer Schering Pharma AG
//	 		H.-Christian Hecht, FHW-BA Berlin; CoMedServ GmbH
//			Adrian Lang,FHW-BA Berlin; Fritz-Haber-Institut
//			Stephan Bauer; FHW-BA Berlin; Bayer Schering Pharma AG
package platform;

public class PlatForm	{
public PlatForm()	{}
public native char nativeCharIn();
public native void nativeCharOut(char c);
public native void exit (int status);
public native int  currentTimeMillis();
public native void charLCDOut(char c);
public native void controlLCD(int control);
public native float getTemperature();
}