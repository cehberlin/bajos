/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
#if LINUX||AVR32LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"
#include "bajvm.h"
#include "heap.h"
#include <string.h>
#ifdef AVR8 
#include <avr/pgmspace.h>
#endif
#include "classfile.h"

extern const char* nativeClassNames[];
extern u2 numNativeClassNames;
extern const functionForNativeMethodType* funcArray[];
extern functionForNativeMethodType functionForNativePlatFormMethod[];


#ifdef AVR8
/* ARDUINOMEGA holds the classfiles in instruction flash!*/
/* RAMPZ = 1 (ever) -> classfiles are in te upper half of flash */
/* address of classfile byte in flash 0x0000 to 0xffff */
/* eg. address 0x2345 means byte in flash at address (binary):1001000110100101 */
extern u1 getU1Flash(u2 pos);
u1 getU1(u2 pos)	{ 
  return getU1Flash(CLASSSTA + ((pos==0) ? (pc++) : pos));
}
#else
/* classSTA and pc are global variables for actual class and method*/
/* parameter != 0 -> value at parameter-pos*/
/* parameter ==0 -> value at global var pc and automatic increment */
u1 getU1(u2 pos){return *(CLASSSTA + ( (pos==0) ? (pc++) : pos) );	}
#endif



u2 getU2(u2 pos){
if (pos==0) {pc+=2;return (*(CLASSSTA + pc-2))*256+*(CLASSSTA + pc-1);}
else return (*(CLASSSTA + pos)*256)+*(CLASSSTA + pos+1);
}
//  return (((u2)getU1(pos) << 8) | (u2)getU1 ( (pos == 0) ? 0 : pos+1 )); }	

u4 getU4(u2 pos){return (((u4)getU2(pos)<<16)|(u4)getU2((pos==0)?0:pos+2));	}

f4 getFloat(u2 pos)	{	return  getU4(pos);	}

//BH#define	getAddr(m)	(CLASSSTA+m)
void* getAddr(u2 pos)	{	return CLASSSTA+pos;	}

u1 findNumArgs(u2 methodRef)	{	/*  count BCDFIJLSZ in runden Klammern!!*/
	u2 i;
	u2 n=0;
	u1 object=0;
	u2 meth = CP(cN, getU2(CP(cN, getU2(CP(cN, methodRef)+3))+3));
	for (i = 0 ; i < getU2(meth+1) ; i++) {
		u1 c = getU1(meth+3+i);//BH*(u1*)(getAddr(meth+3)+i);
		if (c=='(' )			continue;
		if (c==')')				break;
		if (c==';')				object=0;
		if (!object) {
			if (c=='L') {
				object = 1; n++;
			} else if ((c=='B')||(c=='C')||(c=='F')||(c=='I')||(c=='S')||(c=='Z')) {
				n++;
			}
		}
	}
	return n;
}

u2	findMaxLocals()	{	/*cN,mN*/
	return getU2(getStartPC()-6);
}

/* in cN fieldName fieldDescr*/
/* out cN, fNC fNO */
/* return 1 -> found */
u1 findFieldByName(const char* fieldName,u1 fieldNameLength, const char* fieldDescr,u1 fieldDescrLength)	{
	fNO=0;
	do	{
		u1 numFields = getU2(cs[cN].fields_count); 
		for(fNC = 0 ; fNC < numFields ; ++fNC) {
			u2 fieldname = cs[cN].constant_pool[getU2(cs[cN].field_info[fNC] + 2)];
			u2 fielddescr = cs[cN].constant_pool[getU2(cs[cN].field_info[fNC] + 4)];
			if(	fieldNameLength == getU2(fieldname + 1) &&
#ifdef AVR8
strncmpFlashFlash
#else
strncmp
#endif
(fieldName,(const char*) getAddr(fieldname + 3), getU2(fieldname + 1)) == 0 &&
				fieldDescrLength == getU2(fielddescr + 1) &&
#ifdef AVR8
strncmpFlashFlash
#else
strncmp
#endif
(fieldDescr,(const char*) getAddr(fielddescr + 3), getU2(fielddescr + 1)) == 0)	{
						break;
			}
		}
		fNO += fNC;
		if (fNC < numFields) return 1; 
	} while (findSuperClass());
	return 0;
}

