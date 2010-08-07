#
# HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
# See the file "license.terms" for information on usage and redistribution of this file.
#

# java for 8 Bit uC atxmega128 at xplain board
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
CC		= avr-gcc
OBJCOPY		= avr-objcopy
PROGRAM		= avrdude

# DEVICE
#ARCH			= AVR7
PART			= atxmega128a1
DEFS 			= -DAVR8 -DXPLAIN -DF_CPU=16000000 -IXMEGA -IXMEGA/include -include XMEGA/autoconf.h
DEFS+= -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -fshort-enums  -Os -gdwarf-2 -mrelax -ffunction-sections -fdata-sections

OPTIMIZATION		= 

# for standalone application
CLASSFILEBASE		= 0xB000
# fix address for class files (in words), make it better

# JAVA SOURCES
JAVAAPPSOURCES		= javatests/XP.java




PLATFORMCSOURCES	= XMEGA/native.c XMEGA/generic_memcpy.c \
			  XMEGA/generic_strlen.c  XMEGA/port.c \
			  XMEGA/generic_memset.c  XMEGA/init.c \
			  XMEGA/uart_xmega_ctrl.c XMEGA/delay.c \
			  XMEGA/generic_strcmp.c  XMEGA/platform.c XMEGA/sysclk.c \
			  XMEGA/xmega_pr.c

PLATFORMASMSOURCES	= XMEGA/ccp.S  XMEGA/entry.S

OBJFILES		+=	$(PLATFORMASMSOURCES:.S=.o)

# GOALS
ifeq ($(findstring withmon,$(MAKECMDGOALS)),withmon)
DEFS+= -DWITHMON
all:	clean  bootclasses compAM bootpack app compile
	@echo created $(BIN) arv8bootpack amapp

$(TARGETFILE):	${OBJFILES}
	@echo $(MSG_LINKING)
	$(VERBOSE_CMD)${CC} $(filter %.o,$+) -mmcu=$(PART) -architecture=$(ARCH)   -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm -o$(TARGETFILE) -Wl,--defsym=__stack=0x802100 
	@echo $(MSG_BINARY_IMAGE)
	$(VERBOSE_CMD) $(OBJCOPY) -O binary $(TARGETFILE) $(BIN)
	@${CC} --version
#	$(VERBOSE_CMD)${CC} $(filter %.o,$+) -mmcu=$(PART) -architecture=$(ARCH)   -Wl,--section-start,.data=0x800200,--defsym=__heap_start=0x800c00,--defsym=__heap_end=0x802080,--defsym=__stack=0x802100    -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm -o$(TARGETFILE)
##	@rm ${EXE}
else
all:	clean  bootclasses compAM bootpack app compile bajospack burnbajos

$(TARGETFILE):	${OBJFILES}
	@echo $(MSG_LINKING)
	$(VERBOSE_CMD)${CC} $(filter %.o,$+) -mmcu=$(PART) -architecture=$(ARCH)   -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm -o$(TARGETFILE) -Wl,--defsym=__stack=0x802100 "/usr/lib/gcc/avr/4.3.3/avrxmega7/libgcc.a"
 
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
xplain:
	@:

burnbajos:
	$(PROGRAM) -p m1280 -c avrispmkII -D -e -P usb  -Uflash:w:bajos.bin:a


.PHONY:	writeFusesM		# arduinoMega with monitor starts at 0xf000
writeFusesM:	# 4 k word boot section, start from boot section after reset
	$(PROGRAM) -p m1280 -c avrispmkII  -P usb  -Ulfuse:w:0xff:m
	$(PROGRAM) -p m1280 -c avrispmkII  -P usb  -Uhfuse:w:0xd8:m
	$(PROGRAM) -p m1280 -c avrispmkII -P usb  -Uefuse:w:0xf5:m

.PHONY:	writeFusesA		# arduinoMega without monitpts starts at 0x0000
writeFusesA:	# 4 k word boot section, start from address 0000 after reset
	$(PROGRAM) -p m1280 -c avrispmkII  -P usb  -Ulfuse:w:0xff:m
	$(PROGRAM) -p m1280 -c avrispmkII  -P usb  -Uhfuse:w:0xd9:m
	$(PROGRAM) -p m1280 -c avrispmkII  -P usb  -Uefuse:w:0xf5:m

.PHONY:	bootpack
bootpack:
	@echo -n generate file: avr8bootpack\; numClasses: 
	@printf %4d `echo $(JAVABOOTCLASSES)| wc -w` |tee  avr8bootpack
	@for i in $(JAVABOOTCLASSES) ;do printf %4d `cat $$i| wc -c` >> avr8bootpack;	cat $$i >> avr8bootpack;	done
	@echo "  length:" `ls -l avr8bootpack | cut -f5 -d' '`

.PHONY:	app
app:
	@echo -n generate file: amapp\; numClasses: 
	@printf %4d `echo $(JAVAAPPCLASSES)| wc -w` |tee  amapp
	@for i in $(JAVAAPPCLASSES) ;do printf %4d `cat $$i| wc -c` >> amapp;	cat $$i >> amapp;	done
	@echo "  length:" `ls -l amapp | cut -f5 -d' '`

%.o: %.c
	@echo $(MSG_COMPILING)
	$(VERBOSE_CMD) ${CC} -c -Wall $(DEFS) ${DEBUGGEN} ${OPTIMIZATION}   -mmcu=$(PART) -o $@ $<   #-mtiny-stack

%o : %S
	@echo $(MSG_COMPILING)
	$(VERBOSE_CMD) $(CC) -x assembler-with-cpp -D__ASSEMBLY__ -Wall ${DEFS} ${DEBUGGEN}  -mmcu=$(PART)  -c $< -o $@

# java app
compXPLAIN:	
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) $(JAVAAPPSOURCES)
