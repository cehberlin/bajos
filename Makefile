# Hey -  this is a -*- makefile -*-
# for atmega128 (CharonII), linux, avr32UCA (EVK1000), avr32AP7000(NGW100,STK1000)
# goals
# avr8			build avr8
# uc3a		build uc3a
# ap7000		build ap7000
# linux			build linux
# avr8 debug
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
# clobber

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# FUNCTIONS
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
LastWord  = $(if $(1),$(word $(words $(1)) ,$(1)))
FirstWord = $(if $(1),$(word 1,$(1)))
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# CHECK COMMAND LINE
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# test command line - make it better
#$(if   $(filter avr8 linux uc3a ap7000 clean A,$(MAKECMDGOALS)), ,$(error wrong command line))
TARGETHW = $(filter avr8 evk1100 ngw100 stk1000 linux ,$(MAKECMDGOALS))
ifdef $TARGETHW
ifneq "1"  "$(words $(TARGETHW))"
$(error only one target hardware accepted)
endif
endif


# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# ENVIRONMENT SETTINGS
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
#the serial communication lines
TTY		= /dev/ttyS0

APPPATH = /repos/bajos/

AVR8ROOT=
AVR32ROOT=/usr/bin/
AVR32BIN = $(AVR32DIR)
#AVR32INC = $(AVR32ROOT)INCLUDES
AVR32UC3AINC = $(APPPATH)/AVR32UC3AINC
AVR32AP7000INC = $(APPPATH)/AVR32AP7000INC
# java-files
CLASSPATH = $(APPPATH)
APPCLASSPATH = $(CLASSPATH)javatests
BOOTCLASSPATH=${CLASSPATH}BAJOSBOOT/classes/
LANG= ${BOOTCLASSPATH}java/lang
IO=${BOOTCLASSPATH}java/io
UTIL= ${BOOTCLASSPATH}java/util
PLATFORM= ${BOOTCLASSPATH}platform

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# SOURCES AND TARGETS
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
BAJOSSOURCES=bajosavr.c classfile.c interpreter.c heap.c stack.c native.c scheduler.c platform.c
AVR8SOURCES=$(APPPATH)AVR8/lcd.c $(APPPATH)AVR8/shift.c
UC3ASOURCES = $(APPPATH)EVK1100/intcuc3a.c $(APPPATH)EVK1100/pmuc3a.c $(APPPATH)EVK1100/rtcuc3a.c\
$(APPPATH)EVK1100/dip204.c $(APPPATH)EVK1100/spi.c $(APPPATH)EVK1100/gpiouc3a.c \
$(APPPATH)EVK1100/flashcuc3a.c $(APPPATH)EVK1100/usartuc3a.c $(APPPATH)EVK1100/sdramc.c iobinding.c
AP7000SOURCES= iobinding.c  
NGW100SOURCES=$(APPPATH)NGW100/hsdramc.c $(APPPATH)NGW100/gpiongw100.c $(APPPATH)NGW100/pio.c \
$(APPPATH)AVR32AP7000/usartap7000.c
STK1000SOURCES=$(APPPATH)STK1000/lcdc.c $(APPPATH)STK1000/at32stk1000.c \
$(APPPATH)STK1000/lib2d.c $(APPPATH)STK1000/ltv350qv.c \
$(APPPATH)STK1000/pio.c $(APPPATH)STK1000/pm.c $(APPPATH)STK1000/spi.c \
$(APPPATH)STK1000/utils.c $(APPPATH)STK1000/usart.c $(APPPATH)STK1000/sdram.c \
$(APPPATH)STK1000/bmp_lib.c
# verrückt
ASSSOURCESUC3A =  $(APPPATH)/EVK1100/trampolineuc3a.S  $(APPPATH)/EVK1100/exceptionuc3a.S
TARGET		=  $(basename $(call FirstWord,$(BAJOSSOURCES)))
TGTTYPE   = $(suffix $(TARGET))
TGTFILE   = $(TARGET)
#$(PART)-$(TARGET)
LSS       = $(TGTFILE:$(TGTTYPE)=.lss)
SYM       = $(TGTFILE:$(TGTTYPE)=.sym)
HEX       = $(TGTFILE:$(TGTTYPE)=.hex)
BIN       = $(TGTFILE:$(TGTTYPE)=.bin)

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# BINARIES
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
RM        = rm -Rf
ifeq ($(findstring avr8,$(MAKECMDGOALS)),avr8)
OBJDUMP	= $(AVR8ROOT)avr-objdump
OBJCOPY	= $(AVR8ROOT)avr-objcopy
SIZE    = $(AVR8ROOT)avr-size
else
OBJDUMP	= $(AVR32BIN)avr32-objdump
OBJCOPY	= $(AVR32BIN)avr32-objcopy
SIZE    = $(AVR32BIN)avr32-size
PROGRAM	=$(AVR32BIN)avr32program
endif

