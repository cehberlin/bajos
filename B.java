/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
public class B {
int x=33;
String s="hallo";
char zahlen[]={'1','2','3','4','5'};
Integer n=new Integer(-12345);
	public static void main(String [] args){
	B b= new B();
	char zahlen[]={'1','2','3','4','5'};
	System.out.println(b.s);
	b.n.toString();
String s=new String ( b.zahlen, 1, 2);
System.out.println(b.x);
System.out.println(new String ( b.zahlen, 1, 2) ); 
System.out.println(b.n.toString());
for (int i=0; i < 5;i++) System.out.println(b.zahlen[i]);
}

}