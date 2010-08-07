# Hey -  this is a -*- makefile -*- -> for BAJOS
# HWR/Fachbereich Berufsakademie (BA) - Java Operating System for Microcontroller
# atmega128 (CharonII), linux, avr32UCA (EVK1100), avr32AP7000(NGW100,STK1000)
# goals
# avr8 linux avr32-linux evk1100 ngw100 stk1000 clean java (for tests)
# make clean goal
# make compile goal
# make debug compile verbose goal
# make all goal
# make clobber goal
# make program stk1000
# make A linux
# make compA linux
# make bootclasses goal
# ...
# avr8 dump
# ....
# avr8 sizeg
# ...
# evk1100 ngw100 or stk1000
# xxx cpuinfo
# xxx cpuinfo
# xxx halt
# xxx reset
# xxx run
# xxx readregs
# xxx chiperase
# xxx program
# verbose
# clean

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# FUNCTIONS
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
LastWord  = $(if $(1),$(word $(words $(1)) ,$(1)))
FirstWord = $(if $(1),$(word 1,$(1)))
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# CHECK COMMAND LINE
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# test command line - make it better
$(if   $(filter avr8 linux avr32-linux evk1100 stk1000 ngw100,$(MAKECMDGOALS)), ,$(error wrong or incomplete command line))

ifneq "1"  "$(words $(filter avr8 evk1100 ngw100 stk1000 linux avr32-linux,$(MAKECMDGOALS)))"
$(error only one target hardware accepted)
endif

TARGETHW = noTarget
ifeq "1"  "$(words $(filter avr8 evk1100 ngw100 stk1000 linux avr32-linux,$(MAKECMDGOALS)))"
TARGETHW = $(filter avr8 evk1100 ngw100 stk1000 linux avr32-linux java,$(MAKECMDGOALS))
endif

ifeq "1"  "$(words $(MAKECMDGOALS))"
$(error what is to do with $(TARGETHW): clean compile program ...)
endif


# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# ENVIRONMENT SETTINGS
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
#the serial communication lines
TTY		= /dev/ttyS0
# C paths
APPPATH 	= ./
AVR8ROOT	=
AVR8INC		=
AVR32ROOT	= /usr/bin/
AVR32BIN 	= $(AVR32DIR)
AVR32LINUXGCC	= /usr/avr32-linux/bin/avr32-linux-gcc

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# JAVA-SOURCES AND TARGETS
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# java paths
CLASSPATH	= $(APPPATH)
APPCLASSPATH	= $(CLASSPATH)javatests
BOOTCLASSPATH	= ${CLASSPATH}BAJOSBOOT/classes/
LANG		= ${BOOTCLASSPATH}java/lang
IO		= ${BOOTCLASSPATH}java/io
UTIL		= ${BOOTCLASSPATH}java/util
JPLATFORM	= ${BOOTCLASSPATH}platform
GRAPHICS	= ${BOOTCLASSPATH}java/graphics
# java system sources
BOOTSOURCES	= 	$(JPLATFORM)/PlatForm.java \
			$(LANG)/String.class $(LANG)/StringBuffer.java \
			$(LANG)/Object.java $(LANG)/System.java \
			$(LANG)/Thread.java $(LANG)/Throwable.java \
			$(LANG)/Math.java $(LANG)/Float.java \
			$(LANG)/Boolean.java $(LANG)/Byte.java\
			$(LANG)/Integer.java $(UTIL)/Random.java \
			$(LANG)/Exception.java  $(LANG)/Error.java \
			$(LANG)/ArithmeticException.java $(LANG)/ClassCastException.java \
			$(LANG)/ArrayIndexOutOfBoundsException.java \
			$(LANG)/RuntimeException.java $(LANG)/InterruptedException.java \
			$(UTIL)/Collection.java $(UTIL)/List.java \
			$(UTIL)/Stack.java $(UTIL)/Vector.java \
			$(LANG)/Runtime.java \
			$(IO)/OutStream.java $(IO)/InStream.java 
ifeq  ($(TARGETHW), stk1000)
BOOTSOURCES+=		${GRAPHICS}/Display.java ${GRAPHICS}/DisplayHSB.java \
			${GRAPHICS}/DisplayZBuffer.java ${GRAPHICS}/DisplayKonstanten.java \
			${GRAPHICS}/Point.java ${GRAPHICS}/Font.java \
			${GRAPHICS}/Line.java ${GRAPHICS}/Polyline.java \
			${GRAPHICS}/AffineMatrix.java ${GRAPHICS}/ProjectionMatrix.java
