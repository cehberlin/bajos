/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
//by CEH
import java.util.concurrent.InterruptThread;

public class InterruptTest	extends InterruptThread		{

	public void isr(){
		//DDRB PB7 output ->LED
		System.platform.nativeSetData(0x25, (byte)(System.platform.nativeGetData(0x25)^0x80)); // PORTB toggle
	}


	public static void main(String [] args)		{
	
	System.out.println("1 sec blinking led an arduinoMega -> type any char for closing application");

	InterruptTest inter = new InterruptTest();
	System.platform.nativeSetData(0x24, (byte)(System.platform.nativeGetData((byte)0x24)|0x80));
	System.out.println("init Timer");
	System.platform.initTimer8bit(inter,1024);

						
	System.in.read();	/* dummy read	*/		
	}
}


