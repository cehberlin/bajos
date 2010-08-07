package java.lang;

/**
 * An expandable string of characters. Actually not very expandable!
 * 09/25/2001 added number formatting thanks to Martin E. Nielsen.
 * You must ensure that the buffer is large enough to take the formatted
 * number.
 *<P>
 * @author <a href="mailto:martin@egholm-nielsen.dk">Martin E. Nielsen</a>
 */
public final class StringBuffer implements CharSequence
{
	
	char[] characters;
	int curPos = 0;

	public StringBuffer () {
		this(16);
	}
	
	public StringBuffer (String aString) {
		characters = aString.toCharArray();
		curPos = characters.length;
	}

	public StringBuffer (int length) {
		characters = new char[length];
	}

	public StringBuffer delete(int start, int end) {
		checkIndex(start);
		if (start > end) {
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

	public StringBuffer deleteCharAt(int index) {
		checkIndex(index);
		System.arraycopy(characters, index + 1, characters, index, curPos - index);
		--curPos;
		return this;
	}

	public StringBuffer replace(int start, int end, String str) {
    checkIndex(start);
    if (start > end) {
      throw new ArrayIndexOutOfBoundsException();
    }
		char[] buf = new char[start + str.length() + curPos - end];
		System.arraycopy(characters, 0, buf, 0, start);
		System.arraycopy(characters, end, buf, str.length() + start, curPos - end);
		str.getChars(0, str.length(), buf, start);
		characters = buf;
		curPos = buf.length;
		return this;
	}

	public String substring(int start) {
		checkIndex(start);
		return new String(characters, start, characters.length - start);
	}

	public StringBuffer append (String s) {
		return append(s.toCharArray());
	}

	public StringBuffer append (Object aObject) {
		return append ( aObject != null ? aObject.toString() : "null");
	}

	public StringBuffer append(StringBuffer sb) {
		if (sb != null) {
			ensureCapacity(curPos + sb.curPos);
			System.arraycopy(sb.characters, 0, characters, curPos, sb.curPos);
			curPos += sb.curPos;
			return this;
		} else {
			return append("null");
		}
	}

	public StringBuffer append (boolean aBoolean) {
		return append (Boolean.toString(aBoolean));
	}
	
	public StringBuffer append (char aChar) {
		ensureCapacity(curPos + 1);
		characters[curPos++] = aChar;
		return this;
	}

	public StringBuffer append (char[] aChars) {
		return append(aChars, 0, aChars.length);
	}

	public StringBuffer append(char[] str, int offset, int len) {
		ensureCapacity(curPos + len);
		System.arraycopy (str, offset, characters, curPos, len);
		curPos += len;
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
		checkIndex(i);
		return characters[i];
	}
	
	public int length() {
		return curPos;
	}

	public int capacity() {
		return characters.length;
	}

	public void ensureCapacity(int target) {
		if (characters.length < target) {
			char[] newbuf = new char [Math.max(target, characters.length * 2)];
			System.arraycopy(characters, 0, newbuf, 0, characters.length);
			characters = newbuf;
		}
	}

	public void setLength(int newLength) {
		if (newLength < 0) {
			throw new ArrayIndexOutOfBoundsException();
		}
		if (newLength < curPos) {
			curPos = newLength;
		} else {
			ensureCapacity(newLength);
			for (++curPos ; curPos <= newLength ; ++curPos) {
				characters[curPos] = '\00';
			}
		}
	}

	public void setCharAt(int index, char ch) {
		checkIndex(index);
		characters[index] = ch;
	}

	private void checkIndex(int index) {
		if (index < 0 || index > curPos) {
			throw new ArrayIndexOutOfBoundsException();
		}
	}

	private void checkRange(int lower, int upper) {
		if (lower < 0 || lower > upper || upper > curPos) {
			throw new ArrayIndexOutOfBoundsException();
		}
	}

	public char[] getChars() {
		return characters;
	}

  public void getChars(int srcBegin, int srcEnd, char[] dst, int dstBegin) {
		checkRange(srcBegin, srcEnd);
    if (dstBegin < 0 || dstBegin+(srcEnd-srcBegin) > dst.length) {
      throw new ArrayIndexOutOfBoundsException();
    }
    System.arraycopy (characters, srcBegin, dst, dstBegin, srcEnd - srcBegin);
  }

	public String substring(int start, int end) {
		checkRange(start, end);
		return new String(characters, start, end - start);
	}

	public CharSequence subSequence(int start, int end) {
		return substring(start, end);
	}

/*	public StringBuffer insert(int index, char[] str, int offset, int len) {
		checkIndex(index);
		if (offset < 0 || len < 0 || offset + len > str.length) {
			throw new ArrayIndexOutOfBoundsException();
		}
		ensureCapacity(curPos+len);
		char[] buf = new char[curPos + len];
		System.arraycopy(characters, 0, buf, 0, index);
		System.arraycopy(str, offset, buf, index, len);
		System.arraycopy(characters, index + 1, buf, index + len + 1, curPos - index);
		characters = buf;
		curPos += len;
		return this;
	}*/

	public StringBuffer insert(int index, String str) {
		checkIndex(index);
		int len = str.length();
		ensureCapacity(curPos+len);
		char[] buf = new char[curPos + len];
		System.arraycopy(characters, 0, buf, 0, index);
		str.getChars(0, len, buf, index);
		System.arraycopy(characters, index + 1, buf, index + len + 1, curPos - index);
		characters = buf;
		curPos += len;
		return this;
	}

/*	public int indexOf(String str) {
		String th = this.toString();
		for (int k = 0 ; k < curPos - str.length() ; ++k) {
		 if (th.startsWith(str, k)) return k;
		}
		return -1;
	}

	public int indexOf(String str, int fromIndex) {
		String th = this.toString();
		for (int k = Math.min(fromIndex, str.length()) ; k < curPos - str.length() ; ++k) {
		 if (th.startsWith(str, k)) return k;
		}
		return -1;
	}*/

	public StringBuffer reverse() {
		char [] buf = new char [curPos];
		for (int i = 0 ; i < curPos ; ++i) {
			buf[i] = characters[curPos - i];
		}
		characters = buf;
		return this;
	}
}