endif

# add appl. classes for avr32linux, because there is an error (or timing problem 
## betwenn minikermit and bajos!!
#			$(UTIL)/AbstractCollection.java $(UTIL)/AbstractList.java \
#${LANG}/CharSequence.java $(LANG)/StringUtils.java 
#			$(LANG)/StringBuilder.java \
#			$(UTIL)/RandomAccess.java \

# a small subset of java system sources for the small controller
AVR8BOOTSOURCES =	$(JPLATFORM)/PlatForm.java $(LANG)/Throwable.java\
			$(LANG)/String.class $(LANG)/StringBuffer.java \
			$(LANG)/Integer.java $(LANG)/Float.java \
			$(LANG)/Object.java $(LANG)/System.java \
			$(IO)/OutStream.java $(IO)/InStream.java \
			$(LANG)/Thread.java $(LANG)/Exception.java \
			$(LANG)/ArrayIndexOutOfBoundsException.java

#			javatests/Temperature.java
#			 $(LANG)/Math.java \
#			$(LANG)/StringBuilder.java

BOOTCLASSES	= $(BOOTSOURCES:.java=.class)			
AVR8BOOTCLASSES	= $(AVR8BOOTSOURCES:.java=.class)
#java binaries and flags
JAVACOMP		= javac
JAVACOMPFLAGS		= -g:none -source 1.4 -verbose
JAVACOMPBOOTCLASSES	= -bootclasspath ${BOOTCLASSPATH} -classpath BAJOSBOOT/classes -extdirs .

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# C-SOURCES AND TARGETS
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
BAJOSSOURCES	= bajvm.c classfile.c interpreter.c heap.c stack.c scheduler.c \
		nativedispatch.c $(APPPATH)JAVALANGNATIVE/langnative.c
AVR8SOURCES	= $(APPPATH)AVR8/lcd.c $(APPPATH)AVR8/shift.c $(APPPATH)AVR8/ds182x.c \
		$(APPPATH)AVR8/platform.c $(APPPATH)AVR8/native.c
UC3ASOURCES 	= $(APPPATH)EVK1100/intc.c $(APPPATH)EVK1100/pm.c \
		$(APPPATH)EVK1100/rtc.c $(APPPATH)EVK1100/pwm.c \
		$(APPPATH)EVK1100/dip204.c $(APPPATH)EVK1100/spi.c \
		$(APPPATH)EVK1100/gpio.c $(APPPATH)EVK1100/adc.c\
		$(APPPATH)EVK1100/flashc.c $(APPPATH)EVK1100/usart.c \
		$(APPPATH)EVK1100/sdramc.c $(APPPATH)EVK1100/platform.c $(APPPATH)EVK1100/native.c
NGW100SOURCES	= $(APPPATH)NGW100/pio.c $(APPPATH)NGW100/intc.c \
		$(APPPATH)NGW100/hsdramc.c $(APPPATH)NGW100/pm_at32ap7000.c \
		$(APPPATH)NGW100/usart.c \
		$(APPPATH)NGW100/platform.c  $(APPPATH)NGW100/native.c
ASSSOURCESNGW100	= $(APPPATH)NGW100/exception.S $(APPPATH)NGW100/crt0.S
STK1000SOURCES	= $(APPPATH)STK1000/pio.c $(APPPATH)STK1000/lcdc.c $(APPPATH)STK1000/usart.c\
		$(APPPATH)STK1000/lib2d.c $(APPPATH)STK1000/fontlib.c \
		$(APPPATH)STK1000/ltv350qv.c $(APPPATH)STK1000/at32stk1000.c \
		$(APPPATH)STK1000/pm.c $(APPPATH)STK1000/spi.c $(APPPATH)STK1000/intc.c \
		$(APPPATH)STK1000/utils.c $(APPPATH)STK1000/sdram.c \
		$(APPPATH)STK1000/bmplib.c $(APPPATH)STK1000/platform.c $(APPPATH)STK1000/native.c
LINUXSOURCES	= $(APPPATH)LINUX/platform.c $(APPPATH)LINUX/native.c
ASSSOURCESUC3A	= $(APPPATH)/EVK1100/trampoline.S $(APPPATH)/EVK1100/exception.S \