u1 findMethod(const char* className, const u1 classNameLength, const char* methodName, const u1 methodNameLength, const char* methodDescr, const u1 methodDescrLength)		{ 
// all args in flash for arduinomega
/*in cN, out: cN, mN*/
/* recursive down to object*/
//printf("%4x %4x %4x %4x\n", 0x8000+(unsigned int)className/2,classNameLength,0x8000+ (unsigned int)methodName/2,methodNameLength);
	if (!
#ifdef AVR8
findClassFlash
#else
findClass
#endif

		(className,classNameLength)) 					{ 
		CLASSNOTFOUNDERR(className);	}/* out: cN*/
	if (
#ifdef AVR8
findMethodByNameFlash
#else
findMethodByName
#endif
(methodName,methodNameLength,methodDescr,methodDescrLength))
			return 1;
	else	
		if (classNameLength == 16 &&
#ifdef	AVR8
strncmpRamFlash
#else
strncmp
#endif
("java/lang/Object",className,classNameLength) == 0) 		{
			return 0; /* not found*/
		} else 
			return findMethod(	getAddr(CP(cN,getU2(CP(cN,  
						getU2(cs[cN].super_class))+1))+3),
						getU2(CP(cN,getU2(CP(cN,  
						getU2(cs[cN].super_class))+1))+1),
 						methodName,
						methodNameLength,
						methodDescr,
						methodDescrLength);
}

#ifdef AVR8
u1 findMethodByNameFlash(const char* name, const u1 len, const char* methodDescr, const u1 methodDescrLength)	{
/*  in: classNumber cN, out: methodNumber mN*/
/* non recursive */
	for (mN=0; mN < getU2(cs[cN].methods_count); mN++)		
		if (len==getU2(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+2)]+1))
			if(strncmpFlashFlash(name,(char*)getAddr(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+2)]+3),
			getU2(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+2)]+1))==0)	{
				if (methodDescr!=NULL)	{		
					if (methodDescrLength==getU2(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+4)]+1))
						if(strncmpFlashFlash(methodDescr,(char*)getAddr(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+4)]+3),
								getU2(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+4)]+1))==0)			return 1;
												}
				else		return 1;																					}
	return 0;			
}
#endif

u1 findMethodByName(const char* name, const u1 len, const char* methodDescr, const u1 methodDescrLength)	{
/*  in: classNumber cN, out: methodNumber mN*/
/* non recursiv*/
	for (mN=0; mN < getU2(cs[cN].methods_count); mN++)		
		if (len==getU2(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+2)]+1))
			if(
#ifdef AVR8
strncmpRamFlash
#else
strncmp
#endif
(name,(char*)getAddr(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+2)]+3),
			getU2(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+2)]+1)) == 0)	{
				if (methodDescr!=NULL)	{		
					if (methodDescrLength==getU2(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+4)]+1))
						if(
#ifdef AVR8
strncmpRamFlash
#else
strncmp
#endif
(methodDescr,(char*)getAddr(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+4)]+3),
		getU2(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+4)]+1)) == 0)			return 1;
												}
				else		return 1;																					}
	return 0;			
}

u1* findMethodByMethodNumber()		{	/*mb jf  in: methodNumber, out: methodName*/
	u2 methAddr = METHODBASE(cN,mN);	/* get start address in class file of method #methodNumber*/
	u2 methodNameAddr = getU2(methAddr + 2);/* 2 ... offset from begin of method in class file to method's name*/
	return getAddr(CP(cN,methodNameAddr)+3);/* return pointer to field value in class cN at address methodNameAddress*/
}
/* return 1 found, 0 -> super class ist object*/
/* in cN out cN*/
u1 findSuperClass()	{
if (16 == getU2(cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].this_class)]+1)]+1) &&
#ifdef AVR8
 strncmpRamFlash
#else
 strncmp
#endif
("java/lang/Object",getAddr(cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].this_class)]+1)]+3), 16) ==0 )
return 0; /* cN is class Object */
#ifdef AVR8
findClassFlash
#else
findClass
#endif
(getAddr(cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].super_class)]+1)]+3), getU2(cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].super_class)]+1)]+1));
return 1;
}

