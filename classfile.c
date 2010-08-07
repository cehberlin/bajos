/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07

#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"
#include "bajosavr.h"
#include "heap.h"
#ifdef LINUX
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <stdlib.h>
#endif
#ifdef AVR8
	#include <ctype.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <avr/pgmspace.h>
	#include <util/delay.h>
	
#define		BYTES(x)	((x)*2)
#define		STRING(a,b)		#a" "#b
#define		INLINEASM(a,b)	STRING(a,b)
#endif
#include <string.h>

#include "classfile.h"

#ifdef AVR8
#define		loadInSRam1		(2*(0xf000+26))
u2 	loadInSram1(u1*) __attribute__ ((naked));
u2 loadInSram1(unsigned char* addr)	{
asm	 (INLINEASM(jmp,loadInSRam1));	
return 0;				}
#endif

#ifdef AVR8
#define		STRING(a,b)		#a" "#b
#define		INLINEASM(a,b)	STRING(a,b)
// atmega128 Monitor functions
#define		loadInSRam		(2*(0xf000+26))
u2 	loadInSram(u1*) __attribute__ ((naked));
u2 loadInSram(unsigned char* addr)	{
asm	 (INLINEASM(jmp,loadInSRam));			}
#endif

u2 readClassFileneu(u1* fileName,u1* addr)		{

#ifdef LINUX
int fd;
u2 classFileLength=-(u2)((long)addr%(1<<16))-1;
if ((fd=open((char*)fileName,O_RDONLY))==-1) perror("no such file");
while (read(fd,addr++,1));
return classFileLength+=(long)addr;
#endif

#ifdef AVR8
u2 classFileLength=0;
printf("\nLaden der AnwendungsKlassen - Geben Sie ein  'w' ein\n"); 
// the damned holznagelsche protokoll zum laden eines bin files mit minikermit nachbilden
classFileLength=(*loadInSram)(addr);
return classFileLength;
#endif

#if (AVR32UC3A||AVR32AP7000)
int i;
char c=conIn(); // dummy w
if (c =='w')	{
conOut(4);//turn off echo
c=conIn();	//s
while ((c=conIn())=='p'){ 
//if (c=='p) // apage comes
c=conIn();
c=conIn();		// address
conOut('w');
for (i=0; i < 256;i++) *(addr++)=conIn();
conOut('w');}

conOut(5);// turn on echo uploadendS
while((c=conIn())==' ');
i=0;
do
{ i+=16*i+ ((c<='9')? (c-'0'): (c-'a'+10));
conOut(c);}
while ((c=conIn())!= 0xa);
return (u2)i;
}
else	{
//uploadends2
conOut(5); // turn on echo
return (u2) 0;
}

#endif

}

// classSTA and pc are global values for actual class and method
// parameter != 0 -> value at parameter-pos
// parameter ==0 -> value at global var pc and automatic increment 
u1 getU1(u2 pos){	return *(CLASSSTA + ( (pos==0) ? (pc++) : pos) );	}

u2 getU2(u2 pos){	return (((u2)getU1(pos) << 8) | (u2)getU1 ( (pos == 0) ? 0 : pos+1 ));	}	

u4 getU4(u2 pos){	return (((u4)getU2(pos)<<16)|(u4)getU2((pos==0)?0:pos+2));	}

u8 getU8(u2 pos){	return (((u8)getU4(pos)<<32)|(u8)getU4((pos==0)?0:pos+2));	}

f4 getFloat(u2 pos){	return  getU4(pos);	}
//u4 val=getU4(pos); return (f4)*(f4*)&val;	}

double getDouble(u2 pos){	return getU8(pos);	}

void* getAddr(u2 pos){	return CLASSSTA+pos;			}

u1 findMain()		{	//  out: classNumber, methodNumber, get first class with main-method
	for (cN=0; cN < numClasses;cN++)	
		if (findMethodByName((u1*)"main",4,(u1*)"([Ljava/lang/String;)V",22)==1) return 1;	
	return 0;		}

u1 findNumFields()	{	return getU2(cs[cN].fields_count);	}