#		$(APPPATH)/EVK1100/crt0.S

TARGETFILE	= $(basename $(call FirstWord,$(BAJOSSOURCES)))

#$(PART)-$(TARGET)
LSS		= $(TARGETFILE:$(TGTTYPE)=.lss)
SYM		= $(TARGETFILE:$(TGTTYPE)=.sym)
HEX		= $(TARGETFILE:$(TGTTYPE)=.hex)
BIN		= $(TARGETFILE:$(TGTTYPE)=.bin)


# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# BINUTILS-BINARIES
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
RM		= rm -Rf
ifeq ($(findstring avr8,$(MAKECMDGOALS)),avr8)
OBJDUMP		= $(AVR8ROOT)avr-objdump
OBJCOPY		= $(AVR8ROOT)avr-objcopy
SIZE		= $(AVR8ROOT)avr-size
else
OBJDUMP		= $(AVR32BIN)avr32-objdump
OBJCOPY		= $(AVR32BIN)avr32-objcopy
SIZE		= $(AVR32BIN)avr32-size
PROGRAM		= $(AVR32BIN)avr32program
endif
SLEEP		= sleep
SLEEPUSB	= 9

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
#  SOME FLAGS
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
DEBUGGEN	= $(if $(filter debug ,$(MAKECMDGOALS)), -DDEBUG,)
ISPFLAGS	= -device at32$(PART) -hardware usb -operation

# Display main executed commands.
ifneq ($(findstring verbose,$(MAKECMDGOALS)),verbose)
# Prefix displaying the following command if and only if verbose is a goal.
VERBOSE_CMD	= @
# New line displayed if and only if verbose is a goal.
VERBOSE_NL	=
else
VERBOSE_CMD	= 
VERBOSE_NL	= @echo
verbose:
	@:
endif

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# THE RULES
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
ifeq  ($(TARGETHW), avr8)
OBJFILES	= $(BAJOSSOURCES:.c=.o) $(AVR8SOURCES:.c=.o)
ARCH		= AVR5
PART		= atmega128
CC		= avr-gcc
TEXTSEGMENT	= 0x100
STACKSEGMENT	= 0x4000

avr8:
	@:

all:	clean compile bootclasses bootpack

compile: $(TARGETFILE)

$(TARGETFILE):	${OBJFILES}
	@echo $(MSG_LINKING)
	$(VERBOSE_CMD)${CC} $(filter %.o,$+) -mmcu=$(PART) -architecture=$(ARCH)   -Wl,--section-start,.data=0x800900,--defsym=__heap_start=0x802000,--defsym=__heap_end=0x807fff,--defsym=__stack=0x800800    -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm -o$(TARGETFILE)
#		 -Wl,--defsym=__heap_start=0x802000,--defsym=__heap_end=0x807fff    -o$@
# ... <- stack 0x1100 data -> bss ->  0x2200 heap->
	@echo $(MSG_BINARY_IMAGE)
	$(VERBOSE_CMD) $(AVR8ROOT)avr-objcopy -O binary $(TARGETFILE)  $(BIN)
	@${CC} --version
#		@rm ${EXE}
# -Wl,-u,vfprintf -lprintf_flt

bootpack:
	@echo -n generate file: avr8bootpack\; numClasses: 
	@printf %4d `echo $(AVR8BOOTCLASSES)| wc -w` |tee  avr8bootpack
	@for i in $(AVR8BOOTCLASSES) ;do printf %4d `cat $$i| wc -c` >> avr8bootpack;	cat $$i >> avr8bootpack;	done
	@echo "  length:" `ls -l avr8bootpack | cut -f5 -d' '`

%.o: %.c
	@echo $(MSG_COMPILING)
	$(VERBOSE_CMD) ${CC} $(AVR8INC) -c   -Wall -DAVR8 ${DEBUGGEN}  -mmcu=$(PART) -o $@ $<
endif #avr8


ifeq ($(filter $(TARGETHW) ,linux avr32-linux), $(TARGETHW))
OBJFILES	= $(BAJOSSOURCES:.c=.o) $(LINUXSOURCES:.c=.o) 
ifeq  ($(TARGETHW), linux)
CC		= gcc
CPPFLAGS	= -DLINUX
endif

