#
# HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
# See the file "license.terms" for information on usage and redistribution of this file.
#

# java for 8 Bit uC test environment for linux
#=================================#
# make all bootclasses (bootpack)
# TOOLS, SOURCES AND FLAGS
ifeq  ($(TARGETHW), linux)
CC		= gcc
CPPFLAGS	= -DLINUX
all:	clean compile bootclasses
else
CC		= avr32-linux-gcc
CPPFLAGS	= -DAVR32LINUX
all:	clean compile bootclasses bootpack

bootpack:
	@echo -n generate file: avr32bootpack\; numClasses: 
	@printf %4d `echo $(JAVABOOTCLASSES)| wc -w` |tee  avr32bootpack
	@for i in $(JAVABOOTCLASSES) ;do printf %4d `cat $$i| wc -c` >> avr32bootpack;	cat $$i >> avr32bootpack;	done
	@echo "  length:" `ls -l avr32bootpack | cut -f5 -d' '`

endif

OPTIMIZATION		= -O3

# JAVA SOURCES
JAVABOOTSOURCES		+= 	$(LANG)/Math.java  \
			$(LANG)/Boolean.java $(LANG)/Byte.java\
			$(UTIL)/Random.java \
			$(LANG)/Exception.java  $(LANG)/Error.java \
			$(LANG)/ArithmeticException.java $(LANG)/ClassCastException.java \
			$(LANG)/RuntimeException.java $(LANG)/InterruptedException.java \
			$(UTIL)/Collection.java $(UTIL)/List.java \
			$(UTIL)/Stack.java $(UTIL)/Vector.java \
			$(LANG)/Runtime.java $(CONCURRENT)/Lock.java $(CONCURRENT)/InterruptThread.java

# only for stk1000!!!
ifeq  ($(TARGETHW), avr32-linux)
JAVABOOTSOURCES+=		${GRAPHICS}/Display.java ${GRAPHICS}/DisplayHSB.java \
			${GRAPHICS}/DisplayZBuffer.java ${GRAPHICS}/DisplayKonstanten.java \
			${GRAPHICS}/Point.java ${GRAPHICS}/Font.java \
			${GRAPHICS}/Line.java ${GRAPHICS}/Polyline.java \
			${GRAPHICS}/AffineMatrix.java ${GRAPHICS}/ProjectionMatrix.java
endif

PLATFORMCSOURCES	= $(APPPATH)LINUX/platform.c $(APPPATH)LINUX/native.c

avr32-linux:
	@:

linux:	
	@:

$(TARGETFILE):	${OBJFILES}
	@echo $(MSG_LINKING)
	$(VERBOSE_CMD) ${CC}  $(filter %.o,$+)   -o$(TARGETFILE) 



# Preprocess, compile & assemble: create object files from C source files.
%.o: %.c 
	@echo $(MSG_COMPILING)
	$(VERBOSE_CMD) $(CC) -c $(CPPFLAGS) -Wall  ${DEBUGGEN} -o $@ $<
	@touch $@
	$(VERBOSE_NL)


# javac -g:none B.java		// no debug info (line number table)
# javap -c -l -verbose B


EEE:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/EEE.class

compEEE:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} $(APPCLASSPATH)/EEE.java

FFF:
	./$(TARGETFILE)   $(JAVABOOTCLASSES) ${GRAPHICS}/Font.class 

compFFF:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} $(APPCLASSPATH)/FFF.java

GGG:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/GGG.class

compGGG:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} $(APPCLASSPATH)/GGG.java

HHH:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) 	$(APPCLASSPATH)/HHH.class

compHHH:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} $(APPCLASSPATH)/HHH.java

My:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/My.class $(APPCLASSPATH)/Ball.class

compMy:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH}  $(APPCLASSPATH)/My.java

Temp:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/Temperature.class

compTemp:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH}  $(APPCLASSPATH)/Temperature.java


T:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/T2.class $(APPCLASSPATH)/T1.class

compT:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} \
	  	$(APPCLASSPATH)/T1.java $(APPCLASSPATH)/T2.java
	
PC:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/Buffer.class \
		$(APPCLASSPATH)/ProducerConsumer.class 

compPC:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} \
		$(APPCLASSPATH)/ProducerConsumer.java

SchedulerTest:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) \
		$(APPCLASSPATH)/SchedulerTest.class 

compSchedulerTest:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} \
		$(APPCLASSPATH)/SchedulerTest.java

LockTest:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) \
		$(APPCLASSPATH)/LockTest.class $(APPCLASSPATH)/SpinLockTest.class 

compLockTest:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} \
		$(APPCLASSPATH)/LockTest.java $(APPCLASSPATH)/SpinLockTest.java

ADC:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/ADC.class

compADC:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} \
		$(APPCLASSPATH)/ADC.java

Interpreter:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/InterpreterTest.class $(APPCLASSPATH)/InpTest.class $(APPCLASSPATH)/ParamTest.class

compInterpreter:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} \
		$(APPCLASSPATH)/InterpreterTest.java