u1 findNumArgs(u2 methodRef)	{	//  count BCDFIJLSZ in runden Klammern!!
	u2 i;
	u2 n=0;
	u1 c;
	u1 on=1;
	for (i=0; i <getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,methodRef)+3))+3))+1);i++)	{
		c=*(u1*)(getAddr(CP(cN,getU2(CP(cN,getU2(CP(cN,methodRef)+3))+3))+3)+i);
		if (c=='(' )
			continue;
		if (c==')')
			break;
		if (c=='L'){
			on=0;
			n++;
		}
		if (c==';')
			on=1;
		if (on)
			if ((c=='B')||(c=='C')||(c=='D')||(c=='F')||(c=='I')||(c=='J')||(c=='S')||(c=='Z'))
				n++;																				}
	return n;
}

u2	findMaxLocals()	{	//cN,mN
return getU2(getStartPC()-6);
}

u1 findMethod(u1* className, u1 classNameLength, u1* methodName, u1 methodNameLength,u1* methodDescr,u1 methodDescrLength)		{ 
//in cN, out: cN, mN
// rekursiv bis Objekt
	if (!findClass(className,classNameLength)) 					{
		DEBUGPRINTLN1("kann nicht sein die klasse gibts nicht!!");	
		DEBUGPRINTSTRING(className,classNameLength);exit(-1);	}// out: cN
	if (findMethodByName(methodName,methodNameLength,methodDescr,methodDescrLength))
			return cN;
	else	
		if (strncmp("java/lang/Object",(char*)className,classNameLength)==0) 		{
			printf("kann nicht sein die methode gibts nicht!!: cN mN: %d %d  ",cN,mN);	
			PRINTSTRING(methodName,methodNameLength);exit(-1);			}
		else 
			return findMethod(	(u1*)getAddr(CP(cN,getU2(CP(cN,  getU2(cs[cN].super_class))+1))+3),
									getU2(CP(cN,getU2(CP(cN,  getU2(cs[cN].super_class))+1))+1)
									, methodName, methodNameLength,methodDescr,methodDescrLength);
}

u1 findMethodByName(const u1* name,u1 len,u1* methodDescr,u1 methodDescrLength)	{
//  in: classNumber cN, out: methodNumber mN
// non recursiv
	for (mN=0; mN < getU2(cs[cN].methods_count); mN++)		
		if (len==getU2(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+2)]+1))
			if(strncmp((char*)name,(char*)getAddr(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+2)]+3),
								getU2(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+2)]+1))==0)	{
				if (methodDescr!=NULL)	{		
					if (methodDescrLength==getU2(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+4)]+1))
						if(strncmp((char*)methodDescr,(char*)getAddr(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+4)]+3),
								getU2(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+4)]+1))==0)			return 1;
												}
				else		return 1;																					}
	return 0;			
}

u1* findMethodByMethodNumber()		{	//mb jf  in: methodNumber, out: methodName
	u2 methAddr = METHODBASE(cN,mN);	// get start address in class file of method #methodNumber
	u2 methodNameAddr = getU2(methAddr + 2);			// 2 ... offset from begin of method in class file to method's name
	return getAddr(CP(cN,methodNameAddr)+3);			// return pointer to field value in class cN at address methodNameAddress
}
	
u1 findClass(u1* className,u1 classNameLength)	{  // out: cN
//int i;
//for (i=0; i < classNameLength;i++)printf("%c",*(className+i));
//printf("\n");
	for (cN=0; cN < numClasses;cN++)			{
		if (classNameLength !=(u1)getU2(
				cs[cN].constant_pool[	
					getU2(cs[cN].constant_pool[
						getU2(cs[cN].this_class)]+1			
					)]+1)){
			continue;
		}
		if (strncmp((char*)className,(char*)getAddr(cs[cN].constant_pool[	
					getU2(	cs[cN].constant_pool[
						getU2(cs[cN].this_class)]+1			
					)
					]+3),classNameLength)==0){ 
			return 1;
		}
	}
	return 0;
}

