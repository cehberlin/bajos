/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
//by CEH
import java.util.concurrent.InterruptThread;

public class InterruptTest extends InterruptThread {

	public void isr(){
		System.out.println("Interrupt Occured in Java!!");
	}


	public static void main(String [] args)		{
	
		InterruptTest inter = new InterruptTest();

		System.platform.initTestInterrupt(inter);

		System.platform.forceTestInterrupt();	

		System.platform.removeTestInterrupt();

	}
}