SLEEP     = sleep
SLEEPUSB  = 9

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
#  SOME FLAGS
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
DEBUGGEN=$(if $(filter debug ,$(MAKECMDGOALS)), -DDEBUG,)
ISPFLAGS  = -device at32$(PART) -hardware usb -operation

# Display main executed commands.
ifneq ($(findstring verbose,$(MAKECMDGOALS)),verbose)
# Prefix displaying the following command if and only if verbose is a goal.
VERBOSE_CMD = @
# New line displayed if and only if verbose is a goal.
VERBOSE_NL  =
else
VERBOSE_CMD = 
VERBOSE_NL  = @echo
verbose:
	@:
endif

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# THE RULES
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
ifeq  ($(TARGETHW), avr8)
OBJFILES  = $(BAJOSSOURCES:.c=.o) $(AVR8SOURCES:.c=.o)
ARCH = AVR5
PART=atmega128
CC = $(AVR8ROOT)avr-gcc
TEXTSEGMENT	= 0x100
STACKSEGMENT	= 0x4000
.PHONY: avr8
	@:
avr8:	$(TGTFILE)
$(TGTFILE):		${OBJFILES}
		@echo $(MSG_LINKING)
		$(VERBOSE_CMD)${CC}	 $(filter %.o,$+)     -mmcu=$(PART) -architecture=$(ARCH) -Wl,--defsym=__heap_start=0x802000,--defsym=__heap_end=0x807fff   -o$@
#		 -Wl,--defsym=__heap_start=0x802000,--defsym=__heap_end=0x807fff    -o$@
		@echo $(MSG_BINARY_IMAGE)
		$(VERBOSE_CMD) $(AVR8ROOT)avr-objcopy -O binary $@  $(BIN)
		@${CC} --version
#		@rm ${EXE}
# -Wl,-u,vfprintf -lprintf_flt

%.o: %.c
	@echo $(MSG_COMPILING)
	$(VERBOSE_CMD) ${CC}	-c   -Wall -DAVR8 ${DEBUGGEN}  -mmcu=$(PART) -o $@ $<
endif

ifeq  ($(TARGETHW), linux)
OBJFILES  = $(BAJOSSOURCES:.c=.o)
CC = gcc
linux:	$(TGTFILE)
$(TGTFILE):		${OBJFILES}
	@echo $(MSG_LINKING)
	$(VERBOSE_CMD) ${CC} -g $(filter %.o,$+)   -o$@ 
	
# Preprocess, compile & assemble: create object files from C source files.
%.o: %.c
	@echo $(MSG_COMPILING)
	$(VERBOSE_CMD) $(CC) -c -g $(CPPFLAGS) -Wall   -DLINUX ${DEBUGGEN} -o $@ $<
	@touch $@
	$(VERBOSE_NL)
endif

