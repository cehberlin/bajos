package java.lang;
//bh
import java.io.*;
import platform.*;
/**
 * System utilities.
 */
public final class System
{
public static final OutStream out = new OutStream();
public static final InStream in = new InStream();

private System() {}
  
  /**
   * Copies one array to another.
   */
//public static native void arraycopy (Object src, int srcOffset, Object dest, int destOffset, int length);
  static void arraycopy (char[] src, int srcOffset, char[] dest, int destOffset, int length)
  {
    for (int i = 0; i < length; i++)
      dest[i + destOffset] = src[i + srcOffset]; 
  }

/**
   * @author 22.02.2008; Anna Maria Damm; FHW-BA Berlin; Bayer Schering Pharma AG
   * Terminate the application.m
   */
  public static void exit(int code) {
	PlatForm.exit(code);
   }
 
  /**
   * @authors 22.02.2008; Anna Maria Damm; FHW-BA Berlin; Bayer Schering Pharma AG
	 					H.-Christian Hecht, CoMedServ GmbH, IT 2006

   * Current time expressed in milliseconds. In the RCX, this is the number
   * of milliseconds since the RCX has been on. (In Java, this would
   * be since January 1st, 1970).
   */
 //bh public static native long currentTimeMillis();
public static int currentTimeMillis() {
	return PlatForm.currentTimeMillis();
   }
 
  /**
   * Get the singleton instance of Runtime.
   */
   /*bh
  public static Runtime getRuntime() {
  	return Runtime.getRuntime();
  }
  */
}