ifeq  ($(TARGETHW), avr32-linux)
OBJFILES	= $(BAJOSSOURCES:.c=.o) $(LINUXSOURCES:.c=.o) 
CC		= $(AVR32LINUXGCC)
CPPFLAGS	= -DAVR32LINUX
all: clean compile bootpack 


bootpack:
	@echo -n generate file: avr32bootpack\; numClasses: 
	@printf %4d `echo $(BOOTCLASSES)| wc -w` |tee  avr32bootpack
	@for i in $(BOOTCLASSES) ;do printf %4d `cat $$i| wc -c` >> avr32bootpack;	cat $$i >> avr32bootpack;	done
	@echo "  length:" `ls -l avr32bootpack | cut -f5 -d' '`
else
bootpack:
	@:

all: clean compile bootclasses  bootpack 

endif


compile: $(TARGETFILE)

$(TARGETFILE):	${OBJFILES}
	@echo $(MSG_LINKING)
	$(VERBOSE_CMD) ${CC}  $(filter %.o,$+)   -o$(TARGETFILE) 

avr32-linux:
	@:

linux:	
	@:

# Preprocess, compile & assemble: create object files from C source files.
%.o: %.c 
	@echo $(MSG_COMPILING)
	$(VERBOSE_CMD) $(CC) -c $(CPPFLAGS) -Wall  ${DEBUGGEN} -o $@ $<
	@touch $@
	$(VERBOSE_NL)
endif #linux avr32-linux

ifeq  ($(TARGETHW), evk1100)
OBJFILES  = $(BAJOSSOURCES:.c=.o) $(ASSSOURCESUC3A:.S=.o) $(UC3ASOURCES:.c=.o)
CC		= $(AVR32BIN)avr32-gcc
ARCH		= ucr1
# Part: {none|ap7xxx|uc3xxxxx}
#PART		= uc3a0512
PART = uc3a0512es
# Flash memories: [{cfi|internal}@address,size]...
FLASH		= internal@0x80000000,512Kb
# Clock source to use when programming: [{xtal|extclk|int}]
PROG_CLOCK	= xtal
DEFS		= -D BOARD=EVK1100
ifeq ($(findstring withmon,$(MAKECMDGOALS)),withmon)
# Linker script file if any
#LINKER_SCRIPT	= $(APPPATH)EVK1100/link_uc3a0512.lds
DEFS+= -D WITHMON
LINKER_SCRIPT = $(APPPATH)EVK1100/link_uc3a0512withmon.lds
else
LINKER_SCRIPT	= $(APPPATH)EVK1100/link_uc3a0512.lds
endif
# Options to request or suppress warnings: [-fsyntax-only] [-pedantic[-errors]] [-w] [-Wwarning...]
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
WARNINGS	= -Wall
# Options for debugging: [-g]...
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
DEBUG		= -g
# Options that control optimization: [-O[0|1|2|3|s]]...
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
OPTIMIZATION	= -O0 -ffunction-sections -fdata-sections
# Extra flags to use when linking
#LD_EXTRA_FLAGS = -Wl,--gc-sections -nostdlib -Wl,-e,_trampoline
LD_EXTRA_FLAGS	= -Wl,--gc-sections  -Wl,-e,_trampoline

CPPFLAGS	= -march=$(ARCH) -DEVK1100 -DAVR32UC3A -mpart=$(PART) $(WARNINGS) $(DEFS) \
            $(PLATFORM_INC_PATH:%=-I%) $(INC_PATH:%=-I%) $(CPP_EXTRA_FLAGS)

#CC        = avr32-gcc
CFLAGS		= $(DEBUGGEN) $(OPTIMIZATION) $(C_EXTRA_FLAGS) \
		$(PLATFORM_INC_PATH:%=-Wa,-I%)  $(AS_EXTRA_FLAGS)
ASFLAGS		= $(DEBUGGEN) \
		$(PLATFORM_INC_PATH:%=-Wa,-I%) $(AS_EXTRA_FLAGS)
LDFLAGS		= -march=$(ARCH) -mpart=$(PART) \
		$(LIB_PATH:%=-L%) $(LINKER_SCRIPT:%=-T%) $(LD_EXTRA_FLAGS)
LOADLIBES	= -lc
LDLIBS		= $(LIBS:%=-l%)
withmon:	
	@:

evk1100:
	@:

compile: $(TARGETFILE)

