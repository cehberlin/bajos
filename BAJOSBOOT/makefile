CPATH    = ./BAJOSBOOT/classes

JARGS    = -g:none -verbose  -source 1.4 -extdirs . -bootclasspath $(CPATH) -d $(CPATH) -sourcepath ./BAJOSBOOT/sources


boot:
	javac  $(JARGS) ./BAJOSBOOT/sources/java/io/*.java
	javac  $(JARGS) ./BAJOSBOOT/sources/java/lang/*.java
	javac  $(JARGS) ./BAJOSBOOT/sources/java/util/*.java

graphic:
	javac  $(JARGS) ./BAJOSBOOT/sources/java/graphics/*.java

all:
	javac  $(JARGS) ./BAJOSBOOT/sources/platform/PlatForm.java
	javac  $(JARGS) ./BAJOSBOOT/sources/java/io/*.java
	javac  $(JARGS) ./BAJOSBOOT/sources/java/lang/*.java
	javac  $(JARGS) ./BAJOSBOOT/sources/java/graphics/*.java
	javac  $(JARGS) ./BAJOSBOOT/sources/java/util/*.java


charon:
	javac  $(JARGS) ./BAJOSBOOT/sources/platform/charon/PlatForm.java 

arduinomega:
	javac  $(JARGS) ./BAJOSBOOT/sources/platform/arduinomega/PlatForm.java 


evk1100:
	javac  $(JARGS) ./BAJOSBOOT/sources/platform/evk1100/PlatForm.java


evk1104:
	javac  $(JARGS) ./BAJOSBOOT/sources/platform/evk1104/PlatForm.java

linux:
	javac  $(JARGS) ./BAJOSBOOT/sources/platform/linux/PlatForm.java

avr32-linux:
	javac  $(JARGS) ./BAJOSBOOT/sources/platform/avr32-linux/PlatForm.java

ngw100:
	javac  $(JARGS) ./BAJOSBOOT/sources/platform/ngw100/PlatForm.java

stk1000:
	javac  $(JARGS) ./BAJOSBOOT/sources/platform/stk1000/PlatForm.java

clean:
	rm ./BAJOSBOOT/classes/platform/PlatForm.class
	rm ./BAJOSBOOT/classes/java/io/*.class
	rm ./BAJOSBOOT/classes/java/lang/*.class
	rm ./BAJOSBOOT/classes/java/graphics/*.class
	rm ./BAJOSBOOT/classes/java/util/*.class








boot1:
	javac  -verbose -source 1.4  -g:none -classpath ./BAJOSBOOT/classes ./BAJOSBOOT/sources/java/io/*.java
	mv ./BAJOSBOOT/sources/java/io/*.class  ./BAJOSBOOT/classes/java/io
	javac  -verbose -source 1.4 -classpath ./BAJOSBOOT/classes -g:none ./BAJOSBOOT/sources/java/lang/*.java -extdirs . -bootclasspath .
	mv ./BAJOSBOOT/sources/java/lang/*.class  ./BAJOSBOOT/classes/java/lang
	javac  -verbose -source 1.4  -g:none  ./BAJOSBOOT/sources/java/util/*.java -extdirs . -bootclasspath . -classpath ./BAJOSBOOT/classes
	mv ./BAJOSBOOT/sources/java/util/*.class  ./BAJOSBOOT/classes/java/util

graphic1:
	javac  -verbose -source 1.4 -classpath ./BAJOSBOOT/classes -g:none ./BAJOSBOOT/sources/java/graphics/*.java
	mv ./BAJOSBOOT/sources/java/graphics/*.class  ./BAJOSBOOT/classes/java/graphics/






