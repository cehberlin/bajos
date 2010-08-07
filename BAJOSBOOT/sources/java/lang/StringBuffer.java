package java.lang;

/**
 * An expandable string of characters. Actually not very expandable!
 * 09/25/2001 added number formatting thanks to Martin E. Nielsen.
 * You must ensure that the buffer is large enough to take the formatted
 * number.
 *<P>
 * @author <a href="mailto:martin@egholm-nielsen.dk">Martin E. Nielsen</a>
 */
public final class StringBuffer
{
	
	char[] characters;
	int curPos = 0;

	public StringBuffer () {
		this(16);
	}
	
	public StringBuffer (String aString) {
		characters = aString.toCharArray();
	}

	public StringBuffer (int length) {
		characters = new char[length];
	}

	public StringBuffer delete(int start, int end) {
		if (start < 0 || start > end || start > curPos) {
			throw new ArrayIndexOutOfBoundsException();
		}
		if (end >= curPos) {
			end = curPos;
		} else {
			System.arraycopy(characters, end, characters, start, curPos-end);
		}
		curPos -= end-start;
		return this;
	}

	public StringBuffer append (String s) {
		return append(s.toCharArray());
	}

	public StringBuffer append (java.lang.Object aObject) {
		return append (aObject.toString());
	}

	public StringBuffer append (boolean aBoolean) {
		return append (Boolean.toString(aBoolean));
	}
	
	public StringBuffer append (char aChar) {
		if (curPos >= characters.length) {
			char [] nc = new char [1 + curPos];
			System.arraycopy (characters, 0, nc, 0, curPos);
			characters = nc;
		}
		characters[curPos++] = aChar;
		return this;
	}

	public StringBuffer append (char[] aChars) {
		int cl = characters.length;
		int sl = aChars.length;
		char [] nc = characters;
		if (sl + curPos > cl) {
			nc = new char[sl + curPos];
			System.arraycopy (characters, 0, nc, 0, curPos);
		}
		System.arraycopy (aChars, 0, nc, curPos, sl);
		characters = nc;
		curPos += sl;
		return this;
	}

	public StringBuffer append (int aInt) {
		return append(Integer.toString(aInt));
	}

	public StringBuffer append (float aFloat) {
		return append(Float.toString(aFloat));
	}
	
	public String toString() {
		return new String (characters, 0, curPos);
	}

	public char charAt(int i) {
		if (i > curPos) {
			throw new ArrayIndexOutOfBoundsException();
		}
		return characters[i];
	}
	
	public int length() {
		return curPos;
	}

	/**
	* Retrieves the contents of the StringBuffer in the form of an array of characters.
	*/
	public char [] getChars() {
		return characters;
	}
}
