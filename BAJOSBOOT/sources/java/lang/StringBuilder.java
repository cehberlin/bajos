/* StringBuilder.java -- Unsynchronized growable strings
   Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005
   Free Software Foundation, Inc.

This file is part of GNU Classpath.

GNU Classpath is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Classpath is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Classpath; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version. */

package java.lang;

public final class StringBuilder	{
    private static final int DEFAULT_CAPACITY = 16;

  int count;
  char[] value;
  public StringBuilder()  {
    this(DEFAULT_CAPACITY);
  }
  public StringBuilder(int capacity)  {
    value = new char[capacity];
  }
  public StringBuilder(String str)  {
    count = str.length();
    value = new char[count + DEFAULT_CAPACITY];
    System.arraycopy(str.toCharArray(),0,value,0, count);
//    System.arraycopy(value,0,str.toCharArray(),0, count);
//      for (int i=0; i < count; i++)
//  System.arraycopy (c, off, characters, 0, len);
//	value[i]=str.toCharArray()[i];
    
    
  }

  public int length()
  {
    return count;
  }
  public int capacity()
  {
    return value.length;
  }
  public void ensureCapacity(int minimumCapacity)
  {
    if (minimumCapacity > value.length)
      {
        int max = value.length * 2 + 2;
        minimumCapacity = (minimumCapacity < max ? max : minimumCapacity);
        char[] nb = new char[minimumCapacity];
        System.arraycopy(value,0,nb,0, count);
 //       System.arraycopy(value, 0, nb, 0, count);
  //      for (int i=0; i < count; i++)
//	value[i]=nb[i];     
        
        value = nb;
      }
  }

  public void setLength(int newLength)
  {
//bh    if (newLength < 0) throw new StringIndexOutOfBoundsException(newLength);

    int valueLength = value.length;

    /* Always call ensureCapacity in order to preserve copy-on-write
       semantics.  */
    ensureCapacity(newLength);

    if (newLength < valueLength)
      {
	count = newLength;
      }
    else
      {
	while (count < newLength)
          value[count++] = '\0';
      }
  }

  public char charAt(int index)
  {
//    if (index < 0 || index >= count)      throw new StringIndexOutOfBoundsException(index);
    return value[index];
  }
  public void setCharAt(int index, char ch)
  {
//bh    if (index < 0 || index >= count) throw new StringIndexOutOfBoundsException(index);
    ensureCapacity(count);
    value[index] = ch;
  }
  public StringBuilder append(Object obj)
  {
    return append(obj == null ? "null" : obj.toString());
  }

  public StringBuilder append(String str)
  {
    if (str == null)
      str = "null";
    int len = str.length();
    ensureCapacity(count + len);
    System.arraycopy(str.toCharArray(),0,value,count, len);
//        for (int i=0; i < len; i++)
//	str.toCharArray()[i]=value[i+count];
   // str.getChars(0, len, value, count);
    count += len;
    return this;

  }
  public StringBuilder append(boolean bool)
  {
    return append(Boolean.toString(bool));
  }

  public StringBuilder append(char ch)
  {
    ensureCapacity(count + 1);
    value[count++] = ch;
    return this;
  }

  public StringBuilder append(int inum)
  {
    return append(String.valueOf(inum));
  }

  public StringBuilder append(float fnum)
  {
    return append(Float.toString(fnum));
  }
  public String toString()
  {
    return new String(this.value,0,this.length());
  }
}