$(TARGETFILE): $(OBJFILES)
	@echo $(MSG_LINKING)
	$(VERBOSE_CMD)$(CC) $(LDFLAGS) $(filter %.o,$+) $(LOADLIBES) $(LDLIBS) -o $(TARGETFILE)
	@echo $(MSG_BINARY_IMAGE)
	$(OBJCOPY) -O binary $(TARGETFILE)  $(TARGETFILE).bin
	$(VERBOSE_NL)

# Preprocess, compile & assemble: create object files from C source files.
%.o: %.c
	@echo $(MSG_COMPILING)
	$(VERBOSE_CMD)$(CC) -c   $(CPPFLAGS)  $(CFLAGS) -o $@ $<
	@touch $@
	$(VERBOSE_NL)

# Preprocess & assemble: create object files from assembler source files.
%.o: %.S %.asm
	@echo $(MSG_ASSEMBLING)
	$(VERBOSE_CMD)$(CC) -c $(CPPFLAGS) $(ASFLAGS) -o $@ $<
	@touch $@
	$(VERBOSE_NL)

all:	clean compile bootclasses  program progbootpack

# Program MCU memory from ELF output file.
.PHONY: program

program: $(TARGETFILE)
	@echo
	@echo $(MSG_PROGRAMMING)
	$(VERBOSE_CMD) $(PROGRAM) program $(FLASH:%=-f%) $(PROG_CLOCK:%=-c%) -e -v -R $(if $(findstring run,$(MAKECMDGOALS)),-r) $(TARGETFILE)
	sleep 2

progbootpack:
	$(VERBOSE_CMD) printf %4d `echo $(BOOTCLASSES)| wc -w` > mytemp
	$(VERBOSE_CMD) for i in $(BOOTCLASSES) ;do printf %4d `cat $$i| wc -c` >> mytemp;	cat $$i >> mytemp;	done
	$(VERBOSE_CMD) $(PROGRAM) program  -F bin -O 0x80040000  -finternal@0x80000000,512Kb -cxtal -e -v -R mytemp
	@$(SLEEP) $(SLEEPUSB)
endif
# endif evk1100
#	$(VERBOSE_CMD) $(PROGRAM) program  -F bin -O 0x80040000  #-finternal@0x80000000,512Kb -cxtal -e -v -R $(BOOTPACK)


ifeq  ($(TARGETHW), ngw100)
OBJFILES	=  $(BAJOSSOURCES:.c=.o) $(NGW100SOURCES:.c=.o) $(ASSSOURCESNGW100:.S=.o)   
PLATFORM	= NGW100  -D BOARD=NGW100
LDFLAGS		= -nostartfiles
LDFLAGS+	= -march=$(ARCH) -mpart=$(PART)
all:	clean compile  bootclasses program

endif

ifeq  ($(TARGETHW), stk1000)
OBJFILES	=  $(BAJOSSOURCES:.c=.o) $(STK1000SOURCES:.c=.o) 
PLATFORM	= STK1000  -D BOARD=STK1000
LDFLAGS		= -march=$(ARCH) -mpart=$(PART)
all:	clean compile  bootclasses bootgraphic  program logo


endif

ifeq ($(filter $(TARGETHW) ,stk1000 ngw100), $(TARGETHW))
CC		= $(AVR32BIN)avr32-gcc
#CC		= $(AVR32LINUXGCC)
MCPU		= ap7000
CC_FLAGS	= -Wall -c  -mcpu=$(MCPU) -O$(OPT)
# -Werror -g
ARCH		= ap
# Part: {none|ap7xxx|uc3xxxxx}
PART		= ap7000


# Optimization level, can be [0, 1, 2, 3, s]. 
OPT		= 2
# Set your target processor

# Link: create ELF output file from object files

ngw100: 
	@:

stk1000: 
	@:

compile:	$(TARGETFILE)	

$(TARGETFILE): 	$(OBJFILES)
	@echo $(MSG_LINKING)
	$(CC)  $(filter %.o,$+) $(LDFLAGS) -o $(TARGETFILE)
	@echo
	@echo $(MSG_BINARY_IMAGE)
	$(OBJCOPY) -O binary $(TARGETFILE)  $(TARGETFILE).bin
	@echo

# Compile: create object files from C source files.
%.o: %.c	
	@echo $(MSG_COMPILING)
	$(CC)  $(CC_FLAGS) $(DEBUGGEN) -D$(PLATFORM) -DAVR32AP7000 -o $@ $<
	@echo