#ifdef AVR8
u1 findClassFlash(const char* className,u1 classNameLength)	{  /* out: cN*/
	for (cN=0; cN < numClasses;cN++)			{
		if (classNameLength !=(u2)getU2(
				cs[cN].constant_pool[	
					getU2(cs[cN].constant_pool[
						getU2(cs[cN].this_class)]+1			
					)]+1)){
			continue;
		}
		if (strncmpFlashFlash(className,(const char*)getAddr(cs[cN].constant_pool[	
					getU2(	cs[cN].constant_pool[
						getU2(cs[cN].this_class)]+1		
					)
					]+3),classNameLength)==0){
			return 1;
		}
	}
	return 0;
}
#endif

u1 findClass(const char* className,u1 classNameLength)	{  /* out: cN*/
	for (cN=0; cN < numClasses;cN++)			{
		if (classNameLength !=(u2)getU2(
				cs[cN].constant_pool[	
					getU2(cs[cN].constant_pool[
						getU2(cs[cN].this_class)]+1			
					)]+1)){
			continue;
		}
		if (
#ifdef AVR8
strncmpRamFlash
#else
strncmp
#endif
(className,(const char*)getAddr(cs[cN].constant_pool[	
					getU2(	cs[cN].constant_pool[
						getU2(cs[cN].this_class)]+1			
					)
					]+3),classNameLength) == 0){
			return 1;
		}
	}
	return 0;
}

void analyzeClass(classStructure* c)	{
	u2 ln,i;
	u2 attribute_name_index;
	pc=0;
		/* static class object*/
		/* enough heap space for static class objects*/
		HEAPOBJECTMARKER(heapTop).status=HEAPALLOCATEDSTATICCLASSOBJECT;
/* static classfile object*/
		HEAPOBJECTMARKER(heapTop).magic=OBJECTMAGIC;
		HEAPOBJECTMARKER(heapTop).rootCheck=1;
		HEAPOBJECTMARKER(heapTop).mutex = MUTEXNOTBLOCKED;
		HEAPOBJECTMARKER(heapTop).length = 1; /* evtl. später überschreiben	*/
		cs[cN].classInfo.stackObj.pos=heapTop++;
		cs[cN].classInfo.stackObj.magic=OBJECTMAGIC;
		cs[cN].classInfo.stackObj.classNumber=cN;
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("class number:\t \t\t%X   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"), cN);
#else
	printf("class number:\t \t\t%X   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", cN);
#endif

#endif
		c->magic=pc;  /* relative position in classfile*/
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("cf\tmagic:\t %X\t"), getU4(pc));
#else
	printf("cf\tmagic:\t %X\t", getU4(pc));
#endif
	/* 0*/
#endif
		pc=4; /* NOT +=4, because getU*(pc) increases pc when pc = 0 */
		c->minor_version=pc++;			/* 4*/
		c->major_version=++pc;			/* 6*/
		if (getU2(c->major_version) > 49) 	{
			#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
				errorExit(-1, PSTR("this java version is not supported yet: c->major_version %x\n"), getU2(c->major_version));
			#else
				errorExit(-1, "this java version is not supported yet: c->major_version %x\n", getU2(c->major_version));
			#endif
			
		}
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("version:\t%d.%d\n"), getU2(pc),getU2(pc-2));
#else
	printf("version:\t%d.%d\n", getU2(pc),getU2(pc-2));
#endif

#endif
		pc+=2;
		c->constant_pool_count=pc;
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("cf\tconstant_pool_count:\t%d\n"), getU2(pc));
#else
	printf("cf\tconstant_pool_count:\t%d\n", getU2(pc));
#endif
			/*8*/
#endif
		pc+=2;
	if ((c->constant_pool=(u2*)malloc(sizeof(u2)*getU2(c->constant_pool_count)))==NULL) /* -2 möglich wegen 0!!*/

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("malloc error\n"));
#else
	printf("malloc error\n");
#endif

		analyzeConstantPool(c);
		c->access_flags=pc;
#ifdef DEBUG	

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("cf\taccess_flags: %x\n"),getU2(pc));
#else
	printf("cf\taccess_flags: %x\n",getU2(pc));
#endif

#endif
		pc+=2;
		c->this_class=pc;	
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("cf\tthis_class: %d\n"),getU2(pc));
#else
	printf("cf\tthis_class: %d\n",getU2(pc));
#endif

#endif
		pc+=2;
		c->super_class=pc;