ifeq  ($(TARGETHW), evk1100)
OBJFILES  = $(BAJOSSOURCES:.c=.o) $(ASSSOURCESUC3A:.S=.o) $(UC3ASOURCES:.c=.o)
CC=$(AVR32BIN)avr32-gcc
ARCH = uc
# Part: {none|ap7xxx|uc3xxxxx}
PART = uc3a0512
# Flash memories: [{cfi|internal}@address,size]...
FLASH = internal@0x80000000,512Kb
# Clock source to use when programming: [{xtal|extclk|int}]
PROG_CLOCK = xtal
DEFS = -D BOARD=EVK1100
# Include path
INC_PATH = $(AVR32UC3AINC)
# Linker script file if any
LINKER_SCRIPT = $(APPPATH)EVK1100/link_uc3a0512.lds
# Options to request or suppress warnings: [-fsyntax-only] [-pedantic[-errors]] [-w] [-Wwarning...]
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
WARNINGS = -Wall
# Options for debugging: [-g]...
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
DEBUG = -g
# Options that control optimization: [-O[0|1|2|3|s]]...
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
OPTIMIZATION = -O0 -ffunction-sections -fdata-sections
# Extra flags to use when linking
#LD_EXTRA_FLAGS = -Wl,--gc-sections -nostdlib -Wl,-e,_trampoline
LD_EXTRA_FLAGS = -Wl,--gc-sections  -Wl,-e,_trampoline

CPPFLAGS  = -march=$(ARCH) -DEVK1100 -DAVR32UC3A -mpart=$(PART) $(WARNINGS) $(DEFS) \
            $(PLATFORM_INC_PATH:%=-I%) $(INC_PATH:%=-I%) $(CPP_EXTRA_FLAGS)

#CC        = avr32-gcc
CFLAGS    = $(DEBUGGEN) $(OPTIMIZATION) $(C_EXTRA_FLAGS) \
            $(PLATFORM_INC_PATH:%=-Wa,-I%) $(INC_PATH:%=-Wa,-I%) $(AS_EXTRA_FLAGS)

ASFLAGS   = $(DEBUGGEN) \
            $(PLATFORM_INC_PATH:%=-Wa,-I%) $(INC_PATH:%=-Wa,-I%) $(AS_EXTRA_FLAGS)

LDFLAGS   = -march=$(ARCH) -mpart=$(PART) \
            $(LIB_PATH:%=-L%) $(LINKER_SCRIPT:%=-T%) $(LD_EXTRA_FLAGS)

LOADLIBES = -lc
LDLIBS    = $(LIBS:%=-l%)


.PHONY:	evk1100
evk1100:	$(TGTFILE)
$(TGTFILE): $(OBJFILES)
	@echo $(MSG_LINKING)
	$(VERBOSE_CMD)$(CC) $(LDFLAGS) $(filter %.o,$+) $(LOADLIBES) $(LDLIBS) -o $@
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

# Program MCU memory from ELF output file.
.PHONY: program
program: $(TGTFILE)
	@echo
	@echo $(MSG_PROGRAMMING)
	$(VERBOSE_CMD) $(PROGRAM) program $(FLASH:%=-f%) $(PROG_CLOCK:%=-c%) -e -v -R $(if $(findstring run,$(MAKECMDGOALS)),-r) $(TGTFILE)
	sleep 2
	$(VERBOSE_CMD) $(PROGRAM) program  -F bin -O 0x80040000  -finternal@0x80000000,512Kb -cxtal -e -v -R mbc

ifneq ($(call LastWord,$(filter cpuinfo chiperase program secureflash debug readregs,$(MAKECMDGOALS))),program)
	@$(SLEEP) $(SLEEPUSB)
else
	@echo
endif
endif


ifeq  ($(TARGETHW), ngw100)

OBJFILES=$(AP7000SOURCES:.c=.o) $(BAJOSSOURCES:.c=.o) $(NGW100SOURCES:.c=.o) 
PLATFORM=NGW100
endif

