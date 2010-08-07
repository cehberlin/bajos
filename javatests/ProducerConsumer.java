class Buffer	 {
int count=0;		// num Items in buffer
int BUFSIZE;
int out=0,in=0;
int [] buffer;

public Buffer (int bufsize)		{
	BUFSIZE=bufsize;
	buffer= new int[BUFSIZE];	}

public  synchronized void produce(int item) 	{
//while (count == (BUFSIZE-1))					{	
//		try 	{	wait();	} 
//		catch (/*Interrupted*/Exception e) { }	}
	++count;
	buffer[in] = item;
	in = (in + 1) % BUFSIZE;
	System.out.println("produce "+in+" "+count);
//	notifyAll();										
}

public  synchronized void consume(int  item)	{
//	while (count == 0) 							{
//		try { wait(); }
//		catch (/*Interrupted*/Exception e) { }}
	--count;
	item = buffer[out];
	out = (out + 1) % BUFSIZE;
	System.out.println("consume "+out + " " + count);
//	notifyAll();										
}
}

public class ProducerConsumer extends Thread	{
public static Buffer buf;
char dispatch;

public void run()	{
	System.out.println(dispatch);
	if (dispatch=='p')
		while (true) {buf.produce(77);}
	else
		while (true)	{buf.consume(77);}
}

public ProducerConsumer(char c)	{
	dispatch=c;							}

public static void main(String args[])		{
ProducerConsumer.buf=new Buffer(4);
ProducerConsumer p=new ProducerConsumer('p');
ProducerConsumer p1=new ProducerConsumer('p');
ProducerConsumer c=new ProducerConsumer('c');
p.start();
p1.start();
c.start();

p.run();										
}
}
