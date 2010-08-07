
all:
	javac  -g:none sources/platform/PlatForm.java
	mv  sources/platform/PlatForm.class  classes/platform
	javac  -classpath /home/Desktop/bajosavr/BAJOSBOOT/classes -g:none sources/java/lang/*.java
	mv sources/java/lang/*.class  classes/java/lang
	javac  -bootclasspath /home/Desktop/bajosavr/BAJOSBOOT/classes -g:none sources/java/io/*.java
	mv sources/java/io/*.class  classes/java/io