ifeq  ($(TARGETHW), stk1000)
OBJFILES=$(AP7000SOURCES:.c=.o) $(BAJOSSOURCES:.c=.o) $(STK1000SOURCES:.c=.o) 
PLATFORM=STK1000
endif

ifeq ($(filter $(TARGETHW) ,stk1000 ngw100), $(TARGETHW))

CC=$(AVR32BIN)avr32-gcc
MCPU = ap7000
CC_FLAGS =  -Wall -c  -mcpu=$(MCPU) -O$(OPT) 
# -Werror -g
ARCH = ap
# Part: {none|ap7xxx|uc3xxxxx}
PART = ap7000
LD_FLAGS   = -march=$(ARCH) -mpart=$(PART) 

# Optimization level, can be [0, 1, 2, 3, s]. 
OPT = 2
# Set your target processor

# Link: create ELF output file from object files.
ngw100:	ap7000

stk1000: ap7000

ap7000:	$(TGTFILE)
$(TGTFILE):	$(OBJFILES) 
	@echo $(MSG_LINKING)
	$(CC)  $(filter %.o,$+) $(LD_FLAGS) -o $@
	@echo
	@echo $(MSG_BINARY_IMAGE)
	$(OBJCOPY) -O binary $@  $@.bin
	@echo

# Compile: create object files from C source files.
%.o: %.c	
	@echo $(MSG_COMPILING)
	$(CC)  $(CC_FLAGS) $(DEBUGGEN) -D$(PLATFORM) -DAVR32AP7000 -I$(AVR32UC3AINC) -I$(AVR32AP7000INC) -o $@ $<
	@echo

#program your avr32 device
.PHONY: program
program:	
	$(VERBOSE_CMD) $(PROGRAM)   $(JTAG_PORT)  halt
	sleep 3
	$(VERBOSE_CMD) $(PROGRAM)    $(JTAG_PORT)  program  -e -v -f0,8Mb  $(TGTFILE).bin
	sleep 2
	$(VERBOSE_CMD) $(PROGRAM)  program -F bin -O 0x40000  -f@0x00040000,512Kb  -e -v -R mbc
	sleep 3 
	$(VERBOSE_CMD) $(PROGRAM)   run
endif


# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# COMMON MAKE RULES
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# Clean up the project.
.PHONY: clean
clean:
	@echo $(MSG_CLEANING)
	-$(VERBOSE_CMD)$(RM) $(BIN)
	-$(VERBOSE_CMD)$(RM) $(HEX)
	-$(VERBOSE_CMD)$(RM) $(SYM)
	-$(VERBOSE_CMD)$(RM) $(LSS)
	-$(VERBOSE_CMD)$(RM) $(TGTFILE)
	-$(VERBOSE_CMD)$(RM) $(OBJFILES)
	-$(VERBOSE_CMD)$(RM) *.o
	$(VERBOSE_NL)

# Rebuild the project.
.PHONY: rebuild
rebuild: clean $(TGTFILE)

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
sizes: $(TGTFILE)
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
$(LSS): $(TGTFILE)
	@echo $(MSG_EXTENDED_LISTING)
	$(VERBOSE_CMD)$(OBJDUMP) -h -S $< > $@
	$(VERBOSE_NL)

ifeq ($(TGTTYPE),.elf)
# Create binary image from ELF output file.
$(BIN): $(TGTFILE)
	@echo $(MSG_BINARY_IMAGE)
	$(VERBOSE_CMD)$(OBJCOPY) -O binary $< $@
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
JAVACOMP			=javac
JAVACOMPFLAGS		=-g:none -source 1.4 -verbose
JAVACOMPBOOTCLASSES	=-bootclasspath ${BOOTCLASSPATH}

A:
	./$(TARGET)   ${LANG}/Object.class \
	${PLATFORM}/PlatForm.class \
 	${LANG}/Thread.class \
 	${LANG}/Float.class \
 	${LANG}/System.class \
 	 ${LANG}/String.class \
  	 ${IO}/OutStream.class \
  	 ${IO}/InStream.class \
  	 ${LANG}/Integer.class \
  	 ${LANG}/StringBuilder.class \
    	$(APPCLASSPATH)/A.class