void analyzeClass(classStructure* c)	{
	u2 ln,i;
	u2 attribute_name_index;
	pc=0;
						// static class object
						//without gc: enough heap space for static class objects
						HEAPOBJECTMARKER(heapTop).status=HEAPALLOCATEDSTATICCLASSOBJECT;			// static classfile object
						HEAPOBJECTMARKER(heapTop).magic=OBJECTMAGIC;
						HEAPOBJECTMARKER(heapTop).rootCheck=1;
						HEAPOBJECTMARKER(heapTop).mutex = MUTEXNOTBLOCKED;
						//HEAPOBJECTMARKER(heapTop).type = STACKSTATICCLASSOBJECT;

						//HEAPOBJECTMARKER(heapTop).waitSetNumber = NUMWAITSETS;		// NULL
						HEAPOBJECTMARKER(heapTop).length = 1; // evtl. später überschreiben	
						cs[cN].classInfo.stackObj.pos=heapTop++;
						cs[cN].classInfo.stackObj.magic=OBJECTMAGIC;
						cs[cN].classInfo.stackObj.classNumber=cN;
						//2008cs[cN].classInfo.stackObj.type	=STACKSTATICCLASSOBJECT;
#ifdef DEBUG
	printf("class number:\t \t\t%X   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", cN);
#endif
	c->magic=pc;
#ifdef DEBUG
printf("cf\tmagic:\t%X\t", getU4(0));	// 0
#else
pc+=4;
#endif
	c->minor_version=pc++;			// 4
	c->major_version=++pc;			// 6
#ifdef DEBUG
	printf("version:\t%d.%d\n", getU2(0),getU2(c->minor_version));
#else
pc+=2;
#endif
	c->constant_pool_count=pc;
#ifdef DEBUG
	printf("cf\tconstant_pool_count:\t%d\n", getU2(pc));			//8
#endif
pc+=2;
	if ((c->constant_pool=(u2*)malloc(sizeof(u2)*getU2(c->constant_pool_count)))==NULL) // -2 möglich wegen 0!!
		printf("malloc error\n");;
	analyseConstantPool(c);
	c->access_flags=pc;
#ifdef DEBUG	
	printf("cf\taccess_flags: %d\n",getU2(pc));
#endif
pc+=2;
	c->this_class=pc;	
#ifdef DEBUG
	printf("cf\tthis_class: %d\n",getU2(pc));
#endif
pc+=2;
	c->super_class=pc;
#ifdef DEBUG	
	printf("cf\tsuper_class: %d\n",getU2(pc));
#endif
pc+=2;
	c->interfaces_count=pc;
#ifdef DEBUG	
	printf("cf\tinterfaces_count: %d\n",getU2(pc));
#endif
pc+=2;
	// c->interfaces=getAddress();	// no interfaces-> to do
	// printf("cf\tinterfaces: %d\n",getU2(c->interfaces));
	c->fields_count=pc;
#ifdef DEBUG	
	printf("cf\tfields_count: %d\n",getU2(pc));
#endif
pc+=2;
	c->field_info=NULL;
	if (getU2(c->fields_count)!=0){ 
		if ((	c->field_info=(u2*)malloc(sizeof(u2)*getU2(c->fields_count)))==NULL)printf("malloc error\n");
			analyzeFields(c);
	} 
	c->methods_count=pc;
#ifdef DEBUG
	printf("cf\tmethods_count: %d\n",getU2(pc));
#endif
pc+=2;
	c->method_info=NULL;
	if (getU2(c->methods_count)!=0){
		if ((	c->method_info=(u2*)malloc(2*sizeof(u2)*getU2(c->methods_count)))==NULL)printf("malloc error\n");
			analyseMethods(c);
	}
	ln=getU2(0);
	for (i=0; i <ln; i++){
		attribute_name_index=getU2(0); // jetzt raus kriegen, was fuer ein atrr
		if (strncmp("SourceFile",getAddr(c->constant_pool[attribute_name_index]+3),9)==0){
#ifdef DEBUG
			printf("cf-attributes: count: %d ",ln);
			printf("attribute_length: %d",getU4(pc));
			printf(" attribute_index: %d\n",getU2(pc+4));
#endif
pc+=6;
		}
	}
}	

