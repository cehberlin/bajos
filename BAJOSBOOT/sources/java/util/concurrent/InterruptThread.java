package java.util.concurrent;

import java.lang.Thread;

/**
 * InterruptThread for realtime behavior of bajos
 */
public abstract class InterruptThread extends Thread	{

	public static native void sei();

	public static native void cli();

	public InterruptThread(){
		System.out.println("InterruptThread Konstruktor");
		setPriority(MAX_PRIORITY);
		start();	
	}

	public void run()	{
		System.out.println("run started");
		while(true){
			Lock.lock(this);
			Lock.lock(this);
			isr();		
		}
	}

 	public abstract void isr();
 
}
