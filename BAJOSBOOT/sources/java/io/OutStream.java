package java.io;
import platform.PlatForm;
import java.lang.*;

public class OutStream {
	PlatForm platform;

	public OutStream(PlatForm platform) {
		this.platform = platform;
	}

	public synchronized void print(char c) {
		platform.nativeCharOut(c);
	}

	public synchronized void print(String s) {
		for (int i = 0 ; i < s.length() ; i++) {
			print(s.charAt(i));
		}
	}

	public synchronized void print(int n) {
		print(Integer.toString(n));
	}

	public synchronized void print(float f) {
		// das sollte definitiv static sein; dafür muss aber noch ein bug mit static-aufrufen gefixt werden
		// print(Float.toString(f));

		// die native ist irgendwie verbuggt, das geht nicht.
		// print(new Float(f).toString());

		// daher über die in java-implementierte stringbuffer-methode
		StringBuffer buff = new StringBuffer();
		buff.append(f);
		print(buff.toString());
	}

	public synchronized void println() {
		print('\n');
	}

	public synchronized void println(char c) {
		print(c);
		println();
	}

	public synchronized void println(String s) {
		print(s);
		println();
	}

	public synchronized void println(int n) {
		print(n);
		println();
	}

	public synchronized void println(float f) {
		print(f);
		println();
	}
}