void analyseConstantPool(classStructure* c){
	u2 n,i,length;
	for (n=1; n<getU2(c->constant_pool_count);n++){
		c->constant_pool[n]=pc;
		switch (getU1(0)){
			case	CONSTANT_Class:							//    7 
#ifdef DEBUG
					printf("\tcp %d\t:Class\t\t-> name:\t%d\n",n,getU2(pc));
#endif
pc+=2;
			CASE	CONSTANT_String:							//    8 
#ifdef DEBUG
					printf("\tcp %d\t:String\t\t-> string:\t%d\n",n,getU2(pc));
#endif			
pc+=2;
			CASE	CONSTANT_Fieldref:							//    9 
#ifdef DEBUG
					printf("\tcp %d\t:Fieldref\t-> class: %d ",	n,getU2(pc));
					printf("name_and_type:\t%d\n",	getU2(pc+2));
#endif
pc+=4;
			CASE	CONSTANT_InterfaceMethodref:				//   11 
#ifdef DEBUG					
					printf("\tcp %d\t:InterfaceMethodref->class: %d ",n,getU2(pc));
					printf("name_and_type_index\t%d\n",getU2(pc+2));
#endif
pc+=4;			
			CASE	CONSTANT_Methodref: 						//    10  nur Methoden, die aufgerufen werden!!!
#ifdef DEBUG
					printf("\tcp %d\t:Methodref\t-> class: %d ",n,getU2(pc));
					printf("name_and_type:\t%d\n", getU2(pc+2));
#endif
pc+=4;
			CASE	CONSTANT_Integer:							//    3
#ifdef DEBUG
					printf("cp %d\t: Integer -> name:\t%d\n",n,getU4(pc));
#endif
pc+=4;			
			CASE	CONSTANT_NameAndType:					//    12
#ifdef DEBUG
					printf("\tcp %d\t:nameAndType\t-> name:\t%d ",n,getU2(pc));
					printf("descriptor:\t %d\n",getU2(pc+2));
#endif
pc+=4;			
			CASE	CONSTANT_Float:	 							//    4 
#ifdef DEBUG
					printf("\tcp %d\t:Float:\t%f  \n",n,getFloat(pc));
#endif
pc+=4;
			CASE	CONSTANT_Long:							//    5 
#ifdef DEBUG
					printf("\tcp %d\t:Long ->name:\t%ld\n",n,getU8(pc));
#endif
pc+=8;			
			CASE	CONSTANT_Double:		//    6 
#ifdef DEBUG
					printf("\tcp %d\t:Double -> name:\t%lf\n",n,getDouble(pc));
#endif
pc+=8;
			CASE	CONSTANT_Utf8:							//    1
					length=getU2(0);
#ifdef DEBUG
				printf("\tcp %d\t:Utf8:\t\t-> ",n);
					for (i=0; i < length;i++)
						printf("%c",getU1(pc+i));	// utf8 chars ???
					printf("\n");
#endif
pc+=length;
			DEFAULT: printf("invalid constant pool identifier\n");	exit(-1);}		}
}