#ifdef DEBUG	

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("cf\tsuper_class: %d\n"),getU2(pc));
#else
	printf("cf\tsuper_class: %d\n",getU2(pc));
#endif

#endif
		pc+=2;
		c->interfaces_count=pc;
#ifdef DEBUG	

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("cf\tinterfaces_count: %d\n"),getU2(pc));
#else
	printf("cf\tinterfaces_count: %d\n",getU2(pc));
#endif

#endif
		pc+=2;
		c->interfaces=pc;	
 		pc+= getU2(c->interfaces_count) * 2;
	/*
#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("cf\tinterfaces: %d\n"),getU2(c->interfaces));
#else
	printf("cf\tinterfaces: %d\n",getU2(c->interfaces));
#endif
*/
		c->fields_count=pc;
#ifdef DEBUG	

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("cf\tfields_count: %d\n"),getU2(pc));
#else
	printf("cf\tfields_count: %d\n",getU2(pc));
#endif

#endif
		pc+=2;
		c->field_info=NULL;
		if (getU2(c->fields_count)!=0)	{ 
			if ((	c->field_info=(u2*)malloc(sizeof(u2)*getU2(c->fields_count)))==NULL)

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("malloc error\n"));
#else
	printf("malloc error\n");
#endif

			analyzeFields(c);			} 
		c->methods_count=pc;
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("cf\tmethods_count: %d\n"),getU2(pc));
#else
	printf("cf\tmethods_count: %d\n",getU2(pc));
#endif

#endif
		pc+=2;
		c->method_info=NULL;
		if (getU2(c->methods_count)!=0)	{
			if((c->method_info=(u2*)malloc(2*sizeof(u2)*getU2(c->methods_count)))==NULL)

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("malloc error\n"));
#else
	printf("malloc error\n");
#endif

		analyzeMethods(c);				}
		ln=getU2(0);
	for (i=0; i <ln; i++){
		attribute_name_index=getU2(0); /* jetzt raus kriegen, was fuer ein atrr*/
		if (
#ifdef AVR8
strncmpRamFlash
#else
strncmp
#endif
("SourceFile",getAddr(c->constant_pool[attribute_name_index]+3),9) == 0)	{
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("cf-attributes: count: %d "),ln);
#else
	printf("cf-attributes: count: %d ",ln);
#endif


#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("attribute_length: %d"),getU4(pc));
#else
	printf("attribute_length: %d",getU4(pc));
#endif


#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("attribute_index: %d\n"),getU2(pc+4));
#else
	printf("attribute_index: %d\n",getU2(pc+4));
#endif

#endif
			pc+=6;																			}				}
}

