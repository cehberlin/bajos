
all:
	javac  -g:none -source 1.4 sources/platform/PlatForm.java
	mv  sources/platform/PlatForm.class  classes/platform
	javac  -source 1.4 -classpath /repos/bajos/BAJOSBOOT/classes -g:none sources/java/lang/*.java
	mv sources/java/lang/*.class  classes/java/lang
	javac  -source 1.4 -bootclasspath /repos/bajos/BAJOSBOOT/classes -g:none sources/java/io/*.java
	mv sources/java/io/*.class  classes/java/io