void analyseMethods(classStructure* c){ // jan 08 not good tested
	int i,n,m,a;
	u2 etl;
	for (n=0; n<getU2(c->methods_count);n++)	{  //methods
		c->method_info[n]=pc; // absolute in classfile
#ifdef DEBUG
		printf("\tmethod %d\taccess_flags: %d\n",n,getU2(pc));
		printf("\tmethod %d\tname: %d ",n,getU2(pc+2));
		DEBUGPRINTSTRING(METHODNAMESTR(cN,n),METHODNAMESTRLENGTH(cN,n));
		printf("\tmethod %d\tdescriptor: %d ",n,getU2(pc+4)); //Signature
		DEBUGPRINTSTRING(METHODDESCRSTR(cN,n),METHODDESCRSTRLENGTH(cN,n));
		printf("\tmethod %d\tattribute_count: %d\n",n,a=getU2(pc+6));
#endif
a=getU2(pc+6);
pc+=8;		
		if (a==0) continue; // native method
		for (m=0; m<a;m++)																						{ // attributes of method
		u1* adr=getAddr(c->constant_pool[getU2(0)]+1+2);
			if (strncmp("Code", (char*)adr,4)==0)	{
#ifdef DEBUG
				printf("\t\tCode: attribute_length: %d\n",getU4(pc));
				printf("\t\tCode: max_stack: %d\n", getU2(pc+4));
				printf("\t\tCode: max_locals: %d\n",getU2(pc+6));
				printf("\t\tCode: code_length: %d\n\t\t",getU4(pc+8));
#endif
pc+=12;				
#ifdef DEBUG
				for (i=0;i <getU4(c->method_info[n]+8+10);i++)	printf("%2x ",getU1(pc+i));	 //length
#endif				
pc+=getU4(c->method_info[n]+8+10);
etl=getU2(0);
#ifdef DEBUG
				printf("\n\t\tCode: exception_table_length: %d\n",etl);		// exception_table	

					for (i=0;i <etl;i++)	{
						printf("\t\t\texception: nr: %d startPC: %d\n",i,getU2(pc+8*i));
						printf("\t\t\texception: nr: %d endPC: %d\n",i,getU2(8*i+2));
						printf("\t\t\texception: nr: %d handlerPC: %d\n",i,getU2(8*i+4));
						printf("\t\t\texception: nr: %d catchType: %d\n",i,getU2(8*i+6));
										}
#endif
pc+=etl*8;
					u2 h=getU2(0);
#ifdef DEBUG
					printf("\t\tCode: attributes_count: %d\n",	h);
#endif
					for (i=0;i< h;i++)	{
						u1*addr=getAddr(c->constant_pool[getU2(0)]+3);
						if (strncmp("LineNumberTable",(char*)addr, 15)==0 )	pc=getU4(0)+pc;	
						if (strncmp("LocalVariableTable",(char*)addr, 18)==0)	pc=getU4(0)+pc;	
										}		}
			if (strncmp("Exceptions", (char*)adr,10)==0)		{
#ifdef DEBUG
			printf("exeception object\n");
#endif
			mN=n;
			u4 n=getU4(0);
			u2 i;
			n=getU2(0);
#ifdef DEBUG
			for (i=0;i<n;i++)	
			printf("\t\t\texception: nr: %d class: %d\n",i,getU2(pc+2*i));
#endif
pc+=2*n;
			//pc=(u2)getU4(0)+pc;
			}
			if (strncmp("Synthetic", (char*)adr,9)==0)		pc+=4;
			if (strncmp("Deprecated",(char*) adr,10)==0)	pc+=4;								}
	}	// methods_count
}
	
void analyzeFields(classStructure* c){
	int n,a;
	u2 startStaticObject=heapTop-1;
	for (n=0; n<getU2(c->fields_count);n++)	{  //fields
		c->field_info[n]=pc; 																				// absolute in classfile
#ifdef DEBUG
		printf("\tfield %x\taccess_flags: %d\n",n,getU2(pc));
		printf("\tfield %d\tname: %d\n",n,getU2(pc+2));
		printf("\tfield %d\tdescriptor: %d\n",n,getU2(pc+4)); 						//Signature
		printf("\tfield %d\tattribute_count: %d\n",n,a=getU2(pc+6));
#endif
a=getU2(pc+6);
pc+=8;
		if (0x0008&getU2(pc-8))	 	// only static fields in class objects
			heapSetElement((slot)(u4)0x77, heapTop++); // initialize the heap elements	
		if ( a>0)	{
				pc+=6; // assume first is constant_value attribute
				if (getU1(CP(cN,getU2(pc)))==CONSTANT_String)	/*string to do*/;
				else		heapSetElement((slot)getU4(CP(cN,getU2(0))+1),heapTop-1); //   n+1 !!no double, float
				 pc+=(a-1)*6; // go over Synthetic and Deprecated attributes
					}
	}
	HEAPOBJECTMARKER(startStaticObject).length = heapTop-startStaticObject;
}
u2	getStartPC()	{	//cN,mN
	u2 attrLength = 0;  // search code-position
	u2 i;
for (i=0;i<getU2(METHODBASE(cN,mN)+6);i++) 					{// number of attributes
if(strncmp(	"Code",
			getAddr(	cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+8+attrLength)]+3	),
			4)==0)	
			return (u2)METHODBASE(cN,mN)+8+14+attrLength;
				
			attrLength=getU4(METHODBASE(cN,mN)+8)+6;//+attrLength;		????
			}	 // < 64K	
return -1;
}