void analyzeConstantPool(classStructure* c){
	u2 n,anz,length;
	anz=getU2(c->constant_pool_count);
	for (n=1; n<anz;n++){
		c->constant_pool[n]=pc;
		switch (getU1(0)){
			case	CONSTANT_Class:							/*    7 */
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\tcp %d\t:Class\t\t-> name:\t%d\n"),n,getU2(pc));
#else
	printf("\tcp %d\t:Class\t\t-> name:\t%d\n",n,getU2(pc));
#endif

#endif
pc+=2;
			CASE	CONSTANT_String:							/*    8 */
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\tcp %d\t:String\t\t-> string:\t%d\n"),n,getU2(pc));
#else
	printf("\tcp %d\t:String\t\t-> string:\t%d\n",n,getU2(pc));
#endif

#endif			
pc+=2;
			CASE	CONSTANT_Fieldref:							/*    9 */
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\tcp %d\t:Fieldref\t-> class: %d "),	n,getU2(pc));
#else
	printf("\tcp %d\t:Fieldref\t-> class: %d ",	n,getU2(pc));
#endif


#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("name_and_type:\t%d\n"),	getU2(pc+2));
#else
	printf("name_and_type:\t%d\n",	getU2(pc+2));
#endif

#endif
pc+=4;
			CASE	CONSTANT_InterfaceMethodref:				/*   11 */
#ifdef DEBUG					

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\tcp %d\t:InterfaceMethodref->class: %d "),n,getU2(pc));
#else
	printf("\tcp %d\t:InterfaceMethodref->class: %d ",n,getU2(pc));
#endif


#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("name_and_type_index\t%d\n"),getU2(pc+2));
#else
	printf("name_and_type_index\t%d\n",getU2(pc+2));
#endif

#endif
pc+=4;			
			CASE	CONSTANT_Methodref: 						/*    10  nur Methoden, die aufgerufen werden!!!*/
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\tcp %d\t:Methodref\t-> class: %d "),n,getU2(pc));
#else
	printf("\tcp %d\t:Methodref\t-> class: %d ",n,getU2(pc));
#endif


#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("name_and_type:\t%d\n"), getU2(pc+2));
#else
	printf("name_and_type:\t%d\n", getU2(pc+2));
#endif

#endif
pc+=4;
			CASE	CONSTANT_Integer:			/*    3*/
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("cp %d\t: Integer -> name:\t%d\n"),n,getU4(pc));
#else
	printf("cp %d\t: Integer -> name:\t%d\n",n,getU4(pc));
#endif

#endif
pc+=4;			
			CASE	CONSTANT_NameAndType:					/*    12*/
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\tcp %d\t:nameAndType\t-> name:\t%d "),n,getU2(pc));
#else
	printf("\tcp %d\t:nameAndType\t-> name:\t%d ",n,getU2(pc));
#endif


#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("descriptor:\t %d\n"),getU2(pc+2));
#else
	printf("descriptor:\t %d\n",getU2(pc+2));
#endif

#endif
pc+=4;			
			CASE	CONSTANT_Float:	 							/*    4 */
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\tcp %d\t:Float:\t%f  \n"),n,getFloat(pc));
#else
	printf("\tcp %d\t:Float:\t%f  \n",n,getFloat(pc));
#endif

#endif
pc+=4;
			CASE	CONSTANT_Long:							/*    5 */
					DNOTSUPPORTED;
			CASE	CONSTANT_Double:		/*    6 */
					DNOTSUPPORTED;
			CASE	CONSTANT_Utf8:			/*    1*/
					length=getU2(0);
#ifdef DEBUG
				int i;

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\tcp %d\t:Utf8:\t\t-> "),n);
#else
	printf("\tcp %d\t:Utf8:\t\t-> ",n);
#endif

					for (i=0; i < length;i++)

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("%c"),getU1(pc+i));
#else
	printf("%c",getU1(pc+i));
#endif
	/* utf8 chars ???*/

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\n"));
#else
	printf("\n");
#endif

#endif
pc+=length;
			DEFAULT:
				#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
					errorExit(-1, PSTR("invalid constant pool identifier\n"));
				#else
					errorExit(-1, "invalid constant pool identifier\n");
				#endif 
				
				}		}
}

void analyzeMethods(classStructure* c){ /* jan 08 not good tested*/
	int i,n,m,a;
	u2 etl;
c->nativeFunction=NULL;
	for (n=0; n<getU2(c->methods_count);n++)	{  /*methods*/
		c->method_info[n]=pc; /* absolute in classfile*/
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\tmethod %d\taccess_flags: %04x"),n,getU2(pc));
#else
	printf("\tmethod %d\taccess_flags: %04x",n,getU2(pc));
#endif


#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR(" name: %04x "),getU2(pc+2));
#else
	printf(" name: %04x ",getU2(pc+2));
#endif

		//BHDEBUGPRINTSTRING(METHODNAMESTR(cN,n),METHODNAMESTRLENGTH(cN,n));

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR(" descriptor: %04x "),getU2(pc+4));
#else
	printf(" descriptor: %04x ",getU2(pc+4));
#endif
 /*Signature*/
		//BHDEBUGPRINTSTRING(METHODDESCRSTR(cN,n),METHODDESCRSTRLENGTH(cN,n));

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR(" tattribute_count: %04x\n"),a=getU2(pc+6));
#else
	printf(" tattribute_count: %04x\n",a=getU2(pc+6));
#endif

