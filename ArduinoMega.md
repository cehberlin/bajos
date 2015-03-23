# Run Bajos at Arduino #
The Arduino Mega (with the atmega1280 processor  128KB flash, 8 KB data ram) or Mega256 (with the atmega256 processor  256KB flash, 16 KB data ram) is well suitable for Bajos.

## Environment ##
We work in a linux environment with the GNU toolchain  (C/C++ compiler, binutils,...), avrdude for programming and the AVR8 GNU C-compiler and tools (avr-gcc,...), which currently are hunker down in the 32 Bit IDE AVR32 Studio http://www.atmel.no/beta_ware. Download and unizp this package and insert in PLATFORMS/ARDUINOMEGA/arduinomega.inc :<br>
BINDIR = /opt/cross/as4e-ide/plugin/com.atmel.avr.toolchains.linux.x86_64_3.1.0.201012011657/os/linux/x86_64/bin/ <br>
(if this is your path to avr-gcc)  or write this path in the search path of the shell environment.<br>
<br>
There are many ways to install a java application at arduino.<br>

<h2>1. Load BAJOS with arduino bootloader as standalone application</h2>
The simplest way is to use the preinstalled bootloader at arduino board which is normally used to burn the arduino applications with the arduino IDE (windows or linux).<br>
<br>
<h3>Direct upload with AVRDUDE from MAKE</h3>
If you change the configured FLASHER in PLATFORM/ARDUINOMEGA/arduinomega.inc to :<br>

FLASHER	= stk500v1 -P/dev/ttyUSB0 # old ARDUINO Bootloader<br>
FLASHER	= stk500v2 -P/dev/ttyUSB0 # new ARDUINO Bootloader<br>
<br>
the MAKE will flash the bajos direct to your arduino on :<br>
#>make all am<br> or<br>
#>make burnbajos am<br>
<blockquote>if you have already build.</blockquote>


<h3>Arduinokermit</h3>

You can upload any software (bajos also) from pc at arduino with arduinokermit <a href='http://minikermit.googlecode.com'>http://minikermit.googlecode.com</a>, a terminal program for serial communication with microcontrollers.<br>
Download and unpack arduinokermit.tar.gz.<br>
<br>
Edit arduinokermit.c<br>
<br>
Uncomment:<br>
<br>
#define	ARDUINOSWRESET // reset the arduino before bootloading<br>
// you can let this commented for bootloading method 2. and 3.<br>
<br>
#define MODEMDEVICE 	"/dev/ttyUSB0"	    // USB port<br>
<br>
if you connect your arduino via usb cable with the pc.<br>
and:<br>
#define BAUDRATE 	B57600 //thats the default baudrate of arduino bootloader<br>
Compile from command line:<br>
#> g++ arduinokermit.c -lncurses -o arduinokermit<br>
generates the executable arduinokermit.<br>
<br>
You can test the communication between the arduino and the pc.<br>
Connect the arduino via usb cable with the pc, the green power led is on.<br>
<br>
Start arduinokermit:<br>
#>./arduinokermit<br>

Then reset the arduino with the reset button on board and type immediately 3 times '!' on<br>
pc keypad.<br>
<br>
In the minikermit window you see a text similar: "ATmegaBOOT / Arduino Mega - (C) Arduino LLC - 090930".<br>

That shows all is ready for BAJOS.<br>
<br>
<b>Prepare BAJOS</b>
Download and unpack bajos.tar.gz from this page.<br>
<br>
Edit bajos/ARDUINOMEGA/arduinomega.inc<br>
<br>
#define BAUDRATE	57600	/<code>*</code> arduino bootloader default baudrate <code>*</code>/<br>
Then compile bajos, generate bootclasses and compile application class (javatests/AM.class) with the make-utility in the bajos-dir:<br>
<b>#>make clean  bootclasses compAM bootpack app compile bajospack am</b>