/*
void analyzeClass(classStructure* c)	{
	u2 ln,i;
	u2 attribute_name_index;
	pc=0;
						// static class object
						//without gc: enough heap space for static class objects
						HEAPOBJECTMARKER(heapTop).status=HEAPALLOCATEDSTATICCLASSOBJECT;			// static classfile object
						HEAPOBJECTMARKER(heapTop).magic=OBJECTMAGIC;
						HEAPOBJECTMARKER(heapTop).rootCheck=1;
						HEAPOBJECTMARKER(heapTop).mutex = MUTEXNOTBLOCKED;
						//HEAPOBJECTMARKER(heapTop).waitSetNumber = NUMWAITSETS;		// NULL
						HEAPOBJECTMARKER(heapTop).length = 1; // evtl. später überschreiben	
						cs[cN].classInfo.stackObj.pos=heapTop++;
						cs[cN].classInfo.stackObj.magic=OBJECTMAGIC;
						cs[cN].classInfo.stackObj.classNumber=cN;
						cs[cN].classInfo.stackObj	=STACKSTATICCLASSOBJECT;
	fprintf(OUTSTREAM,"class number:\t \t\t%X   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", cN);

	c->magic=pc;
	fprintf(OUTSTREAM,"cf\tmagic:\t%X\t", getU4(0));	// 0
	c->minor_version=pc++;			// 4
	c->major_version=++pc;			// 6
	fprintf(OUTSTREAM,"version:\t%d.%d\n", getU2(0),getU2(c->minor_version));
	c->constant_pool_count=pc;
	fprintf(OUTSTREAM,"cf\tconstant_pool_count:\t%d\n", getU2(0));			//8
	if ((c->constant_pool=(u2*)malloc(sizeof(u2)*getU2(c->constant_pool_count)))==NULL) // -2 möglich wegen 0!!
		fprintf(OUTSTREAM,"malloc error\n");;
	analyseConstantPool(c);
	c->access_flags=pc;
	fprintf(OUTSTREAM,"cf\taccess_flags: %d\n",getU2(0));
	c->this_class=pc;	
	fprintf(OUTSTREAM,"cf\tthis_class: %d\n",getU2(0));
	c->super_class=pc;	
	fprintf(OUTSTREAM,"cf\tsuper_class: %d\n",getU2(0));
	c->interfaces_count=pc;	
	fprintf(OUTSTREAM,"cf\tinterfaces_count: %d\n",getU2(0));
	// c->interfaces=getAddress();	// no interfaces-> to do
	// fprintf(OUTSTREAM,"cf\tinterfaces: %d\n",getU2(c->interfaces));
	c->fields_count=pc;	
	fprintf(OUTSTREAM,"cf\tfields_count: %d\n",getU2(0));
	c->field_info=NULL;
	if (getU2(c->fields_count)!=0){ 
		if ((	c->field_info=(u2*)malloc(sizeof(u2)*getU2(c->fields_count)))==NULL)fprintf(OUTSTREAM,"malloc error\n");
			analyzeFields(c);
	} 
	c->methods_count=pc;
	fprintf(OUTSTREAM,"cf\tmethods_count: %d\n",getU2(0));
	c->method_info=NULL;
	if (getU2(c->methods_count)!=0){
		if ((	c->method_info=(u2*)malloc(2*sizeof(u2)*getU2(c->methods_count)))==NULL)fprintf(OUTSTREAM,"malloc error\n");
			analyseMethods(c);
	}
	ln=getU2(0);
	for (i=0; i <ln; i++){
		attribute_name_index=getU2(0); // jetzt raus kriegen, was fuer ein atrr
		if (strncmp("SourceFile",getAddr(c->constant_pool[attribute_name_index]+3),9)==0){
			fprintf(OUTSTREAM,"cf-attributes: count: %d ",ln);
			fprintf(OUTSTREAM,"attribute_length: %d",getU4(0));
			fprintf(OUTSTREAM," attribute_index: %d\n",getU2(0));
		}
	}
}	

void analyseConstantPool(classStructure* c){
	u2 n,i,length;
	for (n=1; n<getU2(c->constant_pool_count);n++){
		c->constant_pool[n]=pc;
		switch (getU1(0)){
			case	CONSTANT_Class:							//    7 
					fprintf(OUTSTREAM,"\tcp %d\t:Class\t\t-> name:\t%d\n",n,getU2(0));
			CASE	CONSTANT_String:							//    8 
					fprintf(OUTSTREAM,"\tcp %d\t:String\t\t-> string:\t%d\n",n,getU2(0));
			CASE	CONSTANT_Fieldref:							//    9 
					fprintf(OUTSTREAM,"\tcp %d\t:Fieldref\t-> class: %d ",	n,getU2(0));
					fprintf(OUTSTREAM,"name_and_type:\t%d\n",	getU2(0));
			CASE	CONSTANT_InterfaceMethodref:				//   11 
					fprintf(OUTSTREAM,"\tcp %d\t:InterfaceMethodref->class: %d ",n,getU2(0));
					fprintf(OUTSTREAM,"name_and_type_index\t%d\n",getU2(0));
			CASE	CONSTANT_Methodref: 						//    10  nur Methoden, die aufgerufen werden!!!
					fprintf(OUTSTREAM,"\tcp %d\t:Methodref\t-> class: %d ",n,getU2(0));
					fprintf(OUTSTREAM,"name_and_type:\t%d\n", getU2(0));
			CASE	CONSTANT_Integer:							//    3
					fprintf(OUTSTREAM,"cp %d\t: Integer -> name:\t%d\n",n,getU4(0));
			CASE	CONSTANT_NameAndType:					//    12
					fprintf(OUTSTREAM,"\tcp %d\t:nameAndType\t-> name:\t%d ",n,getU2(0));
					fprintf(OUTSTREAM,"descriptor:\t %d\n",getU2(0));
			CASE	CONSTANT_Float:	{ 							//    4 
					fprintf(OUTSTREAM,"\tcp %d\t:Float:\t%f  \n",n,getFloat(0));}
			CASE	CONSTANT_Long:							//    5 
					fprintf(OUTSTREAM,"\tcp %d\t:Long ->name:\t%ld\n",n,getU8(0));
			CASE	CONSTANT_Double:		//    6 
					fprintf(OUTSTREAM,"\tcp %d\t:Double -> name:\t%lf\n",n,getDouble(0));
			CASE	CONSTANT_Utf8:							//    1
					fprintf(OUTSTREAM,"\tcp %d\t:Utf8:\t\t-> ",n);
					length=getU2(0);
					for (i=0; i < length;i++)
						fprintf(OUTSTREAM,"%c",getU1(0));	// utf8 chars ???
					fprintf(OUTSTREAM,"\n");
			DEFAULT: fprintf(OUTSTREAM,"invalid constant pool identifier\n");	}		}
}

void analyseMethods(classStructure* c){
	int i,n,m,a;
	u2 etl;
	for (n=0; n<getU2(c->methods_count);n++)	{  //methods
		c->method_info[n]=pc; // absolute in classfile
		fprintf(OUTSTREAM,"\tmethod %d\taccess_flags: %d\n",n,getU2(0));
		fprintf(OUTSTREAM,"\tmethod %d\tname: %d ",n,getU2(0));
		DEBUGPRINTSTRING(METHODNAMESTR(cN,n),METHODNAMESTRLENGTH(cN,n));
		fprintf(OUTSTREAM,"\tmethod %d\tdescriptor: %d ",n,getU2(0)); //Signature
		DEBUGPRINTSTRING(METHODDESCRSTR(cN,n),METHODDESCRSTRLENGTH(cN,n));
		fprintf(OUTSTREAM,"\tmethod %d\tattribute_count: %d\n",n,a=getU2(0));
		if (a==0) continue; // native method
		for (m=0; m<a;m++)																						{ // attributes of method
		u1* adr=getAddr(c->constant_pool[getU2(0)]+1+2);
			if (strncmp("Code", (char*)adr,4)==0)	{
				fprintf(OUTSTREAM,"\t\tCode: attribute_length: %d\n",getU4(0));
				fprintf(OUTSTREAM,"\t\tCode: max_stack: %d\n", getU2(0));
				fprintf(OUTSTREAM,"\t\tCode: max_locals: %d\n",getU2(0));
				fprintf(OUTSTREAM,"\t\tCode: code_length: %d\n\t\t",getU4(0));
				for (i=0;i <getU4(c->method_info[n]+8+10);i++)	fprintf(OUTSTREAM,"%2x ",getU1(0));	 //length
				fprintf(OUTSTREAM,"\n\t\tCode: exception_table_length: %d\n",etl=getU2(0));		// exception_table	
					for (i=0;i <etl;i++)	{
						fprintf(OUTSTREAM,"\t\t\texception: nr: %d startPC: %d\n",i,getU2(0));
						fprintf(OUTSTREAM,"\t\t\texception: nr: %d endPC: %d\n",i,getU2(0));
						fprintf(OUTSTREAM,"\t\t\texception: nr: %d handlerPC: %d\n",i,getU2(0));
						fprintf(OUTSTREAM,"\t\t\texception: nr: %d catchType: %d\n",i,getU2(0));
										}
					u2 h;
					fprintf(OUTSTREAM,"\t\tCode: attributes_count: %d\n",	h=getU2(0));
					for (i=0;i< h;i++)	{
						u1*addr=getAddr(c->constant_pool[getU2(0)]+3);
						if (strncmp("LineNumberTable",(char*)addr, 15)==0 )	pc=getU4(0)+pc;	
						if (strncmp("LocalVariableTable",(char*)addr, 18)==0)	pc=getU4(0)+pc;	
										}		}
			if (strncmp("Exceptions", (char*)adr,10)==0)		{
			printf("exeception object\n");
			mN=n;
			u4 n=getU4(0);
			u2 i;
			n=getU2(0);
			for (i=0;i<n;i++)	{
			fprintf(OUTSTREAM,"\t\t\texception: nr: %d class: %d\n",i,getU2(0));
			}
			//pc=(u2)getU4(0)+pc;
			}
			if (strncmp("Synthetic", (char*)adr,9)==0)		pc+=4;
			if (strncmp("Deprecated",(char*) adr,10)==0)	pc+=4;								}
	}	// methods_count
}
	
void analyzeFields(classStructure* c){
	int n,a;
	u2 startStaticObject=heapTop-1;
	for (n=0; n<getU2(c->fields_count);n++)	{  //fields
		c->field_info[n]=pc; 																				// absolute in classfile
		fprintf(OUTSTREAM,"\tfield %x\taccess_flags: %d\n",n,getU2(0));
		fprintf(OUTSTREAM,"\tfield %d\tname: %d\n",n,getU2(0));
		fprintf(OUTSTREAM,"\tfield %d\tdescriptor: %d\n",n,getU2(0)); 						//Signature
		fprintf(OUTSTREAM,"\tfield %d\tattribute_count: %d\n",n,a=getU2(0));
		if (0x0008&getU2(pc-8))	 	// only static fields in class objects
			heapSetElement((slot)(u4)0x77, heapTop++); // initialize the heap elements	
		if ( a>0)	{
				pc+=6; // assume first is constant_value attribute
				if (getU1(CP(cN,getU2(pc)))==CONSTANT_String)	//string to do
				else		heapSetElement((slot)getU4(CP(cN,getU2(0))+1),heapTop-1); //   n+1 !!no double, float
				 pc+=(a-1)*6; // go over Synthetic and Deprecated attributes
					}
	}
	HEAPOBJECTMARKER(startStaticObject).length = heapTop-startStaticObject;
}
*/
u2 readClassFile(u1* fileName,u1* addr)		{

#ifdef LINUX
int fd;
u2 classFileLength=-(u2)((long)addr%(1<<16))-1;
if ((fd=open((char*)fileName,O_RDONLY))==-1) perror("no such file");
while (read(fd,addr++,1));
return classFileLength+=(long)addr;
#endif

#ifdef AVR8
u2 classFileLength=0;
printf("Laden der AnwendungsKlassen - Geben Sie ein  'w' ein\n"); 
// the damned holznagelsche protokoll zum laden eines bin files mit minikermit nachbilden
classFileLength=(*loadInSram)(addr);
return classFileLength;
#endif

#if (AVR32UC3A||AVR32AP7000)
int i;
char c=conIn(); // dummy w
if (c =='w')	{
conOut(4);//turn off echo
c=conIn();	//s
while ((c=conIn())=='p'){ 
//if (c=='p) // apage comes
c=conIn();
c=conIn();		// address
conOut('w');
for (i=0; i < 256;i++) *(addr++)=conIn();
conOut('w');}

conOut(5);// turn on echo uploadendS
while((c=conIn())==' ');
i=0;
do
{ i+=16*i+ ((c<='9')? (c-'0'): (c-'a'+10));
conOut(c);}
while ((c=conIn())!= 0xa);
return (u2)i;
}
else	{
//uploadends2
conOut(5); // turn on echo
return (u2) 0;
}

#endif

}