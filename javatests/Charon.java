/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
//import java.graphics.*;

public class Charon
{

	public static void main(String [] args)
	{
		System.out.println("thats a Charon board");
System.out.println("LCD-Test type e for \"end\"");
System.platform.controlLCD(0x00000000); //clear
char c;
// not q!!!
while ((c=System.in.read())!='e') 				{
System.platform.charLCDOut(c);
System.out.println(System.currentTimeMillis());	}
System.out.println("currentTimeMillis Test in a \"for loop\"");
for (c=0; c < 4; c++) System.out.println(System.currentTimeMillis());
System.out.println("1 sec blinking leds on charon");
System.platform.nativeSetPort((char)0x31,(byte) 0xff);
int currentTime=System.currentTimeMillis();
while (System.platform.nativeConStat()==0)	{
while ((System.currentTimeMillis() - currentTime) < 1000);
System.platform.nativeSetPort((char)0x32, (byte)(System.platform.nativeGetPort((char)0x32)^0xff));
// PORTB toggle
currentTime+=1000;				
}
}
}
