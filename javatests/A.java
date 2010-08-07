/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
public class A {//extends Thread {
int [] ii= new int[7];
//float f=3.37F;
String s= new String(" Welt ");

	public static void main(String [] args){
	A a= new A();
	a.ii[1]=45;
	System.out.println ("Starting timer ...");
	System.out.println (" OK");
	byte b; int n = 0xFFFFFF;
	while (true) {
	//	c = System.platform.getButtons();
	//	System.platform.LCDControl(0);
	//	System.platform.LCDControl(0x0F02);
	//	System.platform.LCDOut( (byte) c );
	//	n = (n + 1 ) % 4;
		n = (n * 2);
	//	System.out.println( c);
	//	System.out.println( n );
		if (n < -1000 ) n = 0xFFFFFF;
		System.platform.setOnBoardLEDs( (byte) n );
	//	System.out.println(" 1");
		if (System.platform.getButtons()>0) {
			System.out.println(System.platform.currentTimeMillis());
		}

		if (System.platform.getButtons() >= 128) {
			System.exit(0);
		}
	}
	




/*

	System.platform.controlLCD(0);
	System.platform.charLCDOut('a');
	System.out.println(6);
	char c=System.in.read();
System.out.println('Y');
	System.out.println(c);
//System.out.println(a.f);
	System.out.println("bur"+a.ii[0]+a.s);
	a.doit();
	System.out.println(a.ii[3]);
	System.out.print("hallo");
	System.out.println(a.s);
	n=3;
	int m=4;
	System.out.println(3*4);
	while(m<10) {System.out.println("xx "+m++);}
	int time = System.currentTimeMillis();
	do	{
	if ((System.currentTimeMillis()-time)>5000)	{
	time=System.currentTimeMillis();
	System.out.println("hallo");
	continue;}
	} while(true);
*/
}

void doit()	{
/*synchronized (this)*/	{
ii[3]=33;
}

}
}
