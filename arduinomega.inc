#
# HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
# See the file "license.terms" for information on usage and redistribution of this file.
#

# java for 8 Bit uC atmega1280 at arduinoMega
#=================================#
# working without monitor (standalone application)
# make all am -> generates bajos.bin (jvm, boot classes and appl classes) and burns at arduinoMega
#
# working with monitor bamo128  (cs.ba-berlin.de) -> first load bamo128 in boot section
# make all am withmon -> generates bajvm.bin, avr8bootpack and amapp
# load bajvm.bin avr8bootpack and amapp in flash, then start bajos with the "go" command of  monitor
# fuses setting
# make writeFusesA	-> start after reset at address 0000
# make writeFusesM	-> start after reset vom boot section 0xf000 (in words)

# TOOLS
BINDIR		= /opt/cross/as4e-ide/plugins/com.atmel.avr.toolchains.linux.x86_64_*/os/linux/x86_64/bin/
CC		= $(BINDIR)avr-gcc
OBJCOPY		= $(BINDIR)avr-objcopy
PROGRAM		= avrdude
# flasher means the avrdude device which you are using for flashing
#FLASHER	= stk500 -P/dev/ttyUSB0
#FLASHER 	= stk200 
FLASHER 	= avrispmkII -P usb 

# DEVICE
ARCH			= AVR5
PART			= atmega1280
DEFS 			+= -DAVR8 -DAM -DF_CPU=16000000 
OPTIMIZATION		= -Os

# for standalone application
CLASSFILEBASE		= 0x8000
#0xC480
# fix address for class files (in words), make it better

# JAVA SOURCES
#JAVAAPPSOURCES		= javatests/LockSimple.java
JAVAAPPSOURCES		= javatests/AM.java
#JAVAAPPSOURCES		= javatests/T1.java javatests/T2.java
#JAVAAPPSOURCES		= javatests/SchedulerTest.java
#JAVAAPPSOURCES		= javatests/InterruptTest.java

# JAVA SOURCES
#JAVABOOTSOURCES	+= 

#############
# TINYBAJOS #
#############
ifeq ($(findstring TINYBAJOS,$(MAKECMDGOALS)),TINYBAJOS)
	#Shrink bajos, but looses features
	OPTIMIZATION		+= -funsigned-char -funsigned-bitfields \
	                     	-fpack-struct -fshort-enums \
	                     	-ffreestanding -g -gdwarf-2 \
	                     	-fno-inline-small-functions -fno-split-wide-types -fno-tree-scev-cprop \
                        	-ffunction-sections \
                        	-Wl,--relax,--gc-sections 
else
	PLATFORMASMSOURCES	+= $(APPPATH)ARDUINOMEGA/routines_interrupt.asm
endif

PLATFORMCSOURCES	= $(APPPATH)ARDUINOMEGA/platform.c $(APPPATH)ARDUINOMEGA/native.c
PLATFORMASMSOURCES	+= $(APPPATH)ARDUINOMEGA/routines.asm $(APPPATH)ARDUINOMEGA/routines_stack.asm 

OBJFILES		+=	$(PLATFORMASMSOURCES:.asm=.o)

# GOALS

ifeq ($(findstring withmon,$(MAKECMDGOALS)),withmon)
DEFS+= -DWITHMON
DEFS+= -Wl,--defsym=__stack=0x802100

all:	clean  bootclasses compAM bootpack app compile
	@echo created $(BIN) arv8bootpack amapp

$(TARGETFILE):	${OBJFILES}
	@echo $(MSG_LINKING)
	$(VERBOSE_CMD)${CC} $(filter %.o,$+) -mmcu=$(PART) -architecture=$(ARCH) $(DEFS)  -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm -o$(TARGETFILE)
	@echo $(MSG_BINARY_IMAGE)
	$(VERBOSE_CMD) $(OBJCOPY) -O binary $(TARGETFILE) $(BIN)
	@${CC} --version
#	$(VERBOSE_CMD)${CC} $(filter %.o,$+) -mmcu=$(PART) -architecture=$(ARCH)   -Wl,--section-start,.data=0x800200,--defsym=__heap_start=0x800c00,--defsym=__heap_end=0x802080,--defsym=__stack=0x802100    -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm -o$(TARGETFILE)
##	@rm ${EXE}
else
all:	clean  bootclasses compAM bootpack app compile bajospack burnbajos

