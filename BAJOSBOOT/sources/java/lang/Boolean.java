package java.lang;

public final class Boolean {
	public final static Boolean TRUE = new Boolean(true);
	public final static Boolean FALSE = new Boolean(false);

	private boolean boolval;

	public Boolean(boolean val) {
		boolval = val;
	}

	public static String toString(boolean val) {
		return val ? "true" : "false";
	}

	public String toString() {
		return toString(boolval);
	}
}