%.o: %.S
	@echo $(MSG_COMPILING)
	$(CC)  $(CC_FLAGS) $(DEBUGGEN) -D$(PLATFORM) -DAVR32AP7000 -o $@ $<
	@echo

#program your avr32 device
logo:
	sleep 2
	$(VERBOSE_CMD) $(PROGRAM)  program -F bin -O 0x700000  -f@0x00700000,512Kb  -e -v -R STK1000/logo.bmp

.PHONY: program
program:	
	$(VERBOSE_CMD) $(PROGRAM)   $(JTAG_PORT)  halt
	sleep 3
	$(VERBOSE_CMD) $(PROGRAM)    $(JTAG_PORT)  program  -e -v -f0,8Mb  $(TARGETFILE).bin
	@printf %4d `echo $(BOOTCLASSES)| wc -w` > mytemp
	@for i in $(BOOTCLASSES) ;do printf %4d `cat $$i| wc -c` >> mytemp;	cat $$i >> mytemp;	done
	sleep 2
	$(VERBOSE_CMD) $(PROGRAM)  program -F bin -O 0x40000  -f@0x00040000,512Kb  -e -v -R mytemp
	@rm mytemp


#	$(VERBOSE_CMD) $(PROGRAM)   run
endif

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# COMMON MAKE RULES
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# Clean up the project.
.PHONY: clean

.PHONY: clobber

clobber: clean
	-$(VERBOSE_CMD)$(RM) *bootpack
	-$(VERBOSE_CMD)$(RM) `find . -name *.class -print`

