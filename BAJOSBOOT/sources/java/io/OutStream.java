package java.io;
import platform.*;
//import java.lang.*;


public class OutStream	{
//PlatForm platform;
//public final static Charon	platform=new Charon();
/*public OutStream(PlatForm platform)	{
this.platform=platform;
}
*/
public synchronized void print(String s)							{
for (int i=0;i< s.length();i++) print(s.charAt(i));	}

public synchronized void  println(String s)	{
print(s);
println();							}

public synchronized void  println()				{
print('\n');							}

public synchronized void print(char c)		{
PlatForm.nativeCharOut(c);		
}

public synchronized void println(char c)		{
print(c);
println();							}

public synchronized void  print(int n)				{
print(Integer.toString(n));	}

public synchronized void  print(float f)			{
print(new Float(f).toString());
}

public synchronized void  println(float f)		{
print(f);
println();						}

public synchronized  void println(int n)		{
print(n);
println();											}

}
