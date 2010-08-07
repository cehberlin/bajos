package java.util.concurrent;

/**
 * InterruptThread for realtime behavior of bajos
 */
public abstract class InterruptThread extends Thread	{

	public static native void sei();

	public static native void cli();

	public InterruptThread(){
		setPriority(MAX_PRIORITY);
		run();	
	}

	public final void run()	{
		while(true){
			Lock.lock(this);
			Lock.lock(this);
			isr();		
		}
	}

 	public abstract void isr();
 
}