$(TARGETFILE):	${OBJFILES}
	@echo $(MSG_LINKING)
	$(VERBOSE_CMD)${CC} $(filter %.o,$+) -mmcu=$(PART) -architecture=$(ARCH)   -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm -o$(TARGETFILE) -Wl,--defsym=__stack=0x802200 
	@echo $(MSG_BINARY_IMAGE)
	$(VERBOSE_CMD) $(OBJCOPY) -O binary $(TARGETFILE) $(BIN)

bajospack:
	$(VERBOSE_CMD) cat $(BIN) > bajos.bin
	$(VERBOSE_CMD)(i=`ls -l $(BIN) | cut -f5 -d' '`; limit=`expr 2 \* $$(($(CLASSFILEBASE)))`;while [ "$$i" -lt "$$limit" ];  do  i=$$(($$i+1));echo -n 'x' >> bajos.bin;  done)
	cat avr8bootpack >> bajos.bin
	cat amapp >> bajos.bin
	@echo Creating binary image to \`bajos.bin\'	.
	@${CC} --version

endif	#withmon

withmon:
	@:
arduinoMega:
	@:
am:
	@:
AM:	
	@:

#print usage of ram and flash
printsize:
	#avr-size  --mcu=$(PART) $(TARGETFILE)
	avr-size --mcu=$(PART) --format=avr $(TARGETFILE)
	#avr-nm --size-sort --print-size $(TARGETFILE) # more detailed information about size of objects functions variables

burnbajos:
	$(PROGRAM) -p m1280 -c $(FLASHER) -D -e -Uflash:w:bajos.bin:a


.PHONY:	writeFusesM		# arduinoMega with monitor starts at 0xf000
writeFusesM:	# 4 k word boot section, start from boot section after reset
	$(PROGRAM) -p m1280 -c $(FLASHER) -Ulfuse:w:0xff:m
	$(PROGRAM) -p m1280 -c $(FLASHER) -Uhfuse:w:0xd8:m
	$(PROGRAM) -p m1280 -c $(FLASHER) -Uefuse:w:0xf5:m

.PHONY:	writeFusesA		# arduinoMega without monitpts starts at 0x0000
writeFusesA:	# 4 k word boot section, start from address 0000 after reset
	$(PROGRAM) -p m1280 -c $(FLASHER) -Ulfuse:w:0xff:m
	$(PROGRAM) -p m1280 -c $(FLASHER) -Uhfuse:w:0xd9:m
	$(PROGRAM) -p m1280 -c $(FLASHER) -Uefuse:w:0xf5:m

.PHONY:	bootpack
bootpack:
	@echo -n generate file: avr8bootpack\; numClasses: 
	@printf %4d `echo $(JAVABOOTCLASSES)| wc -w` |tee  avr8bootpack
	$(VERBOSE_CMD) for i in $(JAVABOOTCLASSES) ;do printf %4d `cat $$i| wc -c` >> avr8bootpack;	cat $$i >> avr8bootpack;	done
	@echo "  length:" `ls -l avr8bootpack | cut -f5 -d' '`

.PHONY:	app
app:
	@echo -n generate file: amapp\; numClasses: 
	@printf %4d `echo $(JAVAAPPCLASSES)| wc -w` |tee  amapp
	$(VERBOSE_CMD) for i in $(JAVAAPPCLASSES) ;do printf %4d `cat $$i| wc -c` >> amapp;	cat $$i >> amapp;	done
	@echo "  length:" `ls -l amapp | cut -f5 -d' '`

%.o: %.c
	@echo $(MSG_COMPILING)
	$(VERBOSE_CMD) ${CC} -c -Wall $(DEFS) ${DEBUGGEN} ${OPTIMIZATION}   -mmcu=$(PART) -o $@ $<   #-mtiny-stack

%o : %asm
	@echo $(MSG_COMPILING)
	$(VERBOSE_CMD) $(CC) -x assembler-with-cpp -Wall ${DEFS} ${DEBUGGEN}  -mmcu=$(PART) -gstabs -Wa,-ahlms=$(<:.asm=.lst)  -c $< -o $@

# java app
compAM:	
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) $(JAVAAPPSOURCES)
