/*
 * by Christopher-Eyk Hrabia
 */
public class SchedulerTest extends Thread	{

	int nr;

	int prio;

	SchedulerTest child;


	public SchedulerTest(int nr, int prio){
		this.nr=nr;
		this.prio=prio;
		//child=new SchedulerTest(nr+10,prio);
	}

	public void run()	{
		//child.start();
		setPriority(prio);
		for(int i=0;i<20;i++){
			System.out.println("My nr: "+ this.nr + " My prio: " + getPriority() + " Counter: " + i);
		}
	}

	public static void main(String args[]){

		SchedulerTest t1=new SchedulerTest(1,8);	
		SchedulerTest t2=new SchedulerTest(2,4);
		SchedulerTest t3=new SchedulerTest(3,2);
		SchedulerTest t4=new SchedulerTest(4,8);
		SchedulerTest t5=new SchedulerTest(5,5);
		t1.start();
		t2.start();
		t3.start();
		t4.start();
		t5.start();
		int mynr=0;
		int prio=5;
		for(int i=0;i<20;i++){
			System.out.println("Main:My nr: "+ mynr + " My prio: " + prio + " Counter: " + i);
		}								
	}
}
