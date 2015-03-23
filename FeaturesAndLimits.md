# Features and limits of BAJOS #
BAJOS contains a byte code interpreter, exception handling, multithreading and the
java synchronization mechanism, a garbage collected heap, a priority based scheduler and a native code interface. One of the newest features is support of realtime interrupts and handling in java in special threads, which could be created by the user!

Currently BAJOS is based or better “inspired” on the JVM-specification with some limits.
Known differences are:
  * no long data type
  * no double data type
  * no unicode support
  * no full run time evaluation of all class/method/field-attributes
  * no complete exception handling
  * less than 256 classes/methods/local variables in methods
  * less than 16 kwords (32 bit) on heap
  * Array objects have no type information
  * No typechecking in the bytecode handlers for ASTORE
  * RETURN throws no IllegalMonitorStateException
  * String constants are not automatically embedded in a java.lang.String object. If you encounter problems for example with casting String constants, try ‘new String(“STRINGCONSTANT”)’ instead of ‘“STRINGCONSTANT”’
  * No Serialization, no java.lang.Serializable
  * No hashCode() methods
  * No System properties
  * No java.lang.Class, no getClass() methods
  * No java.lang.NumberFormatException, it is replaced by java.lang.IllegalArgumentException
  * No class java.lang.Number and no shortValue, floatValue, byteValue, longValue methods in subclasses.
  * No java.lang.Comparable, no compareTo() methods

The amount of used boot- and system-classes and the complexity of exception handling
must be adapted on the available memory space on your target system.
Java software can be developed with these limits on a PC and compiled with javac.
Classfiles compiled on external machines can be included.