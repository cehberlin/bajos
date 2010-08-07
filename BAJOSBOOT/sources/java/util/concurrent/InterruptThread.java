package java.util.concurrent;

/**
 * InterruptThread for realtime behavior of bajos
 */
public abstract class InterruptThread extends Thread	{

	Lock myLock;

	public native void interrupt();

	public native void sei();

	public native void cli();

	public InterruptThread(){
		setPriority(MAX_PRIORITY);	
	}

	public final void run()	{
		while(true){
			myLock.lock();
			myLock.lock();
			isr();		
		}
	}

 	public abstract void isr();
 
}
