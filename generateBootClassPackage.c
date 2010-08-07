#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


#define APPCLASSPATH 	"/home/Desktop/bajosavr/javatests"
#define CLASSPATH 			"/home/Desktop/bajosavr"
#define BOOTCLASSPATH	CLASSPATH"/BAJOSBOOT/classes"
#define LANG					BOOTCLASSPATH"/java/lang"
#define IO 						BOOTCLASSPATH"/java/io"
#define UTIL						BOOTCLASSPATH"/java/util"
#define PLATFORM			BOOTCLASSPATH"/platform"
#define NUMCLASSES		(sizeof(classFiles)/sizeof(classFiles[0]))

int main(int argc, char* argv[])	{
const char* classFiles[] ={  LANG"/String.class",
						LANG"/Object.class" , 
						PLATFORM"/PlatForm.class",
 						LANG"/System.class",
 						LANG"/Thread.class",
 						LANG"/Math.class",
 						LANG"/Throwable.class",
 						LANG"/Exception.class",
						LANG"/Error.class",
						LANG"/CharSequence.class",  
						LANG"/ArithmeticException.class",
						LANG"/ClassCastException.class", 
 						LANG"/Integer.class",
 						LANG"/StringBuilder.class", 
//						LANG"/StringBuffer.class", 
						LANG"/StringUtils.class", 
						LANG"/Runtime.class",
						LANG"/RuntimeException.class",  
						LANG"/InterruptedException.class", 
						IO"/OutStream.class",
  	 					IO"/InStream.class"
  	 					};

FILE*  readFile,*writeFile;
unsigned int  length=NUMCLASSES;	// 4 Bytes
unsigned char c;
if (argc != 2)	exit (-1);
if ((writeFile=fopen(argv[1],"w")) < 0) {perror("can not open");exit(-2);}
fwrite(&length,sizeof(int),1,writeFile);		// NUMCLASSES
for (int i=0; i< NUMCLASSES; i++)	{
if ((readFile=fopen(classFiles[i],"r")) < 0) {perror("can not open");exit(-3);}
printf("%d %s\n",i,classFiles[i]);
fseek(readFile,0L,SEEK_END);
length=ftell(readFile);
rewind(readFile);
fwrite(&length,sizeof(unsigned int),1,writeFile);		// FILELENGTH
//		    while (!feof(readFile))	
			while (fread(&c,1,1,readFile)==1) 			fwrite(&c,1,1,writeFile);
}
fclose(writeFile);
printf("%s erzeugt\n",argv[1]);
return 0;
}