#endif
a=getU2(pc+6);
pc+=8;		
		if (a==0) {
for (i=0; i < (/*sizeof(nativeClassNames)/sizeof(char*)*/numNativeClassNames); i++)
	if (!
#ifdef AVR8
strncmpRamFlash
#else
strncmp
#endif
(nativeClassNames[i],
		(char*)getAddr(c->constant_pool[getU2(c->constant_pool[getU2(c->this_class)]+1)]+3),
		getU2(c->constant_pool[getU2(c->constant_pool[getU2(c->this_class)]+1)]+1)) )
			{c->nativeFunction=(functionForNativeMethodType*)funcArray[i]; break;}
	continue; /* native method*/
}
		for (m=0; m<a;m++)									{ /* attributes of method*/
		const char* adr=getAddr(c->constant_pool[getU2(0)]+1+2);
			if (
#ifdef AVR8
strncmpRamFlash
#else
strncmp
#endif
("Code", adr,4) == 0)
	{
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\t\tCode: attribute_length: %d\n"),getU4(pc));
#else
	printf("\t\tCode: attribute_length: %d\n",getU4(pc));
#endif


#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\t\tCode: max_stack: %d\n"), getU2(pc+4));
#else
	printf("\t\tCode: max_stack: %d\n", getU2(pc+4));
#endif


#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\t\tCode: max_locals: %d\n"),getU2(pc+6));
#else
	printf("\t\tCode: max_locals: %d\n",getU2(pc+6));
#endif


#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\t\tCode: code_length: %d\n\t\t"),getU4(pc+8));
#else
	printf("\t\tCode: code_length: %d\n\t\t",getU4(pc+8));
#endif

#endif
pc+=12;				
#ifdef DEBUG
				for (i=0;i <getU4(pc-4);i++)
#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("%2x "),getU1(pc+i));
#else
	printf("%2x ",getU1(pc+i));
#endif
	 /*length*/
#endif				
pc+=getU4(pc-4);
etl=getU2(0);
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\n\t\tCode: exception_table_length: %d\n"),etl);
#else
	printf("\n\t\tCode: exception_table_length: %d\n",etl);
#endif
		/* exception_table	*/

					for (i=0;i <etl;i++)	{

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\t\t\texception: nr: %d startPC: %d\n"),i,getU2(pc+8*i));
#else
	printf("\t\t\texception: nr: %d startPC: %d\n",i,getU2(pc+8*i));
#endif


#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\t\t\texception: nr: %d endPC: %d\n"),i,getU2(8*i+2));
#else
	printf("\t\t\texception: nr: %d endPC: %d\n",i,getU2(8*i+2));
#endif


#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\t\t\texception: nr: %d handlerPC: %d\n"),i,getU2(8*i+4));
#else
	printf("\t\t\texception: nr: %d handlerPC: %d\n",i,getU2(8*i+4));
#endif


#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\t\t\texception: nr: %d catchType: %d\n"),i,getU2(8*i+6));
#else
	printf("\t\t\texception: nr: %d catchType: %d\n",i,getU2(8*i+6));
#endif

										}
#endif
pc+=etl*8;
					u2 h=getU2(0);
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\t\tCode: attributes_count: %d\n"),	h);
#else
	printf("\t\tCode: attributes_count: %d\n",	h);
#endif

#endif
					for (i=0;i< h;i++)	{
						const char*addr=getAddr(c->constant_pool[getU2(0)]+3);
						if (
#ifdef AVR8
strncmpRamFlash
#else
strncmp
#endif
("LineNumberTable",addr, 15) == 0 )	pc=getU4(0)+pc;	
						if (
#ifdef AVR8
strncmpRamFlash
#else
strncmp
#endif
("LocalVariableTable",addr, 18) == 0)	pc=getU4(0)+pc;	
										}		}
			if (
#ifdef AVR8
strncmpRamFlash
#else
strncmp
#endif
("Exceptions", adr,10) == 0)		{
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("exception object\n"));
#else
	printf("exception object\n");
#endif

#endif
			mN=n;
			u4 n2=getU4(0); /*attribute_length. don't need that.*/
			n2=getU2(0);
#ifdef DEBUG
			for (i=0;i<n2;i++)	

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\t\t\texception: nr: %d class: %d\n"),i,getU2(pc+2*i));
#else
	printf("\t\t\texception: nr: %d class: %d\n",i,getU2(pc+2*i));
#endif

#endif
pc+=2*n2;
			/*pc=(u2)getU4(0)+pc;*/
			}
			if (
#ifdef AVR8
strncmpRamFlash
#else
strncmp
#endif
("Synthetic", adr,9) == 0)		pc+=4;
			if (
#ifdef AVR8
strncmpRamFlash
#else
strncmp
#endif
("Deprecated", adr,10) == 0)	pc+=4;								}
	}	/* methods_count*/
}
	
