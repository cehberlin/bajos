CPATH	= ./classes

JARGS	= -g:none -source 1.4 -extdirs . -bootclasspath $(CPATH) -d $(CPATH) -sourcepath ./sources

all:
	javac  $(JARGS) sources/platform/PlatForm.java
	javac  $(JARGS) sources/java/io/*.java
	javac  $(JARGS) sources/java/lang/*.java 
	javac  $(JARGS) sources/java/graphics/*.java
	javac  $(JARGS) sources/java/util/*.java

clean:
	rm classes/platform/PlatForm.class
	rm classes/java/io/*.class
	rm classes/java/lang/*.class
	rm classes/java/graphics/*.class
	rm classes/java/util/*.class
