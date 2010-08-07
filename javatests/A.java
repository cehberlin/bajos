/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
import java.io.*;
import java.graphics.*;

public class A extends Aparent
{
int whow=99;

public A()	{
System.out.println ("im Konstruktor von a");
}
	public static void main(String [] args)
	{
System.out.println ("Starting a.class ...");
A a=new A();
a.m=a.n;
a.arr[2]=8;
st=99;
//a.n=55;
System.out.println ("Starting a.class ...");

A.xMax=317;
System.out.println(Aparent.xMax);
System.out.println(A.xMax);
System.out.println(Aparent.yMax);
System.out.println(A.yMax);
System.out.println(a.n);
System.out.println(a.arr[2]);

		System.out.println ("Starting a.class ...");
		System.out.println (" OK");
		
		System.out.println (" make a Display Object...");
		DisplayZBuffer dp = new DisplayZBuffer();
/*
		System.out.println (" OK");		
		System.out.println (" draw fillRect...");
		dp.setColor(0xff0000); //rot
		dp.fillRect(20,20,30,30);
		System.out.println (" OK");
		
		System.out.println (" draw drawRect...");
		dp.setColor(0x000000); //schwarz
		dp.drawRect(60, 60, 30, 30);
		System.out.println (" OK");
		
		System.out.println (" draw drawLine...");
		dp.drawLine(300, 200, 10, 100);
		System.out.println (" OK");

		while (true) {

		}
*/
	}
}