clean:
	@echo $(MSG_CLEANING)
	-$(VERBOSE_CMD)$(RM) $(BIN)
	-$(VERBOSE_CMD)$(RM) $(HEX)
	-$(VERBOSE_CMD)$(RM) $(SYM)
	-$(VERBOSE_CMD)$(RM) $(LSS)
	-$(VERBOSE_CMD)$(RM) $(TARGETFILE)
	-$(VERBOSE_CMD)$(RM) $(OBJFILES)
	-$(VERBOSE_CMD)$(RM)  *.o
	-$(VERBOSE_CMD)$(RM)  */*.o
	$(VERBOSE_NL)
	
# Rebuild the project.
.PHONY: rebuild
rebuild: clean $(TARGETFILE)

# Display CC version information.
.PHONY: ccversion
ccversion:
	@echo
	@echo
	@$(CC) --version

# Create extended listing from target output file.
.PHONY: lss
lss: $(LSS)

# Display target size information.
.PHONY: sizes
sizes: $(TARGETFILE)
	@echo
	@echo
ifeq ($(TGTTYPE),.a)
	@$(SIZE) -Bxt $<
else
ifeq ($(TGTTYPE),)
	-$(VERBOSE_CMD) $(SIZE) -Ax $<
	-$(VERBOSE_CMD) $(SIZE) -Bx $<
endif
endif

# Get CPU information.
.PHONY: cpuinfo
cpuinfo:
	@echo
	@echo $(MSG_GETTING_CPU_INFO)
	$(VERBOSE_CMD)$(PROGRAM) cpuinfo
ifneq ($(call LastWord,$(filter cpuinfo chiperase erase program secureflash reset debug run readregs,$(MAKECMDGOALS))),cpuinfo)
	@$(SLEEP) $(SLEEPUSB)
else
	@echo
endif

# Stop CPU execution.
.PHONY: halt
halt:
ifeq ($(filter cpuinfo chiperase erase program secureflash reset run readregs,$(MAKECMDGOALS)),)
	@echo
	@echo $(MSG_HALTING)
	$(VERBOSE_CMD)$(PROGRAM) halt
ifneq ($(call LastWord,$(filter halt debug,$(MAKECMDGOALS))),halt)
	@$(SLEEP) $(SLEEPUSB)
else
	@echo
endif
else
	@:
endif

# Perform a JTAG Chip Erase command.
.PHONY: chiperase
chiperase:
	@echo
	@echo $(MSG_ERASING_CHIP)
	$(VERBOSE_CMD) $(PROGRAM) chiperase
ifneq ($(call LastWord,$(filter cpuinfo chiperase program secureflash reset debug run readregs,$(MAKECMDGOALS))),chiperase)
	@$(SLEEP) $(SLEEPUSB)
else
	@echo
endif

# Perform a flash chip erase.
.PHONY: erase
erase:
ifeq (  $(filter chiperase program,$(MAKECMDGOALS)),  )
	@echo
	@echo $(MSG_ERASING)
	$(VERBOSE_CMD)$(PROGRAM) erase $(FLASH:%=-f%)
ifneq ($(call LastWord,$(filter cpuinfo erase secureflash reset debug run readregs,$(MAKECMDGOALS))),erase)
	@$(SLEEP) $(SLEEPUSB)
else
	@echo
endif
else
	@:
endif

# Reset MCU.
.PHONY: reset
reset:
ifeq ($(filter program run,$(MAKECMDGOALS)),)
	@echo
	@echo $(MSG_RESETTING)
	$(VERBOSE_CMD)$(PROGRAM) reset
ifneq ($(call LastWord,$(filter cpuinfo chiperase erase secureflash reset debug readregs,$(MAKECMDGOALS))),reset)
	@$(SLEEP) $(SLEEPUSB)
else
	@echo
endif
else
	@:
endif

# Start CPU execution.
.PHONY: run
run:
ifeq ($(findstring program,$(MAKECMDGOALS)),)
	@echo
	@echo $(MSG_RUNNING)
	$(VERBOSE_CMD)$(PROGRAM) run $(if $(findstring reset,$(MAKECMDGOALS)),-R)
ifneq ($(call LastWord,$(filter cpuinfo chiperase erase secureflash debug run readregs,$(MAKECMDGOALS))),run)
	@$(SLEEP) $(SLEEPUSB)
else
	@echo
endif
else
	@:
endif

# Read CPU registers.
.PHONY: readregs
readregs:
	@echo
	@echo $(MSG_READING_CPU_REGS)
	$(VERBOSE_CMD)$(PROGRAM) readregs
ifneq ($(call LastWord,$(filter cpuinfo chiperase erase program secureflash reset debug run readregs,$(MAKECMDGOALS))),readregs)
	@$(SLEEP) $(SLEEPUSB)
else
	@echo
endif

# Create extended listing from target output file.
$(LSS): $(TARGETFILE)
	@echo $(MSG_EXTENDED_LISTING)
	$(VERBOSE_CMD)$(OBJDUMP) -h -S $< > $@
	$(VERBOSE_NL)

ifeq ($(TGTTYPE),.elf)
# Create binary image from ELF output file.
$(BIN): $(TARGETFILE)
	@echo $(MSG_BINARY_IMAGE)
	$(VERBOSE_CMD)$(OBJCOPY) -O binary $< $(TARGETFILE)
	$(VERBOSE_NL)
endif

debug:
	@:

# Copyright (c) 2007, Atmel Corporation All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/
# or other materials provided with the distribution.
#
# 3. The name of ATMEL may not be used to endorse or promote products derived
# from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
# SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

bootclasses:
	make -f ./BAJOSBOOT/makefile $(TARGETHW)
	make -f ./BAJOSBOOT/makefile boot

bootgraphic:
	make -f ./BAJOSBOOT/makefile graphic



	


# examples 
# make linux A
# make linux compA
.PHONY:	NGW100
NGW100:
	./$(TARGETFILE)   $(BOOTCLASSES) 	$(APPCLASSPATH)/NGW100.class 

compNGW100:	
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) $(APPCLASSPATH)/NGW100.java

compCharon:	
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) $(APPCLASSPATH)/Charon.java

compEVK1100:	
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) $(APPCLASSPATH)/EVK1100.java

compSTK1000:	
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) $(APPCLASSPATH)/STK1000.java

compSTK1000a:	
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) $(APPCLASSPATH)/STK1000a.java

compSTK1000b:	
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) $(APPCLASSPATH)/STK1000b.java

A:
	./$(TARGETFILE)   $(BOOTCLASSES) 	$(APPCLASSPATH)/A.class 
#	$(APPCLASSPATH)/Aparent.class

compA:	
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) $(APPCLASSPATH)/A.java
	
AA:
	./$(TARGETFILE)   $(BOOTCLASSES) $(APPCLASSPATH)/AA.class 

compAA:	
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) $(APPCLASSPATH)/AA.java

compB:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES)  $(APPCLASSPATH)/B.java

B:	 
	./$(TARGETFILE)   $(BOOTCLASSES) $(APPCLASSPATH)/B.class
	
compC:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES)  $(APPCLASSPATH)/C.java

C:	 
	./$(TARGETFILE)   $(BOOTCLASSES) $(APPCLASSPATH)/C.class

# javac -g:none B.java		// no debug info (line number table)
# javap -c -l -verbose B

compD:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH}  $(APPCLASSPATH)/D.java

D:	 
	./$(TARGETFILE)   $(BOOTCLASSES)  $(APPCLASSPATH)/D.class

EEE:	 
	./$(TARGETFILE)   $(BOOTCLASSES) $(APPCLASSPATH)/EEE.class

compEEE:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} $(APPCLASSPATH)/EEE.java

FFF:
	./$(TARGETFILE)   $(BOOTCLASSES) ${GRAPHICS}/Font.class 

compFFF:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} $(APPCLASSPATH)/FFF.java

GGG:	 
	./$(TARGETFILE)   $(BOOTCLASSES) $(APPCLASSPATH)/GGG.class

compGGG:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} $(APPCLASSPATH)/GGG.java

HHH:	 
	./$(TARGETFILE)   $(BOOTCLASSES) 	$(APPCLASSPATH)/HHH.class

compHHH:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} $(APPCLASSPATH)/HHH.java

My:	 
	./$(TARGETFILE)   $(BOOTCLASSES) $(APPCLASSPATH)/My.class $(APPCLASSPATH)/Ball.class

compMy:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH}  $(APPCLASSPATH)/My.java

Temp:	 
	./$(TARGETFILE)   $(BOOTCLASSES) $(APPCLASSPATH)/Temperature.class

compTemp:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH}  $(APPCLASSPATH)/Temperature.java


T:	 
	./$(TARGETFILE)   $(BOOTCLASSES) $(APPCLASSPATH)/T2.class $(APPCLASSPATH)/T1.class

compT:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} \
	  	$(APPCLASSPATH)/T1.java $(APPCLASSPATH)/T2.java
	
PC:	 
	./$(TARGETFILE)   $(BOOTCLASSES) $(APPCLASSPATH)/Buffer.class \
		$(APPCLASSPATH)/ProducerConsumer.class 

compPC:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} \
		$(APPCLASSPATH)/ProducerConsumer.java

ADC:	 
	./$(TARGETFILE)   $(BOOTCLASSES) $(APPCLASSPATH)/ADC.class

compADC:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} \
		$(APPCLASSPATH)/ADC.java

Inp:	 
	./$(TARGETFILE)   $(BOOTCLASSES) $(APPCLASSPATH)/InpTest.class

compInp:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH} \
		$(APPCLASSPATH)/InpTest.java

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# MESSAGES
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
ERR_TARGET_TYPE       = Target type not supported: `$(TGTTYPE)'
MSG_CLEANING          = Cleaning project.
MSG_PREPROCESSING     = Preprocessing \`$<\' to \`$@\'.
MSG_COMPILING         = Compiling \`$<\' to \`$@\'.
MSG_ASSEMBLING        = Assembling \`$<\' to \`$@\'.
MSG_ARCHIVING         = Archiving to \`$@\'.
MSG_LINKING           = Linking to \`$(TARGETFILE)\'.
MSG_EXTENDED_LISTING  = Creating extended listing to \`$@\'.
MSG_SYMBOL_TABLE      = Creating symbol table to \`$@\'.
MSG_IHEX_IMAGE        = Creating Intel HEX image to \`$@\'.
MSG_BINARY_IMAGE      = Creating binary image to \`$(TARGETFILE).bin\'.
MSG_GETTING_CPU_INFO  = Getting CPU information.
MSG_HALTING           = Stopping CPU execution.
MSG_ERASING_CHIP      = Performing a JTAG Chip Erase command.
MSG_ERASING           = Performing a flash chip erase.
MSG_PROGRAMMING       = Programming MCU memory from \`$(TARGETFILE)\'.
MSG_SECURING_FLASH    = Protecting chip by setting security bit.
MSG_RESETTING         = Resetting MCU.
MSG_DEBUGGING         = Opening debug connection with MCU.
MSG_RUNNING           = Starting CPU execution.
MSG_READING_CPU_REGS  = Reading CPU registers.
MSG_CLEANING_DOC      = Cleaning documentation.
MSG_GENERATING_DOC    = Generating documentation to \`$(DOC_PATH)\'.
