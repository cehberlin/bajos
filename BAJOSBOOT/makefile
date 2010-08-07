
# compile platform agains sun java system classes
# compile io against sun java classes and bajos-platform
# compile lang against bajos-io and bajos-platform 


boot:
	javac  -verbose -source 1.4  -g:none -classpath ./BAJOSBOOT/classes ./BAJOSBOOT/sources/java/io/*.java
	mv ./BAJOSBOOT/sources/java/io/*.class  ./BAJOSBOOT/classes/java/io
	javac  -verbose -source 1.4 -classpath ./BAJOSBOOT/classes -g:none ./BAJOSBOOT/sources/java/lang/*.java -extdirs . -bootclasspath .
	mv ./BAJOSBOOT/sources/java/lang/*.class  ./BAJOSBOOT/classes/java/lang
	javac  -verbose -source 1.4  -g:none  ./BAJOSBOOT/sources/java/util/*.java -extdirs . -bootclasspath . -classpath ./BAJOSBOOT/classes
	mv ./BAJOSBOOT/sources/java/util/*.class  ./BAJOSBOOT/classes/java/util

graphic:
	javac  -verbose -source 1.4 -classpath ./BAJOSBOOT/classes -g:none ./BAJOSBOOT/sources/java/graphics/*.java
	mv ./BAJOSBOOT/sources/java/graphics/*.class  ./BAJOSBOOT/classes/java/graphics/


avr8:
	javac  -verbose -g:none -source 1.4 ./BAJOSBOOT/sources/platform/avr8/PlatForm.java 
	mv  ./BAJOSBOOT/sources/platform/avr8/PlatForm.class  ./BAJOSBOOT/classes/platform

evk1100:
	javac  -verbose -g:none -source 1.4 ./BAJOSBOOT/sources/platform/evk1100/PlatForm.java
	mv  ./BAJOSBOOT/sources/platform/evk1100/PlatForm.class  ./BAJOSBOOT/classes/platform

linux:
	javac  -verbose -g:none -source 1.4 ./BAJOSBOOT/sources/platform/linux/PlatForm.java
	mv  ./BAJOSBOOT/sources/platform/linux/PlatForm.class  ./BAJOSBOOT/classes/platform

avr32-linux:
	javac  -verbose -g:none -source 1.4 ./BAJOSBOOT/sources/platform/avr32-linux/PlatForm.java
	mv  ./BAJOSBOOT/sources/platform/avr32-linux/PlatForm.class  ./BAJOSBOOT/classes/platform

ngw100:
	javac -verbose -g:none -source 1.4 ./BAJOSBOOT/sources/platform/ngw100/PlatForm.java
	mv  ./BAJOSBOOT/sources/platform/ngw100/PlatForm.class  ./BAJOSBOOT/classes/platform

stk1000:
	javac -verbose -g:none -source 1.4 ./BAJOSBOOT/sources/platform/stk1000/PlatForm.java
	mv  ./BAJOSBOOT/sources/platform/stk1000/PlatForm.class  ./BAJOSBOOT/classes/platform





