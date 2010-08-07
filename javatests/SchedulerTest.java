
public class SchedulerTest extends Thread	{

int nr;

int prio;

int counter;

public SchedulerTest(int nr, int prio){
this.nr=nr;
this.prio=prio;
}

public void run()	{
	setPriority(prio);
	while(true){
		//System.out.println("My nr: "+ this.nr + " My prio: " + getPriority());
		counter++;
	}
}

public static void main(String args[])		{

SchedulerTest t1=new SchedulerTest(1,8);
SchedulerTest t2=new SchedulerTest(2,4);
SchedulerTest t3=new SchedulerTest(3,2);
t1.start();
t2.start();
t3.start();
/*
	Here you can see, that getPriority gives only the priority of the current running thread.
*/
while(true){
	System.out.println("My nr: "+ t1.nr + " My prio: " + t1.getPriority()+ " My Couter: "+ t1.counter);
	System.out.println("My nr: "+ t2.nr + " My prio: " + t2.getPriority()+ " My Couter: "+ t2.counter);
	System.out.println("My nr: "+ t3.nr + " My prio: " + t3.getPriority()+ " My Couter: "+ t3.counter);
}										
}
}
