/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
public class A {//extends Thread {
int [] ii= new int[7];
String s= new String(" Welt ");

	public static void main(String [] args){
	A a= new A();
	a.ii[1]=45;
	System.out.println('Y');
	char c=(char)System.in.read();
	System.out.println(c);
	System.out.println("bur"+a.ii[0]+a.s);
	a.doit();
	System.out.println(a.ii[3]);
	System.out.print("hallo");
	System.out.println(a.s);
	int n=3;
	int m=4;
	System.out.println(3*4);
	while(m<1000) System.out.println("x "+m++);

}
void doit()	{
synchronized (this)	{
ii[3]=33;
}

}
}