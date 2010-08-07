package java.util;

public interface Collection {
	public int size();
	public boolean isEmpty();
	public boolean contains(Object o);
	/*
	public Iterator iterator();
	*/
	public Object[] toArray();
	public Object[] toArray(Object[] a);
	public boolean add(Object o);
	public boolean remove(Object o);
	/*
	public boolean containsAll(Collection c);
	public boolean addAll(Collection c);
	public boolean removeAll(Collection c);
	public boolean retainAll(Collection c);
	*/
	public void clear();
}
