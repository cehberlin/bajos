/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
//by CEH
import java.util.concurrent.InterruptThread;

public class InterruptTest extends InterruptThread {

	public void isr(){
		System.platform.nativeSetData(0x25, (byte)(System.platform.nativeGetData(0x25)^0x80));
		//System.out.println("Interrupt Occured in Java!!");
	}


	public static void main(String [] args)		{
	
		InterruptTest inter = new InterruptTest();

		System.platform.initTestInterrupt(inter);

		for(int i=0;i<10000000;i++){
		//System.platform.forceTestInterrupt();	//comment out for arduino; comment in for linux!
		}

		System.platform.removeTestInterrupt();

	}
}


