SOURCES		= bajosavr.c classfile.c interpreter.c heap.c stack.c native.c scheduler.c
OBJECTS		= bajosavr.o classfile.o interpreter.o heap.o stack.o native.o scheduler.o
UNIXCC		= gcc
AVRCC		= avr-gcc
EXE		= baJVM128
DEBUGGEN	= DEBUG
CLASSPATH = /home/SWPROJEKTE/bajosavr
BOOTCLASSPATH=${CLASSPATH}/bajosboot/classes
LANG= ${BOOTCLASSPATH}/java/lang
IO=${BOOTCLASSPATH}/java/io
UTIL= ${BOOTCLASSPATH}/java/util
PLATFORM= ${BOOTCLASSPATH}/platform

unix:		${SOURCES}
	${UNIXCC} 	-Wall ${SOURCES} -DUNIX -D${DEBUGGEN}  -o${EXE} 

unixnd:		${SOURCES}
	${UNIXCC} 	${SOURCES} -Wall -DUNIX  -o${EXE} 


avr:		${SOURCES}
		${AVRCC}	${SOURCES}   -Wall DAVR -D${DEBUGGEN}  -mmcu=atmega128  -Wl,-Tdata=0x802000,--defsym=__heap_end=0x807fff  -Wl,-u,vfprintf -lprintf_flt  -o${EXE}
		avr-objcopy -O binary ${EXE}  ${EXE}.bin
		${AVRCC} --version
#		@rm ${EXE}
# -Wl,-u,vfprintf -lprintf_flt
avrnd:		${SOURCES}
		${AVRCC}	${SOURCES} -Wall  -DAVR  -mmcu=atmega128  -lm -Wl,-Tdata=0x802000,--defsym=__heap_end=0x807E00 -o${EXE}
		avr-objcopy -O binary ${EXE}  ${EXE}.bin
		@rm ${EXE}



A:
	./baJVM128    ${LANG}/Object.class \
	${PLATFORM}/Charon.class \
 	${LANG}/System.class \
 	${LANG}/Thread.class \
 	 ${LANG}/String.class \
  	 ${LANG}/Math.class \
  	 ${IO}/OutStream.class \
  	 ${IO}/InStream.class \
  	 ${LANG}/Integer.class \
  	 ${LANG}/StringBuilder.class \
    	A.class


compA:	
	javac -g:none -bootclasspath ${BOOTCLASSPATH}  A.java

compB:
	javac -verbose  -g:none -bootclasspath ${BOOTCLASSPATH}  B.java


B:	 
	./baJVM128    ${LANG}/Object.class \
	${PLATFORM}/Charon.class \
 	${LANG}/System.class \
	${LANG}/String.class \
	 ${LANG}/StringBuilder.class \
	 ${LANG}/Integer.class \
	 ${IO}/OutStream.class \
  	 ${IO}/InStream.class \
  	 B.class
	
compC:
	javac -verbose  -g:none -bootclasspath ${BOOTCLASSPATH}  C.java

C:	 
	./baJVM128    ${LANG}/Object.class \
	${PLATFORM}/Charon.class \
 	${LANG}/System.class \
	${LANG}/String.class \
	 ${LANG}/StringBuilder.class \
	 ${LANG}/Integer.class \
	 ${IO}/OutStream.class \
 	 ${IO}/InStream.class \
	C.class
	
compD:
	javac -verbose  -g:none -bootclasspath ${BOOTCLASSPATH}  D.java


D:	 
	./baJVM128    ${LANG}/Object.class \
	${PLATFORM}/Charon.class \
 	${LANG}/System.class \
	${LANG}/String.class \
	 ${IO}/OutStream.class \
  	 ${IO}/InStream.class \
  	 D.class

EEE:	 
	./baJVM128    ${LANG}/Object.class 	${PLATFORM}/Charon.class ${LANG}/System.class \
 	 ${LANG}/Math.class ${LANG}/Thread.class	 ${LANG}/String.class \
 	${IO}/OutStream.class   	 ${IO}/InStream.class  ${LANG}/Integer.class \
  	EEE.class

compEEE:
	javac -g:none -bootclasspath ${BOOTCLASSPATH}  EEE.java

FFF:
	./baJVM128  	 FFF.class  ${LANG}/Object.class    ${PLATFORM}/Charon.class  ${LANG}/System.class \
	${IO}/OutStream.class   	 ${IO}/InStream.class   ${LANG}/StringBuilder.class \
 	${LANG}/Math.class   ${LANG}/Integer.class  ${LANG}/String.class

compFFF:
	javac -g:none -bootclasspath ${BOOTCLASSPATH}  FFF.java

GGG:	 
	./baJVM128    ${LANG}/Object.class 	${PLATFORM}/Charon.class  	${LANG}/System.class \
 	${LANG}/Thread.class  	 ${LANG}/String.class   	 ${LANG}/Math.class \
 	${IO}/OutStream.class   	 ${IO}/InStream.class \
  	GGG.class

compGGG:
	javac -g:none -bootclasspath ${BOOTCLASSPATH}  GGG.java

HHH:	 
	./baJVM128    ${LANG}/Object.class 	${PLATFORM}/Charon.class  	${LANG}/System.class \
 	${LANG}/Thread.class  	 ${LANG}/String.class   	 ${LANG}/Math.class \
 	${IO}/OutStream.class   	 ${IO}/InStream.class \
  	HHH.class

compHHH:
	javac -g:none -bootclasspath ${BOOTCLASSPATH}  HHH.java

My:	 
	./baJVM128    ${LANG}/Object.class 	${PLATFORM}/Charon.class  	${LANG}/System.class \
 	${LANG}/Thread.class  	 ${LANG}/String.class   	 ${LANG}/Math.class  ${LANG}/Throwable.class\
 	${LANG}/Exception.class ${LANG}/Integer.class ${LANG}/StringBuilder.class \
 	${IO}/OutStream.class   	 ${IO}/InStream.class \
  	My.class Ball.class

compMy:
	javac -g:none -bootclasspath ${BOOTCLASSPATH}  My.java

T:	 
	./baJVM128    ${LANG}/Object.class 	${PLATFORM}/Charon.class  	${LANG}/System.class \
	${IO}/OutStream.class   	 ${IO}/InStream.class   ${LANG}/Integer.class ${LANG}/StringBuilder.class ${LANG}/String.class \
	T2.class T1.class

compT:
	javac -g:none -bootclasspath ${BOOTCLASSPATH}  T1.java T2.java
	
PC:	 
	./baJVM128     	${PLATFORM}/Charon.class  	${LANG}/System.class \
 	${LANG}/Thread.class  	 ${LANG}/String.class     ${LANG}/Throwable.class\
 	${LANG}/Exception.class ${LANG}/Integer.class ${LANG}/StringBuilder.class \
 	${IO}/OutStream.class ${LANG}/InterruptedException.class   	 ${IO}/InStream.class \
  	ProducerConsumer.class Buffer.class ${LANG}/Object.class

compPC:
	javac -g:none -bootclasspath ${BOOTCLASSPATH}  ProducerConsumer.java
		
clean:
	rm -f *.o ${EXE} *.bin

nodeb:	${SOURCES}
	${UNIXCC} 	${SOURCES} -DUNIX -o${EXE} 
