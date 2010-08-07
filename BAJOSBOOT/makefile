
all:
	javac  -verbose -g:none -source 1.4 sources/platform/PlatForm.java -extdirs . -classpath /repos/bajos/BAJOSBOOT/classes
	mv  sources/platform/PlatForm.class  classes/platform
	javac  -verbose -source 1.4 -classpath /repos/bajos/BAJOSBOOT/classes -g:none sources/java/io/*.java -extdirs .  -bootclasspath .
	mv sources/java/io/*.class  classes/java/io
	javac  -verbose -source 1.4 -classpath /repos/bajos/BAJOSBOOT/classes -g:none sources/java/lang/*.java -extdirs .
	mv sources/java/lang/*.class  classes/java/lang
	javac  -verbose -source 1.4 -classpath /repos/bajos/BAJOSBOOT/classes -g:none sources/java/graphics/*.java -extdirs . -bootclasspath .
	mv sources/java/graphics/*.class  classes/java/graphics/