compA:	
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES)  $(APPCLASSPATH)/A.java
AA:
	./$(TARGET)    ${LANG}/Object.class \
	${PLATFORM}/PlatForm.class \
 	${LANG}/System.class \
 	 ${LANG}/String.class \
	${LANG}/Integer.class \
  	 ${IO}/OutStream.class \
  	 ${IO}/InStream.class \
	$(APPCLASSPATH)/AA.class

compAA:	
	javac -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH}  $(APPCLASSPATH)/AA.java

compB:
	javac -verbose -source 1.4 -g:none -bootclasspath ${BOOTCLASSPATH}  $(APPCLASSPATH)/B.java

B:	 
	./$(TARGET)    ${LANG}/Object.class \
	${PLATFORM}/PlatForm.class \
 	${LANG}/System.class \
	${LANG}/String.class \
	 ${LANG}/StringBuilder.class \
	 ${LANG}/Integer.class \
	 ${IO}/OutStream.class \
  	 ${IO}/InStream.class \
  	 ${LANG}/Integer.class \
  	 $(APPCLASSPATH)/B.class
	
compC:
	javac -verbose  -source 1.4 -g:none -bootclasspath ${BOOTCLASSPATH}  $(APPCLASSPATH)/C.java

C:	 
	./$(TARGET)    ${LANG}/Object.class \
	${PLATFORM}/PlatForm.class \
 	${LANG}/System.class \
	${LANG}/String.class \
	 ${LANG}/StringBuilder.class \
	 ${LANG}/Integer.class \
	 ${IO}/OutStream.class \
 	 ${IO}/InStream.class \
	$(APPCLASSPATH)/C.class

# javac -g:none B.java		// no debug info (line number table)
# javap -c -l -verbose B


compD:
	javac -verbose  -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH}  $(APPCLASSPATH)/D.java

D:	 
	./$(TARGET)  ${LANG}/Object.class \
	${PLATFORM}/PlatForm.class \
 	${LANG}/System.class \
	${LANG}/String.class \
	 ${IO}/OutStream.class \
  	 ${IO}/InStream.class \
  	 $(APPCLASSPATH)/D.class

EEE:	 
	./$(TARGET)    ${LANG}/Object.class 	${PLATFORM}/PlatForm.class ${LANG}/System.class ${LANG}/Float.class\
 	 ${LANG}/Math.class ${LANG}/Thread.class	 ${LANG}/String.class \
 	${IO}/OutStream.class   	 ${IO}/InStream.class  ${LANG}/Integer.class \
  	$(APPCLASSPATH)/EEE.class

compEEE:
	javac -source 1.4 -g:none -bootclasspath ${BOOTCLASSPATH}  $(APPCLASSPATH)/EEE.java

FFF:
	./$(TARGET)  	 $(APPCLASSPATH)/FFF.class  ${LANG}/Object.class    ${PLATFORM}/PlatForm.class  ${LANG}/System.class \
	${IO}/OutStream.class   	 ${IO}/InStream.class   ${LANG}/StringBuilder.class \
 	${LANG}/Math.class   ${LANG}/Integer.class  ${LANG}/String.class

compFFF:
	javac -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH}  $(APPCLASSPATH)/FFF.java

GGG:	 
	././$(TARGET)    ${LANG}/Object.class 	${PLATFORM}/PlatForm.class  	${LANG}/System.class \
 	${LANG}/Thread.class  	 ${LANG}/String.class   	 ${LANG}/Math.class \
 	${IO}/OutStream.class   	 ${IO}/InStream.class \
  	$(APPCLASSPATH)/GGG.class

compGGG:
	javac -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH}  $(APPCLASSPATH)/GGG.java

