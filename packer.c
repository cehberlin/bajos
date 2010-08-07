#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])	{

FILE*  readFile,*writeFile;
int i, count = argc - 2;
unsigned int  length=count;
unsigned char c;
if (argc <= 2)	exit (-1);
if ((writeFile=fopen(argv[1],"w")) < 0) {perror("can not open");exit(-2);}
fwrite(&length,sizeof(int),1,writeFile);		// NUMCLASSES
for (int i=0; i< count; i++)	{
if ((readFile=fopen(argv[i+2],"r")) < 0) {perror("can not open");exit(-3);}
printf("%d %s\n",i,argv[i+2]);
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
