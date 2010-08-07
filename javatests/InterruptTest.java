/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
//by CEH
/*
 * This example is only for arduino and linux, please consider you comment in and out the right lines, see comments
 */
import java.util.concurrent.InterruptThread;

public class InterruptTest extends InterruptThread {

	public void isr(){
		System.platform.nativeSetData(0x25, (byte)(System.platform.nativeGetData(0x25)^0x80)); //comment in for arduino; comment out for linux!
		//System.out.println("Interrupt Occured in Java!!"); //comment out for arduino; comment in for linux!
	}


	public static void main(String [] args)		{
	
		InterruptTest inter = new InterruptTest();
		inter.initInterrupt((byte)23);

		for(int i=0;i<100000;i++){
		//InterruptTest.forceInterrupt();	//comment out for arduino; comment in for linux!
		}

		InterruptTest.removeInterrupt((byte)5);

	}
}


