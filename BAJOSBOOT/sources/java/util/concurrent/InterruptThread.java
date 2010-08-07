package java.util.concurrent;

/**
 * InterruptThread for realtime behavior of bajos
 */
public abstract class InterruptThread extends Thread	{

	Lock myLock;

	public final void run()	{
		while(true){
			myLock.lock();
			myLock.lock();
			isr();		
		}
	}

 	public abstract void isr();
 
}