creates bajos.bin which contains the virtual machine, booclasses and the application class. Thats our binary file for upload at the arduino.<br>
Start arduinokermit (maybe there are outputs from an old program in arduino. Don't mind me.)<br>
and type 'w'. The arduinokermit window display the text: "name of bin file for uploading?".<br>
<br>
Type "bajos.bin". Bajos is uploading now and starts immediately. AM.java in the directory javatests of bajos is the running application class. It contains a test of the method currentTimeMilliSec and driver for the programmable board led. You can change this sourcecode and produce a new standalone bajos with the command line above.<br>
Other or more application classes can be added by editing the arduinomega.inc file.<br>
<br>
<br>
<h2>2 Burn BAJOS with AVR-ISP-mkII programmer as standalone application</h2>
If you have a avr-isp programmer connect it with your arduino board (pins labeled as "ICSP") and connect the serial usb port with a second usb cable with the pc. In the bajos directory type:<br>

#> make all am<br>

That burns bajos, boot classes and application classes in the arduino board. Start arduinokermit (see above) and after Reset runs the application class AM.class (see above). More precisely: after reset starts the bootloader in the boot section and if you not type a key at pc keyboard in a time window the bootloader jumps to the address 0 where bajos is loaded.<br>
<br>
<br>
<h2>3 Burn the monitor bamo128 with AVR-ISP-mkII programmer and load bajos with the monitor</h2>
Bamo128 <a href='http://bamo128.googlecode.com'>http://bamo128.googlecode.com</a> is a useful monitor program written in assembler for avr8 microcontroller. It occupies 4Kwords (arduino bootloader needs 2Kwords) in the bootsection. After reset the microcontroller starts at boot section the monitor. The resident monitor is a simple operating system with following features:<br>
<ul><li>user interaction over serial interface with a pc-terminal program (arduinokermit)<br>
</li><li>upload application programs<br>
</li><li>start  application programs with/without breakpoints and step-mode<br>
</li><li>disassemble machine code<br>
</li><li>display/modify flash, sdram and eeprom-memory<br>
</li><li>display/modify cpu register and state<br>
</li><li>driver interface for use in application programs<br>
Download and unpack bamo128. In the bamo128 directory:<br>
#> make burn am<br>
#> make fuses am<br>
generates and burn bamo128 in your arduino board (before connect the board with the programmer and the usb-serial line with the pc). The arduino bootloader now is overwritten! (Never say die, every time you can reinstall the shipped bootloader with the arduino IDE). The fuses bits must be programmed for bamo128 (4k word boot section, start after reset at bootsection is reversible with the arduino IDE also). With arduinokermit (see above) you can communicate now with bamo128 (see also <a href='http://bamo128.googlecode.com'>http://bamo128.googlecode.com</a> ) and upload every binary software. After reset starts the monitor in the boot section with a greeting and the monitor prompt. Type h for help or see at the bamo128 side.</li></ul>


<h2>3.1 Load bajos with bamo128 as standalone application</h2>
Remove the programmer from the arduino board.<br>
<br>
Switch to the bajos directory and type:<br>
<br>
<b>#>make clean  bootclasses compAM bootpack app compile bajospack am</b>

Be sure, the baudrates of bajos and arduinokermit are the same (up to 115200 possible, default 57600).<br>
The command line produces bajos.bin (how described above).<br>
Then start arduinokermit and type 'w' in the monitor after the arduinokermit terminal prompt. Arduinokermit ask for filename for uploading. Type the filename (bajos.bin) and arduinokermit and bamo128 upload the file at address 0. After this you see against the monitor prompt of bamo128. Type:<br>
#> g 0<br>

and the java application (javatests/AM.class) runs. But what is to do when the java program ends. The virtual machine starts against und run the application. With bamo128 we have a better solution. Use the drivers in bamo128 (which is resident in bootsection) and return to the bamo128 prompt at java program end.<br>
<br>
<br>
<h2>3.2 Burn bajos by offering drivers through bamo128</h2>
The better way with bamo128 is to use the io-drivers (serial, milliSec-Timer,jump to monitor prompt). This reduces the program size also. There ist a jump table at the beginning of bamo128 with fixed addresses to entry points in bamo128. Revise the source code of bamo128 change not the jump table location.<br>
Switch to the bajos directory and type:<br>

#>make all am withmon<br>

creates 3 files:<br>
<ul><li>bajvm.bin -> the virtual machine<br>
</li><li>avr8bootpack -> the boot (system) classes of bajos as bin file<br>
</li><li>amapp -> the application classes as bin- file<br>
Start arduinokermit and type the monitor command 'w' in the arduinokermit terminal and upload bajvm.bin (to address 0).</li></ul>

Then type 'W' and give file (avr8bootpack) and the address where the avr8bootpack (8000) is uploaded too. (boot classes and application classes are stored in flash for arduino Mega).<br>
After this load with the same W-command amapp to the address: E600<br>
(The actual addresses for java classes are defined in bajos/defines.h.)<br>
Now you can start bajos with:<br>

#> g 0<br>

the application runs and after end the application jumps back to the monitor prompt.<br>
<br>
You can effectively now upload the parts you need now. Changing the java application source, type:<br>

#>make compAM amapp am<br>

and upload with bamo128 "amapp" ( the application class files) only. <br><br>
You can replace arduinokermit by avrdude (with the programmer option -carduino) for uploading bajos. For terminal communication you can use screen, kermit or other terminal software instead of arduinokermit (see <a href='http://bamo128.googlecode.com'>http://bamo128.googlecode.com</a>).