HHH:	 
	./$(TARGET)    ${LANG}/Object.class 	${PLATFORM}/PlatForm.class  	${LANG}/System.class \
 	${LANG}/Thread.class  	 ${LANG}/String.class   	 ${LANG}/Math.class \
 	${IO}/OutStream.class   	 ${IO}/InStream.class \
  	$(APPCLASSPATH)/HHH.class

compHHH:
	javac -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH}  $(APPCLASSPATH)/HHH.java

My:	 
	./$(TARGET)    ${LANG}/Object.class 	${PLATFORM}/PlatForm.class  	${LANG}/System.class \
 	${LANG}/Thread.class  	 ${LANG}/String.class   	 ${LANG}/Math.class  ${LANG}/Throwable.class\
 	${LANG}/Exception.class ${LANG}/Integer.class ${LANG}/StringBuilder.class \
 	${IO}/OutStream.class   	 ${IO}/InStream.class \
  	$(APPCLASSPATH)/My.class $(APPCLASSPATH)/Ball.class

compMy:
	javac -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH}  $(APPCLASSPATH)/My.java

T:	 
	./$(TARGET)    ${LANG}/Object.class 	${PLATFORM}/PlatForm.class  	${LANG}/System.class \
	${IO}/OutStream.class   	 ${IO}/InStream.class   ${LANG}/Integer.class ${LANG}/StringBuilder.class ${LANG}/String.class \
	$(APPCLASSPATH)/T2.class $(APPCLASSPATH)/T1.class

compT:
	javac -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH}  $(APPCLASSPATH)/T1.java $(APPCLASSPATH)/T2.java
	
PC:	 
	./$(TARGET)   	${PLATFORM}/PlatForm.class  	${LANG}/System.class \
 	${LANG}/Thread.class  	 ${LANG}/String.class     ${LANG}/Throwable.class\
 	${LANG}/Exception.class ${LANG}/Integer.class ${LANG}/StringBuilder.class \
 	${IO}/OutStream.class ${LANG}/InterruptedException.class   	 ${IO}/InStream.class \
  	$(APPCLASSPATH)/ProducerConsumer.class  $(APPCLASSPATH)/Buffer.class ${LANG}/Object.class

compPC:
	javac -g:none -source 1.4 -bootclasspath ${BOOTCLASSPATH}  $(APPCLASSPATH)/ProducerConsumer.java

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# MESSAGES
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
ERR_TARGET_TYPE       = Target type not supported: `$(TGTTYPE)'
MSG_CLEANING          = Cleaning project.
MSG_PREPROCESSING     = Preprocessing \`$<\' to \`$@\'.
MSG_COMPILING         = Compiling \`$<\' to \`$@\'.
MSG_ASSEMBLING        = Assembling \`$<\' to \`$@\'.
MSG_ARCHIVING         = Archiving to \`$@\'.
MSG_LINKING           = Linking to \`$@\'.
MSG_EXTENDED_LISTING  = Creating extended listing to \`$@\'.
MSG_SYMBOL_TABLE      = Creating symbol table to \`$@\'.
MSG_IHEX_IMAGE        = Creating Intel HEX image to \`$@\'.
MSG_BINARY_IMAGE      = Creating binary image to \`$@.bin\'.
MSG_GETTING_CPU_INFO  = Getting CPU information.
MSG_HALTING           = Stopping CPU execution.
MSG_ERASING_CHIP      = Performing a JTAG Chip Erase command.
MSG_ERASING           = Performing a flash chip erase.
MSG_PROGRAMMING       = Programming MCU memory from \`$(TGTFILE)\'.
MSG_SECURING_FLASH    = Protecting chip by setting security bit.
MSG_RESETTING         = Resetting MCU.
MSG_DEBUGGING         = Opening debug connection with MCU.
MSG_RUNNING           = Starting CPU execution.
MSG_READING_CPU_REGS  = Reading CPU registers.
MSG_CLEANING_DOC      = Cleaning documentation.
MSG_GENERATING_DOC    = Generating documentation to \`$(DOC_PATH)\'.