void analyzeFields(classStructure* c){
	u2 n, a, cur_a;
	u2 startStaticObject = heapTop - 1;
    u2 numStaticFields=0;
	for (n=0; n<getU2(c->fields_count);n++)	{  /*fields*/
		c->field_info[n]=pc; 	/* absolute in classfile*/
#ifdef DEBUG

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\tfield %x\taccess_flags: %d\n"),n,getU2(pc));
#else
	printf("\tfield %x\taccess_flags: %d\n",n,getU2(pc));
#endif


#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\tfield %d\tname: %d\n"),n,getU2(pc+2));
#else
	printf("\tfield %d\tname: %d\n",n,getU2(pc+2));
#endif


#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\tfield %d\tdescriptor: %d\n"),n,getU2(pc+4));
#else
	printf("\tfield %d\tdescriptor: %d\n",n,getU2(pc+4));
#endif
 						/*Signature*/

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("\tfield %d\tattribute_count: %d\n"),n,getU2(pc+6));
#else
	printf("\tfield %d\tattribute_count: %d\n",n,getU2(pc+6));
#endif

#endif
		pc += 6;
		a = getU2(0);
/*		if (0x0008 & getU2(pc-8)) {// only static fields in class objects*/
			heapSetElement((slot)(u4)0x77, heapTop++); /* initialize the heap elements*/
/*		}*/
		numStaticFields++;
		for (cur_a = 0; cur_a < a; ++cur_a) {
			u2 attribute_name_index = getU2(0);
			u1 attribute_name = cs[cN].constant_pool[attribute_name_index];
			u4 attribute_length = getU4(0);

			if (getU1(attribute_name) != CONSTANT_Utf8) {
				#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
					errorExit(-1, PSTR("error while reading class file, CONSTANT_Utf8 expected\n"));
				#else
					errorExit(-1, "error while reading class file, CONSTANT_Utf8 expected\n");
				#endif	
			}

			if (getU2(attribute_name + 1) == 13 && 
#ifdef AVR8
strncmpRamFlash
#else
strncmp
#endif
("ConstantValue", getAddr(attribute_name + 3), 13) == 0) {
				if (attribute_length != 2) {
					#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
						errorExit(-1, PSTR("error while reading class file, ConstantValue_attribute should have a length of 2\n"));
					#else
						errorExit(-1, "error while reading class file, ConstantValue_attribute should have a length of 2\n");
					#endif	
					
				}
				u2 constantvalue_index = getU2(0);
				u1 constantvalue = cs[cN].constant_pool[constantvalue_index];
				if (getU1(constantvalue) == CONSTANT_String) {
                    u2 utf8 = cs[cN].constant_pool[getU2(constantvalue+1)];
                    if (getU1(utf8) != CONSTANT_Utf8) {
				#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
					errorExit(-1, PSTR("error while reading class file, CONSTANT_String target is no Utf8 entry but %d\n"), getU1(utf8));
				#else
					errorExit(-1, "error while reading class file, CONSTANT_String target is no Utf8 entry but %d\n", getU1(utf8));
				#endif	
                        
                    }

                    u2 space = getFreeHeapSpace(getU2(utf8+1));
                    int i;
                    for (i = 0 ; i < getU2(utf8+1) ; ++i) {
                        heapSetElement((slot)(u4)getU2(utf8+1+1+i), space++);
                    }
				} else {
					heapSetElement((slot)getU4(constantvalue+1),heapTop-1); /*   n+1 !!no double, float*/
				}
			} else {
				pc += attribute_length; /* continue*/
			}
		}
	}
	HEAPOBJECTMARKER(startStaticObject).length = (numStaticFields > 0)? heapTop-startStaticObject : 1;
}

u2	getStartPC()	{	/*cN,mN*/
	u2 attrLength = 0;  /* search code-position*/
	u2 i;
for (i=0;i<getU2(METHODBASE(cN,mN)+6);i++) 		{/* number of attributes*/
if(
#ifdef AVR8
strncmpRamFlash
#else
strncmp
#endif
(	"Code",
			getAddr(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+8+attrLength)]+3),
			4) == 0)
			return (u2)METHODBASE(cN,mN)+8+14+attrLength;
			attrLength=getU4(METHODBASE(cN,mN)+8)+6;/*+attrLength;		????*/
			}	 /* < 64K	*/
return -1;
}
