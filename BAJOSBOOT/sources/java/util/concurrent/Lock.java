 package java.util.concurrent;

 /*
 * by Christopher-Eyk Hrabia
 */
public class Lock {

	public native void lock();

	public native void unlock();

	public native boolean tryLock();

}
