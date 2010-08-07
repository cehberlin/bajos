package java.lang;

/**
 * An immutable string of characters.
 */
public final class String
{
  // NOTE: The state of this class is mapped to
private char[] characters; // let it private
// never use characters.length better length()

public String(String str)
  {
      characters = new char[str.length()];
  for (int i=0; i<str.length(); i++)	
  characters[i]=str.charAt(i); 
  }

  /**
   * Create a String from a character array.
   * @param c the character array
   * @param off the offset - usually 0
   * @param len the length of the String - must not be greater than c.length
   **/
  public String (char[] c, int off, int len)
  {
   //  System.out.print('\n');
  //for (int i=0;i < len ; i++) System.out.print(c[i+off]);
     // System.out.print('\n');
    characters = new char[len];
  //  for (int i=0; i < len; i++)
    System.arraycopy (c, off, characters, 0, len);
//	characters[i]=c[i+off];
  }

  /**
   * Return the length of the String in characters
   * @return the length of the String
   **/
public native int nativeStringLength();

  public int length()
  {
  int length;
  if ((length=nativeStringLength())!=0xffff)  return length;
else    return characters.length;
  }

  /**
   * Return the character at the given index
   * @return the characters at the given index
   **/
 
 public native char nativeCharAt(int index);

  public char charAt(int index) 
  {
  char c;
    //  System.out.print('\n');
  if ((c=nativeCharAt(index))!=0) {
 // System.out.println('B');
  return c;}
  else{
  //System.out.println('C');
    return characters[index];
    }
  }

  /**
   * Converts the String into an array of characters
   * @return the character Array
   **/
  public char[] toCharArray()
  {
    int len = length()-1;
    char[] ca = new char[len+1];
for (;len >=0; --len) ca[len]=charAt(len);
    return ca;
  }

  /**
   * Converts an Object to a String
   * @return the String that represents the object
   **/
  public static String valueOf (Object aObj)
  {
    return aObj.toString();
  }
 public static String valueOf (int n)
  {
    return new Integer(n).toString();
  }
  /**
   * Returns itself.
   * @return the String itself
   */
  public String toString()
  {
    return this;
  }
  
  /**
   * Compares the String with an Object
   * @return true if the String is equal to the object, false otherwise
   **/
  public boolean equals(Object other) // to do
  {
    if (other == null)
      return false;
      
//bh    try {
      String os = (String)other;
      if (os.characters.length != characters.length)
         return false;
         
      for (int i=0; i<characters.length; i++)
      {
        if (characters[i] != os.characters[i])
          return false;
      }
      
      return true;
   // } catch (ClassCastException e) {
    //}    
//    return false;
  }
}

