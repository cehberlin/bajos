/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...
// 14.8.06 sie lief das erste mal durch (in Unix)
// april 2007-> threads, scheduling, funtionsüberladung, native-dispatch
// mai 2007 -> garbage collection, exception handling
// jan08 synchronized, wait notify
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "typedefinitions.h"
#include "definitions.h"
#include "bajvm.h"
#include "stack.h"
#include "heap.h"
#include "classfile.h"
#include "interpreter.h"
#include "scheduler.h"
#include "native.h"

#define GETSTARTPC(offset)	((strncmp(	"Code",\
			getAddr( cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+8+offset)]+3	),\
			4)==0)? (u2)METHODBASE(cN,mN)+8+14+offset :\
			GETSTARTPC(offset+getU4(METHODBASE(cN,mN)+8)+6))

#define fieldName		name
#define fieldNameLength		nameLength
#define	lengthArray		nameLength
#define methodName		name
#define	methodNameLength	nameLength
#define fieldDescr		descr
#define fieldDescrLength	descrLength
#define methodDescr		descr
#define methodDescrLength	descrLength


static slot	first;
static slot	second;
static slot	third;
static slot 	fourth;
static u1*	className;;
static u2 	classNameLength;
static u1* 	name;		// field or method
static u2 	nameLength;
static u1* 	descr;		// field or method
static u2	descrLength;
// static u1	numFields; <-- not used?
static u2 	i,j,k;
static s2	count;


void run(){	// in: classNumber,  methodNumber cN, mN
u1 code, byte1, byte2;
pc	= getStartPC();
DEBUGPRINTSTACK;
do {		//while(1)
code	= getU1(0);
byte1	= getU1(pc);
byte2	= getU1(pc+1);
DEBUGPRINTE(actualThreadCB->tid,x);
DEBUGPRINT2(", pc: %x ",pc-getStartPC()-1);
DEBUGPRINTE(code,2x);
DEBUGPRINTE(byte1,2x);
DEBUGPRINTE(byte2,2x\t);
switch (code)	{
	CASE	NOP: 	DEBUGPRINTLN1("NOP");
	CASE	ACONST_NULL: 	DEBUGPRINTLN1("ACONSTANT_NULL -> push\t...,=> NULLOBJECT");
			opStackPush(NULLOBJECT);
			DEBUGPRINTSTACK;
	CASE	ICONST_M1:
	case	ICONST_0:
	case 	ICONST_1:	
	case	ICONST_2:
	case	ICONST_3:
	case	ICONST_4:	
	case	ICONST_5:DEBUGPRINTLN3("ICONST_%4x -> push\t...,=> %4x",code-ICONST_0,code - ICONST_0);
			opStackPush(( slot)(s4)(code - ICONST_0));
			DEBUGPRINTSTACK;
			DEBUGPRINTLOCALS;
	CASE	LCONST_0:
	case	LCONST_1:PRINT1EXIT("LCONST_%d -> nicht realisiert\n",code - LCONST_0,4);
	CASE	FCONST_0:
	case	FCONST_1:
	case	FCONST_2:DEBUGPRINTLN3("FCONST_%d  -> push\t...,=> %f",
				code - FCONST_0,(f4)(code - FCONST_0));
			opStackPush((slot)(f4)(code - FCONST_0));
			DEBUGPRINTSTACK;
	CASE DCONST_0:
	case	DCONST_1:	PRINT1EXIT("DCONST_%d -> nicht realisiert\n",code - DCONST_0,4);
	CASE	BIPUSH:	DEBUGPRINTLN2("BIPUSH  -> push\t...,=> %d",(s1)byte1);
				// BIPUSH is defined as follows:
				// reads a signed 8 bit constant from byte1,
				// extends it to int signed (32bit)
				// and saves it on stack
			opStackPush( (slot) (((getU1(pc) & 0x80) >> 7) * 0xffffff80 | (getU1(0) & 0x7f)) );
			DEBUGPRINTSTACK;
			DEBUGPRINTLOCALS;
			DEBUGPRINTHEAP;		
	CASE	SIPUSH:	DEBUGPRINTLN2("SIPUSH  -> push\t...,=> %x",(s2)BYTECODEREF);
			opStackPush(( slot)((s4)((s2)getU2(0))));
			DEBUGPRINTSTACK;	
	CASE	LDC: 	if(getU1(CP(cN,getU1(0))) == CONSTANT_String)	{
				first.stackObj.magic=CPSTRINGMAGIC;
				first.stackObj.classNumber=cN;
				first.stackObj.pos=(u2)(/*((u2)cN <<8)*/+byte1);
				opStackPush(first);							}
			else 	opStackPush(( slot)getU4(CP(cN,byte1)+1));	// int or float const value on stack
			DEBUGPRINTLN2("ldc  push\t...,=> x%x",opStackPeek().UInt);
			DEBUGPRINTSTACK;
			DEBUGPRINTHEAP;
	CASE	LDC_W:	PRINTSEXIT("LDC_W",nry,4);
	CASE	LDC2_W:	PRINTSEXIT("LDC2_W",nry,4);
	CASE	ILOAD:	DEBUGPRINTLN2("ILOAD -> local(%x) -> push\t...,=>",byte1);
			opStackPush(opStackGetValue(local+getU1(0)));	//mb jf 
			DEBUGPRINTSTACK;
			DEBUGPRINTLOCALS;
	CASE	LLOAD:	PRINTSEXIT("LLOAD",nry,4);
	CASE	FLOAD:	DEBUGPRINTLN3("FLOAD -> local(%d: %d) -> push\t...,=>",byte1,getU1(byte1));
			opStackPush(opStackGetValue(local+getU1(0)));	
			DEBUGPRINTSTACK;
			DEBUGPRINTLOCALS;
	CASE	DLOAD:	PRINTSEXIT("DLOAD",nry,4);
	CASE	ALOAD:	DEBUGPRINTLN2("ALOAD -> local(%x) -> push\t,=>",byte1);
			opStackPush(opStackGetValue(local+getU1(0)));	//mb jf changed getU1() --> getU1(0)
			DEBUGPRINTSTACK;
			DEBUGPRINTLOCALS;		
	CASE	ILOAD_0:				
	case	ILOAD_1:
	case	ILOAD_2:
	case	ILOAD_3:DEBUGPRINTLN2("ILOAD_%d local -> push\t...,=>",code - ILOAD_0);
			opStackPush(opStackGetValue(local + code - ILOAD_0));
			DEBUGPRINTSTACK;
			DEBUGPRINTLOCALS;
			DEBUGPRINTHEAP;
	CASE	FLOAD_0:
	case	FLOAD_1:
	case	FLOAD_2:
	case	FLOAD_3:DEBUGPRINTLN2("FLOAD_%d local -> push\t...,=>",code - FLOAD_0);
			opStackPush(opStackGetValue(local + code - FLOAD_0));
			DEBUGPRINTSTACK;
			DEBUGPRINTLOCALS;
	CASE	DLOAD_0:
	case	DLOAD_1:
	case	DLOAD_2:
	case	DLOAD_3:PRINT1EXIT("DLOAD_%d -> nicht realisiert\n",code - DLOAD_0,4);	//mb jf changed output to "DLOAD..."
	CASE	ALOAD_0:
	case	ALOAD_1:
	case	ALOAD_2:
	case	ALOAD_3:DEBUGPRINTLN2("ALOAD_%d local -> push\t...,=>",code - ALOAD_0);
			opStackPush(opStackGetValue(local + code - ALOAD_0));
			DEBUGPRINTSTACK;
			DEBUGPRINTLOCALS;
	CASE	IALOAD:
	case	FALOAD:
	case	AALOAD:
	case	BALOAD:
	case	CALOAD:
	case	SALOAD:	count = (s2)opStackPop().Int;			//mb jf	
			if((first = opStackPeek()).UInt == NULLOBJECT.UInt)	{NULLPOINTEREXCEPTION;}
						/* a string is not an array!!!*/
			lengthArray=first.stackObj.arrayLength;
			if (count < 0 || lengthArray < count||count > (MAXHEAPOBJECTLENGTH-1))	
				raiseExceptionFromIdentifier("java/lang/ArrayIndexOutOfBoundsException", 40);
			opStackPoke(( slot)heapGetElement((u2)first.stackObj.pos/*UInt*/+count+1));
#ifdef DEBUG
			switch (code)								{
				case IALOAD:	DEBUGPRINTLN2("iaload %x,=>",first.UInt);
				CASE FALOAD:	DEBUGPRINTLN2("faload %x =>",first.UInt);
				CASE AALOAD:	DEBUGPRINTLN2("aaload %x, =>",first.UInt);
				CASE BALOAD:	DEBUGPRINTLN2("baload %x, =>",first.UInt);
				CASE CALOAD:	DEBUGPRINTLN2("caload %x, =>",first.UInt);
				CASE SALOAD:	DEBUGPRINTLN2("saload %x, =>",first.UInt);	}
#endif			
			DEBUGPRINTLN2(", %x",heapGetElement((u2)first.UInt+count+1).UInt);
			DEBUGPRINTSTACK;
			DEBUGPRINTHEAP;
	CASE	LALOAD:	PRINTSEXIT("laload",nry,4);	//mb jf
	CASE	DALOAD:	PRINTSEXIT("daload",nry,4);	//mb jf
	CASE	ISTORE:
	case	FSTORE:
	case	ASTORE:
#ifdef DEBUG
			switch (code)	{
				case	ISTORE:	DEBUGPRINTLN2("ISTORE  pop -> local(%d)=>,\n",byte1);
				CASE	FSTORE:	DEBUGPRINTLN2("FSTORE  pop -> local(%d)=>,\n",byte1);
				CASE	ASTORE:	DEBUGPRINTLN2("ASTORE  pop -> local(%x)=>,\n",byte1);
					}
#endif
			opStackSetValue(local+getU1(0),opStackPop());
			DEBUGPRINTSTACK;
			DEBUGPRINTLOCALS;
	CASE	LSTORE:	PRINTSEXIT("LSTORE",nry,4);	
	CASE	DSTORE:	PRINTSEXIT("DSTORE",nry,4);
	CASE	ISTORE_0:
	case	ISTORE_1:
	case	ISTORE_2:
	case	ISTORE_3:DEBUGPRINTLN3("ISTORE_%d pop -> local   %d=>,",code-ISTORE_0,opStackPeek().Int);
			opStackSetValue(local + code - ISTORE_0,opStackPop());
			DEBUGPRINTSTACK;	
			DEBUGPRINTLOCALS;	
	CASE	LSTORE_0:
	case	LSTORE_1:
	case	LSTORE_2:
	case	LSTORE_3:PRINT1EXIT("LSTORE_%d nicht realisiert\n",code - LSTORE_0,4);
	CASE	FSTORE_0:
	case	FSTORE_1:
	case	FSTORE_2:
	case	FSTORE_3:DEBUGPRINTLN2("FSTORE_%d pop -> local   =>,",code - FSTORE_0);
			opStackSetValue(local+code - FSTORE_0,opStackPop());
			DEBUGPRINTSTACK;
			DEBUGPRINTLOCALS;
	CASE	DSTORE_0:
	case	DSTORE_1:
	case	DSTORE_2:
	case	DSTORE_3:PRINT1EXIT("DSTORE_%d -> nicht realisiert\n",code - DSTORE_0,4);
	CASE	ASTORE_0:
	case	ASTORE_1:
	case	ASTORE_2:
	case	ASTORE_3:DEBUGPRINTLN2("ASTORE_%d pop -> local  =>,",code-ASTORE_0);
			opStackSetValue(local+code - ASTORE_0,opStackPop());
			DEBUGPRINTSTACK;
			DEBUGPRINTLOCALS;
	CASE	IASTORE:
	case	FASTORE:
	case	AASTORE:
	case	BASTORE:
	case	CASTORE:
	case	SASTORE:
			DEBUGPRINTLN1("xASTORE");
			DEBUGPRINTSTACK;
			second = opStackPop();
			count = (s2)(opStackPop().Int);
			first = opStackPop();
			lengthArray=first.stackObj.arrayLength;				
			if(first.UInt==((slot)NULLOBJECT).UInt)						{
				printf("count %d %d\n",count,lengthArray);
				raiseExceptionFromIdentifier("java/lang/NullPointerException", 30);
			}
			if	(count < 0 	|| lengthArray < count||count > (MAXHEAPOBJECTLENGTH-1))	
				raiseExceptionFromIdentifier("java/lang/ArrayIndexOutOfBoundsException", 40);
			heapSetElement(second,first.stackObj.pos+count+1);
			DEBUGPRINTSTACK;
			DEBUGPRINTHEAP;
#ifdef DEBUG
			switch (code)		{
				case	IASTORE:	DEBUGPRINTLN1("iastore stack -> local");	//mb jf		//int
				CASE	FASTORE:	DEBUGPRINTLN1("fastore");	//mb jf		//float
				CASE	AASTORE:	DEBUGPRINTLN1("fastore");	//mb jf		//float
				CASE	BASTORE:	DEBUGPRINTLN1("bastore");	//mb jf		//byte or boolean
				CASE	CASTORE:	DEBUGPRINTLN1("castore");	//mb jf		//char
				CASE	SASTORE:	DEBUGPRINTLN1("sastore");	//mb jf		//short
						}
#endif
	CASE	LASTORE:PRINTSEXIT("lastore",nry,4);	//mb jf
	CASE	DASTORE:PRINTSEXIT("dastore",nry,4);	//mb jf
	CASE	POP:	DEBUGPRINTLN2("POP %x",opStackPeek().UInt);
			opStackPop();	
			DEBUGPRINTSTACK;
	CASE	POP2:	DEBUGPRINTLN1("POP2");
			opStackPop();	
			opStackPop();	
			DEBUGPRINTSTACK;					
	CASE	DUP:	DEBUGPRINTLN1("dup");
			opStackPush(opStackPeek());
			DEBUGPRINTSTACK;
	CASE	DUP_X1:	DEBUGPRINTLN1("DUP_X1"); 
			second = opStackPop();
			first = opStackPop();
			opStackPush( second);
			opStackPush(first);
			opStackPush(second);
			DEBUGPRINTSTACK;
	CASE	DUP_X2:	DEBUGPRINTLN1("DUP_X2");
			second = opStackPop();
			first = opStackPop();
			third = opStackPop();
			opStackPush( second);
			opStackPush(third);
			opStackPush(first);
			opStackPush(second);
			DEBUGPRINTSTACK;	
	CASE	DUP2:	DEBUGPRINTLN1("DUP2");
			second = opStackPop();
			first = opStackPop();
			opStackPush(first);
			opStackPush(second);
			opStackPush(first);
			opStackPush(second);
			DEBUGPRINTSTACK;	
	CASE	DUP2_X1:DEBUGPRINTLN1("DUP2_X1");
			second = opStackPop();
			first = opStackPop();
			third = opStackPop();
			opStackPush(first);
			opStackPush(second);
			opStackPush(third);
			opStackPush(first);
			opStackPush(second);
			DEBUGPRINTSTACK;
	CASE	DUP2_X2:DEBUGPRINTLN1("DUP2_X2");
			second = opStackPop();
			first = opStackPop();
			third = opStackPop();
			fourth=opStackPop();
			opStackPush(first);
			opStackPush(second);
			opStackPush(fourth);
			opStackPush(third);
			opStackPush(first);
			opStackPush(second);
			DEBUGPRINTSTACK;
	CASE	SWAP:	DEBUGPRINTLN1("SWAP");
			second = opStackPop();
			first= opStackPop();
			opStackPush(second);
			opStackPush(first);
			DEBUGPRINTSTACK;
	CASE	IADD:	DEBUGPRINTLN1("IADD");
			opStackPoke(( slot)(opStackPop().Int + opStackPeek().Int));
			DEBUGPRINTSTACK;
	CASE	LADD:	PRINTSEXIT("LADD",nry,4);
	CASE	FADD:	DEBUGPRINTLN1("FADD");
			opStackPoke(( slot)(opStackPop().Float + opStackPeek().Float));
			DEBUGPRINTSTACK;	
			DEBUGPRINTLOCALS;
	CASE	DADD:	PRINTSEXIT("DADD",nry,4);
	CASE	ISUB:	DEBUGPRINTLN1("ISUB");
			first = opStackPop();		//mb fj changed substraction order
			opStackPoke(( slot)(opStackPeek().Int - first.Int));
			DEBUGPRINTSTACK;
	CASE	LSUB:	PRINTSEXIT("Lsub",nry,4);
	CASE	FSUB:	DEBUGPRINTLN1("Fsub");	
			first = opStackPop();		//mb fj changed substraction order
			opStackPoke(( slot)(opStackPeek().Float - first.Float));
			DEBUGPRINTSTACK;	
	CASE	DSUB:	PRINTSEXIT("Dsub",nry,4);
	CASE	IMUL:	DEBUGPRINTLN1("IMUL");
			opStackPoke(( slot)(opStackPop().Int * opStackPeek().Int));
			DEBUGPRINTSTACK;
	CASE	LMUL:	PRINTSEXIT("LMUL",nry,4);
	CASE	FMUL:	DEBUGPRINTLN1("FMUL");
			opStackPoke(( slot)(opStackPop().Float * opStackPeek().Float));
			DEBUGPRINTSTACK;	//mb jf
	CASE	DMUL:	PRINTSEXIT("DMUL",nry,4);
	CASE	IDIV:	DEBUGPRINTLN1("IDIV");
			first = opStackPop();		//mb fj changed dividend order
			if(first.Int == 0)	handleArithmeticException();
			else 			opStackPush(( slot)( opStackPop().Int / first.Int));
			DEBUGPRINTSTACK;
	CASE	LDIV:	PRINTSEXIT("LDIV",nry,4);
	CASE	FDIV:	DEBUGPRINTLN1("FDIV");
			first = opStackPop();		//mb fj changed dividend order
			if(first.Float == 0.0)	handleArithmeticException();
			else			opStackPoke(( slot)(opStackPeek().Float / first.Float));
			DEBUGPRINTSTACK;
	CASE	DDIV:	PRINTSEXIT("DDIV",nry,4);		
	CASE	IREM:	DEBUGPRINTLN1("IREM");
			if (( (first=opStackPop()).Int)==0)	ARITHMETICEXCEPTION	// ohne Semikolon!!
			else					opStackPoke((slot)(opStackPeek().Int % first.Int));
			DEBUGPRINTSTACK;
	CASE	LREM:	PRINTSEXIT("LREM",nry,4);
	CASE	FREM:	DEBUGPRINTLN1("FREM");//???	
			opStackPoke(( slot)(f4)((opStackPop().Float / opStackPeek().Float)));
			DEBUGPRINTSTACK;	
	CASE	DREM:	PRINTSEXIT("DREM",nry,4);
	CASE	INEG:	DEBUGPRINTLN1("INEG");
			opStackPoke(( slot)(-opStackPeek().Int));
			DEBUGPRINTSTACK;
	CASE	LNEG:	PRINTSEXIT("LNEG",nry,4);
	CASE	FNEG:	DEBUGPRINTLN1("FNEG");	
			opStackPoke(( slot)(-opStackPeek().Float));
			DEBUGPRINTSTACK;	
	CASE	DNEG:	PRINTSEXIT("DNEG",nry,4);
	CASE	ISHL:	DEBUGPRINTLN1("ISHL");
			opStackPoke(( slot)(opStackPop().UInt << opStackPeek().UInt));
			DEBUGPRINTSTACK;
	CASE	LSHL:	PRINTSEXIT("LSHL",nry,4);
	CASE	ISHR:	DEBUGPRINTLN1("ISHR");
			opStackPoke(( slot)(opStackPop().Int >> opStackPeek().Int));
			DEBUGPRINTSTACK;
	CASE	LSHR:	PRINTSEXIT("LSHR",nry,4);
	CASE	IUSHR:	DEBUGPRINTLN1("IUSHR");
			opStackPoke(( slot)(opStackPop().UInt >> opStackPeek().UInt));
			DEBUGPRINTSTACK;
	CASE	LUSHR:	PRINTSEXIT("LUSHR",nry,4);
	CASE	IAND:	DEBUGPRINTLN1("IAND");
			opStackPoke(( slot)(opStackPop().UInt & opStackPeek().UInt));
			DEBUGPRINTSTACK;
	CASE	LAND:	PRINTSEXIT("LAND",nry,4);
	CASE	IOR:	DEBUGPRINTLN1("IOR");
			opStackPoke(( slot)(opStackPop().UInt | opStackPeek().UInt));
			DEBUGPRINTSTACK;
	CASE	LOR:	PRINTSEXIT("LOR",nry,4);
	CASE	IXOR:	DEBUGPRINTLN1("IXOR");
			opStackPoke(( slot)(opStackPop().UInt ^ opStackPeek().UInt));
			DEBUGPRINTSTACK;
	CASE	LXOR:	PRINTSEXIT("LXOR",nry,4);
	CASE	IINC:	DEBUGPRINTLN1("IINC");	//mb, jf
			DEBUGPRINTLOCALS;
			opStackSetValue((u2)(local + byte1), // position
					( slot)((s4)(opStackGetValue(local + byte1).Int	// old value
					 + (s4)(s1)byte2))); // add const
			pc += 2;	// to skip the index + const
			DEBUGPRINTSTACK;
			DEBUGPRINTLOCALS;
	CASE	I2L:	PRINTSEXIT("I2L",nry,4);
	CASE	I2F:	DEBUGPRINTLN1("I2F");
			opStackPoke(( slot)(f4)opStackPeek().Int);
			DEBUGPRINTSTACK;
	CASE	I2D:	PRINTSEXIT("I2D",nry,4);
	CASE	L2I:	PRINTSEXIT("L2I",nry,4);
	CASE	L2F:	PRINTSEXIT("L2F",nry,4);
	CASE	L2D:	PRINTSEXIT("L2D",nry,4);
	CASE	F2I:	DEBUGPRINTLN1("F2I");
			opStackPoke(( slot)(s4)(opStackPeek().Float));
			DEBUGPRINTSTACK;
	CASE	F2L:	PRINTSEXIT("F2L",nry,4);
	CASE	F2D:	PRINTSEXIT("F2D",nry,4);
	CASE	D2I:	PRINTSEXIT("D2I",nry,4);
	CASE	D2L:	PRINTSEXIT("D2L",nry,4);
	CASE	D2F:	PRINTSEXIT("D2F",nry,4);
	CASE	I2C:	DEBUGPRINTLN1("I2C");
			opStackPoke((slot)(opStackPeek().UInt & 0x0000ffff));
			DEBUGPRINTSTACK;
	CASE	I2B:	DEBUGPRINTLN1("I2B");
			opStackPoke(( slot)(opStackPeek().Int >> 24));
			DEBUGPRINTSTACK;
	CASE	I2S:	DEBUGPRINTLN1("I2S");
			opStackPoke(( slot)(s4)((s2)opStackPeek().Int ));
			DEBUGPRINTSTACK;
	CASE	LCMP:	PRINTSEXIT("LCMP",nry,4);
	CASE	FCMPL:		
	case	FCMPG:	DEBUGPRINTLN1("fcmpg");
			second = opStackPop();
			first = opStackPop();
			if (first.Float == 0x7fc00000 || second.Float == 0x7fc00000) 	{
						opStackPush((slot)(s4) (code == FCMPG ? 1 : -1));
											} 
			else 	if(first.Float > second.Float) {
							opStackPush(( slot)(s4)1);
						} 
				else 	if(first.Float == second.Float) { 	// corrected by al june 08
							opStackPush(( slot)(s4)0);
						} 
					else {
							opStackPush(( slot)(s4)-1);
						}
			DEBUGPRINTSTACK;
			DEBUGPRINTHEAP;
	CASE	DCMPL:	PRINTSEXIT("DCMPL",nry,4);
	CASE	DCMPG:	PRINTSEXIT("DCMPG",nry,4);
	CASE	IFEQ:	DEBUGPRINTLN1("ifeq");		//mb, jf
			if(opStackPop().Int == 0)
			pc+= (s2)((byte1 << 8) | (byte2))-1;
			else	pc += 2;// to skip the jump-adress
			DEBUGPRINTSTACK;
			DEBUGPRINTHEAP;
	CASE	IFNULL:	if((u2)opStackPop().UInt == 0)	pc+= BYTECODEREF-1;	// add offset to pc at ifnull-address
			else				pc += 2;	// skip branch bytes
			DEBUGPRINTSTACK;
	CASE	IFNONNULL:	DEBUGPRINTLN1("ifnonnull");	// mb jf
			if( (u2)opStackPop().UInt != 0)	pc+= BYTECODEREF-1;	// add offset to pc at ifnull-address
			else				pc+= 2;	// skip branch bytes
			DEBUGPRINTSTACK;
	CASE	IFNE:	DEBUGPRINTLN1("ifne");		//mb, jf
			if(opStackPop().Int != 0)	pc+= (s2)((byte1 << 8) | (byte2))-1;
			else				pc += 2;	// to skip the jump-adress
			DEBUGPRINTSTACK;
			DEBUGPRINTHEAP;
	CASE	IFLT:	DEBUGPRINTLN1("iflt");		//mb, jf
			if(opStackPop().Int < 0)	pc+= (s2)((byte1 << 8) | (byte2))-1;
			else				pc += 2;	// to skip the jump-adress
			DEBUGPRINTSTACK;
			DEBUGPRINTHEAP;		
	CASE	IFLE:	DEBUGPRINTLN1("ifle");		//mb, jf
			if(opStackPop().Int <= 0)	pc+= (s2)((byte1 << 8) | (byte2))-1;
			else				pc += 2;	// to skip the jump-adress
			DEBUGPRINTSTACK;
			DEBUGPRINTHEAP;
	CASE	IFGT:	DEBUGPRINTLN1("ifgt");		//mb, jf
			if(opStackPop().Int > 0)	pc+= (s2)((byte1 << 8) | (byte2))-1;
			else				pc += 2;	// to skip the jump-adress
			DEBUGPRINTSTACK;
			DEBUGPRINTHEAP;
	CASE	IFGE:	DEBUGPRINTLN1("ifge");		//mb, jf
			if(opStackPop().Int >= 0)	pc+= (s2)((byte1 << 8) | (byte2))-1;
			else				pc += 2;	// to skip the jump-adress
			DEBUGPRINTSTACK;
			DEBUGPRINTHEAP;
	CASE	IF_ACMPEQ:	DEBUGPRINTLN1("if_acmpeq");		//mb, jf
	case	IF_ICMPEQ:	DEBUGPRINTLN1("if_icmpeq");		//mb, jf
			if(opStackPop().Int == opStackPop().Int)	pc+= (s2)((u2)((byte1 << 8) | (byte2)))-1;
			else						pc+= 2;	// to skip the jump-adress
			DEBUGPRINTSTACK;
			DEBUGPRINTHEAP;
	CASE	IF_ACMPNE:	DEBUGPRINTLN1("if_acmpne");		//mb, jf
	case	IF_ICMPNE:	DEBUGPRINTLN1("if_icmpne");		//mb, jf
			if(opStackPop().Int != opStackPop().Int)	pc+= (s2)((u2)((byte1 << 8) | (byte2)))-1;
			else						pc+= 2;	// to skip the jump-adress
			DEBUGPRINTSTACK;
			DEBUGPRINTHEAP;
	CASE	IF_ICMPLT:	DEBUGPRINTLN1("if_icmplt");		//mb, jf
			if(opStackPop().Int > opStackPop().Int) /*???*/	pc+= (s2)((u2)((byte1 << 8) | (byte2)))-1;
			else						pc+= 2;	// to skip the jump-adress
			DEBUGPRINTSTACK;
			DEBUGPRINTHEAP;
	CASE	IF_ICMPGE:	DEBUGPRINTLN1("if_icmpge");		//mb, jf
			if(opStackPop().Int <= opStackPop().Int)	pc+= (s2)((u2)((byte1 << 8) | (byte2)))-1;
			else						pc+= 2;	// to skip the jump-adress
			DEBUGPRINTSTACK;
			DEBUGPRINTHEAP;
	CASE	IF_ICMPGT:	DEBUGPRINTLN1("if_icmpgt");		//mb, jf
			if(opStackPop().Int < opStackPop().Int)		pc+= (s2)((u2)((byte1 << 8) | (byte2)))-1;
			else						pc+= 2;	// to skip the jump-adress
			DEBUGPRINTSTACK;
			DEBUGPRINTHEAP;
	CASE	IF_ICMPLE:	DEBUGPRINTLN1("if_icmple");		//mb, jf
			if(opStackPop().Int >= opStackPop().Int)	pc+= (s2)((u2)((byte1 << 8) | (byte2)))-1;
			else						pc+= 2;	// to skip the jump-adress
			DEBUGPRINTSTACK;
	CASE	GOTO:	DEBUGPRINTLN1("goto");	// mb, jf
			pc+= (s2)BYTECODEREF-1;
	CASE	JSR:	DEBUGPRINTLN1("jsr (not tested)");	// mb, jf
			// not tested because no exceptions implemented yet 14.12.2006
			// the opcode of athrow is required
			opStackPush(( slot)(u4)(pc+2));
			pc += (s2)BYTECODEREF;
			DEBUGPRINTSTACK;
	CASE	RET:	DEBUGPRINTLN1("ret (not tested)");	// mb, jf
			pc = opStackGetValue(local+getU2(0)).UInt+getStartPC();
			DEBUGPRINTSTACK;
	CASE	TABLESWITCH:	DEBUGPRINTLN1("tableswitch");	// mb, jf
					{
							/*
							aa		tableswitch
							0  0  0 	padding
							0 0 0 2		DefaultByte
							0 0 0 1		LowByte
							0 0 0 3		HighByte
							0  0  0 1c	Offset für 1
							0  0  0 22	Offset für 2
							0  0  0 28	Offset für 3
							*/
						
						u2 startPc = --pc;
						u2 relPc = pc - getStartPC();//pcMethodStart;	//calculate relative PC for ByteCode in Method
						u4 windex = opStackPop().Int;
						relPc = (u2)((relPc + 4) & 0xfffc);	// next pc as multiple of 4 --> skip padding bytes
						pc = relPc + getStartPC();//pcMethodStart;	// set pc to begin of default address
						u4 offset = (u4)((u4)getU1(pc++)<<24 | (u4)getU1(pc++)<<16 | (u4)getU1(pc++)<<8 | getU1(pc++));	// default offset
						u4 lowbyte = (u4)((u4)getU1(pc++)<<24 | (u4)getU1(pc++)<<16 | (u4)getU1(pc++)<<8 | getU1(pc++));
						u4 highbyte = (u4)((u4)getU1(pc++)<<24 | (u4)getU1(pc++)<<16 | (u4)getU1(pc++)<<8 | getU1(pc++));
						if(lowbyte <= windex && windex <= highbyte){
							u4 tableoffset = windex - lowbyte;
							pc+= tableoffset * 4;	// skip 4 byte of previous address(es)
							offset = (u4)((u4)getU1(0)<<24 | (u4)getU1(0)<<16 | (u4)getU1(0)<<8 | (u4)getU1(0));
						}
						pc = startPc + offset;
						DEBUGPRINTLOCALS;
						DEBUGPRINTSTACK;
						DEBUGPRINTHEAP;
					}
	CASE	LOOKUPSWITCH:	DEBUGPRINTLN1("lookupswitch");	// mb, jf
					{
							/*
							ab          lookupswitch
							0  0        padding
							0  0  0 25  defaultByte
							0  0  0  2  npairs
							0  0  0  1  match
							0  0  0 1b  offset
							0  0  0  2  match
							0  0  0 20  offset
							*/
						u2 startPc = --pc;
						u2 relPc = pc - getStartPC();//pcMethodStart;	//calculate relative PC for ByteCode in Method
						u4 key = opStackPop().Int;

						relPc = (u2)((relPc + 4) & 0xfffc); // next pc as multiple of 4 from address of 0xab (lookupswitch)
						pc = relPc +getStartPC();// pcMethodStart;	// set pc to begin of default address
						u4 offset = (u4)((u4)getU1(pc++)<<24 | (u4)getU1(pc++)<<16 | (u4)getU1(pc++)<<8 | getU1(pc++));	// default offset
						u4 matches = (u4)((u4)getU1(pc++)<<24 | (u4)getU1(pc++)<<16 | (u4)getU1(pc++)<<8 | getU1(pc++));
						u4 count;
						for(count=0;count < matches;count++){
							u4 match = (u4)((u4)getU1(pc++)<<24 | (u4)getU1(pc++)<<16 | (u4)getU1(pc++)<<8 | getU1(pc++));
							u4 tmpOffset = (u4)((u4)getU1(pc++)<<24 | (u4)getU1(pc++)<<16 | (u4)getU1(pc++)<<8 | getU1(pc++));
							if (key == match){
								offset = tmpOffset;
								break;
							}
						}
						pc = startPc + offset;
						DEBUGPRINTLOCALS;
						DEBUGPRINTSTACK;
						DEBUGPRINTHEAP;
					}
	CASE	GETSTATIC:  DEBUGPRINTLN1("getstatic ");	//mb jf ... corrected funtion
			methodStackPush(cN);
			fieldName = (u1*)getAddr(	CP(cN,		// utf8						
							getU2(		// name-index
							CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) 	// index to name and type
							+ 1))
							+ 3);		// bytes
			fieldNameLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) + 1))
							+ 1);		// length
			fieldDescr = (u1*)getAddr(	CP(cN,		// utf8						
							getU2(		// descriptor-index
							CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) 	// index to name and type
							+ 3))
							+ 3);		// bytes
			fieldDescrLength = getU2(CP(cN, getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+ 3))+ 3))
							+ 1);		// length
			findClass((u1*)getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 3),
							getU2(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 1));

if (!findFieldByName(fieldName,fieldNameLength,fieldDescr,fieldDescrLength)) exit(-27);
/*
do	{
numFields=findNumFields();
for(i=0; i < numFields; i++){
				if(fieldNameLength == getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 1)){
									if(strncmp((char*)fieldName,(char*) getAddr(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 3),
											getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 1)) == 0){
										break;
									}
								}
							}
if (i<numFields) break;
} while (findSuperClass()); // waht ifnot found??
*/







/*

						j = findNumFields();	
						for(i=0; i < j; i++){
							if(fieldNameLength == getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 1)){
								if(strncmp((char*)fieldName,(char*) getAddr(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 3),
										getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 1)) == 0){
									break;
								}
							}
						}
*/
	// got position in constant pool --> results in position on heap
						DEBUGPRINTLNSTRING(fieldName,fieldNameLength);
						opStackPush(heapGetElement( cs[cN].classInfo.stackObj.pos+fNC+1));
						pc += 2;
						cN = methodStackPop();
						DEBUGPRINTSTACK;
						DEBUGPRINTLOCALS;
						DEBUGPRINTHEAP;
	CASE	PUTSTATIC:  	DEBUGPRINTLN1("putstatic -> stack in static field");	//mb jf
				methodStackPush(cN);
				fieldName = (u1*)getAddr(
							CP(cN,		// utf8
							getU2(		// name-index
							CP(cN,getU2(CP(cN,BYTECODEREF)+3)) 	// index to name and type
							+1))
							+3);		// bytes
				fieldNameLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+1))
							+1);		// length
				fieldDescr = (u1*)getAddr(
							CP(cN,		// utf8					
							getU2(		// descriptor-index
							CP(cN,getU2(CP(cN,BYTECODEREF)+3)) 	// index to name and type
							+3))
							+3);		// bytes
				fieldDescrLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+3))+1); // length
				findClass( (u1*)getAddr(CP(cN,getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1))+3),
							getU2(CP(cN,getU2(CP(cN,  
							getU2(CP(cN,BYTECODEREF)+1))+1))+1));
if (!findFieldByName(fieldName,fieldNameLength,fieldDescr,fieldDescrLength)) exit(-27)
/*
				do	{
					numFields=findNumFields();
					for(i=0; i < numFields; i++)	{
						if(fieldNameLength == getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 1)){
							if(strncmp((char*)fieldName,(char*) getAddr(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 3),
								getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 1)) == 0)	{
										break;	}
							}
									}
					if (i<numFields) break;
					} while (findSuperClass()); // what ifnot found??
*/
				DEBUGPRINTHEAP;
							heapSetElement(opStackPop(), cs[cN].classInfo.stackObj.pos+/*i*/fNC+1);//opStackPop().UInt+i+1);
				pc+=2;
				DEBUGPRINTSTACK;
				DEBUGPRINTHEAP;
				cN=methodStackPop();	// restore cN
/*
	CASE	GETSTATIC:  DEBUGPRINTLN1("getstatic ");	//mb jf ... corrected funtion
						methodStackPush(cN);
						// get name
						fieldName = (u1*)getAddr(
							CP(cN,		// utf8						
							getU2(		// name-index
							CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) 	// index to name and type
							+ 1))
							+ 3);		// bytes
						fieldNameLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) + 1))
							+ 1);		// length
						// get type
						fieldDescr = (u1*)getAddr(
							CP(cN,		// utf8						
							getU2(		// descriptor-index
							CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) 	// index to name and type
							+ 3))
							+ 3);		// bytes
						fieldDescrLength = getU2(CP(cN, getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+ 3))+ 3))
							+ 1);		// length
						findClass((u1*)getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 3),
							getU2(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 1));
						j = findNumFields();	
						for(i=0; i < j; i++){
							if(fieldNameLength == getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 1)){
								if(strncmp((char*)fieldName,(char*) getAddr(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 3),
										getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 1)) == 0){
									break;
								}
							}
						}	// got position in constant pool --> results in position on heap
						DEBUGPRINTLNSTRING(fieldName,fieldNameLength);
						opStackPush(heapGetElement( cs[cN].classInfo.stackObj.pos+i+1));
						pc += 2;
						cN = methodStackPop();
						DEBUGPRINTSTACK;
						DEBUGPRINTLOCALS;
						DEBUGPRINTHEAP;
		CASE	PUTSTATIC:  	DEBUGPRINTLN1("putstatic -> stack in static field");	//mb jf
						methodStackPush(cN);
						fieldName = (u1*)getAddr(
							CP(cN,		// utf8
							getU2(		// name-index
							CP(cN,getU2(CP(cN,BYTECODEREF)+3)) 	// index to name and type
							+1))
							+3);		// bytes
						fieldNameLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+1))
							+1);		// length
						fieldDescr = (u1*)getAddr(
							CP(cN,		// utf8						
							getU2(		// descriptor-index
							CP(cN,getU2(CP(cN,BYTECODEREF)+3)) 	// index to name and type
							+3))
							+3);		// bytes
						fieldDescrLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+3))+1);	// length
//						DEBUGPRINTLNSTRING(fieldDescriptorName,fieldDescriptorNameLength);
						findClass(
								(u1*)getAddr(CP(cN,getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1))+3),
								getU2(CP(cN,getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1))+1));
						j = findNumFields();
						for(i=0; i < j; i++){
							if(fieldNameLength == getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i]+2)]+1)){
								if(strncmp((char*)fieldName,(char*) getAddr(cs[cN].constant_pool[getU2(cs[cN].field_info[i]+2)]+3), getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i]+2)]+1)) == 0){
									break;
								}
							}
						}	// got position in constant pool --> results in position on heap
   // B    C	D    F    I    J    S    Z	    L <classname>   ArrayType:    [ ComponentType
   DEBUGPRINTHEAP;
							heapSetElement(opStackPop(), cs[cN].classInfo.stackObj.pos+i+1);//opStackPop().UInt+i+1);
						pc+=2;
						DEBUGPRINTSTACK;
						DEBUGPRINTHEAP;
						cN=methodStackPop();	// restore cN
*/
	CASE	GETFIELD:DEBUGPRINTLN1("getfield ->   heap to stack:");
			DEBUGPRINTSTACK;
			methodStackPush(cN);
			first=opStackPop();
			fieldName = (u1*)getAddr( CP(cN,		// utf8						
						getU2(		// name-index
						CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) 	// index to name and type
						+ 1))
						+ 3);		// bytes
			fieldNameLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) + 1))
						+ 1);		// length
			fieldDescr = (u1*)getAddr( CP(cN,		// utf8						
						getU2(		// descriptor-index
						CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) 	// index to name and type
						+ 3))
						+ 3);		// bytes
			fieldDescrLength = getU2(CP(cN, getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+ 3))+ 3))
						+ 1);		// length
/*			findClass((u1*)getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 3),
						getU2(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 1));
*/	
//printf("GETFIELD popRef %x von findClass %x numfields von dieser kl.%x",first,cN,numFields);
// findClass liefert nur die Klasse, in der das field verwendet wird
// nicht die Klasse in der das field definiert wird (kann eine superclass sein)
// jetzt ist es noch besser, im stackobject die classnummer zu ermitteln

			cN=first.stackObj.classNumber;
if (!findFieldByName(fieldName,fieldNameLength,fieldDescr,fieldDescrLength)) exit(-27);
/*
			count=0;
// in: cN , fname fdesc
// out: cN, fNC, fNO

do	{
numFields=findNumFields();
for(i=0; i < numFields; i++){
				if(fieldNameLength == getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 1)){
									if(strncmp((char*)fieldName,(char*) getAddr(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 3),
											getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 1)) == 0){
										break;
									}
								}
							}
if (i<numFields) break; else count+=numFields;
} while (findSuperClass());
// jetzt müsste ich auch abbrechen, wenn ich nichts finde


							// jetzt hab ich alles
							// den Typ
							// die Stelle auf dem heap
//printf("offset %x cN %x numFields%x i %x\n", count,cN,numFields,i);
							DEBUGPRINTLNSTRING(fieldName,fieldNameLength);
//printf("classnumber: %d nummer %d was auf stack %d\n",cN,i,heapGetElement(objRef + i +1).Int);
*/
							opStackPush(( slot)heapGetElement(first.stackObj.pos +fNO/*count+ i*/ +1).Int);//bh2007!!!!!!!
							pc += 2;
							cN = methodStackPop();
							DEBUGPRINTSTACK;
							DEBUGPRINTLOCALS;
							DEBUGPRINTHEAP;
		CASE	PUTFIELD:	DEBUGPRINTLN1("putfield -> stack to heap");
						methodStackPush(cN);
						{
							// mb jf print name
							fieldName = (u1*)getAddr(
								CP(cN,		// utf8
								getU2(		// name-index
								CP(cN,getU2(CP(cN,BYTECODEREF)+3)) 	// index to name and type
								+1))
								+3);		// bytes
							fieldNameLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+1))
								+1);		// length
							// mb jf print type 
							fieldDescr = (u1*)getAddr(
								CP(cN,		// utf8						
								getU2(		// descriptor-index
								CP(cN,getU2(CP(cN,BYTECODEREF)+3)) 	// index to name and type
								+3))
								+3);		// bytes
							fieldDescrLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+3))+1);	// length	
			/*kann weg*/			findClass(
									(u1*)getAddr(CP(cN,getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1))+3),
									getU2(CP(cN,getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1))+1));

first = opStackPop();	//mb jf doesn't work without variable ?!?!
second =opStackPop();


cN=second.stackObj.classNumber;
if (!findFieldByName(fieldName,fieldNameLength,fieldDescr,fieldDescrLength)) exit(-27);
/*
count=0;
u2 numFields;
do	{
numFields=findNumFields();
for(i=0; i < numFields; i++){
				if(fieldNameLength == getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 1)){
									if(strncmp((char*)fieldName,(char*) getAddr(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 3),
											getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 1)) == 0){
										break;
									}
								}
							}
if (i<numFields) break; else count+=numFields;
} while (findSuperClass());
*/
// jetzt müsste cih auch abbrechen, wenn ich nichts finde

//printf("PUTFIELD von refCl %x%x first %x second %x offset %x numFields %x i%x\n", oldcN,cN ,first,second,count,numFields,i);


/*

							u2 numFields = findNumFields();
							for(i=0; i < numFields; i++){
								if(fieldNameLength == getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i]+2)]+1)){
									if(strncmp((char*)fieldName, (char*)getAddr(cs[cN].constant_pool[getU2(cs[cN].field_info[i]+2)]+3), getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i]+2)]+1)) == 0){
										break;
									}
								}
							}
*/
							// jetzt hab ich alles
							// den Typ
							// die Stelle auf dem heap
							
//printf("classnumber: %d nummer %d was von stack %d\n",cN,i,val);
								heapSetElement(first, second.stackObj.pos+/*count+i*/fNO+1);
						}
						pc+=2;
						DEBUGPRINTSTACK;
						DEBUGPRINTLOCALS;
						DEBUGPRINTHEAP;
						cN=methodStackPop();
/*
		CASE	GETFIELD:	DEBUGPRINTLN1("getfield ->   heap to stack:");
							DEBUGPRINTSTACK;
						methodStackPush(cN);
						{u2 objRef=(u2)opStackPop().stackObj.pos;
							u1* fieldName = (u1*)getAddr(
								CP(cN,		// utf8						
								getU2(		// name-index
								CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) 	// index to name and type
								+ 1))
								+ 3);		// bytes
							fieldNameLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) + 1))
								+ 1);		// length
							fieldDescr = (u1*)getAddr(
								CP(cN,		// utf8						
								getU2(		// descriptor-index
								CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) 	// index to name and type
								+ 3))
								+ 3);		// bytes
							fieldDescrLength = getU2(CP(cN, getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+ 3))+ 3))
								+ 1);		// length
							findClass((u1*)getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 3),
								getU2(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 1));
							u2 numFields = findNumFields();	
							for(i=0; i < numFields; i++){
								if(fieldNameLength == getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 1)){
									if(strncmp((char*)fieldName,(char*) getAddr(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 3),
											getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)] + 1)) == 0){
										break;
									}
								}
							}
							// jetzt hab ich alles
							// den Typ
							// die Stelle auf dem heap
							DEBUGPRINTLNSTRING(fieldName,fieldNameLength);

							opStackPush(( slot)heapGetElement(objRef + i +1).Int);//bh2007!!!!!!!
							pc += 2;
							cN = methodStackPop();
							DEBUGPRINTSTACK;
							DEBUGPRINTLOCALS;
							DEBUGPRINTHEAP;
						}
		CASE	PUTFIELD:	DEBUGPRINTLN1("putfield -> stack to heap");
						methodStackPush(cN);
						{
							// mb jf print name
							fieldName = (u1*)getAddr(
								CP(cN,		// utf8
								getU2(		// name-index
								CP(cN,getU2(CP(cN,BYTECODEREF)+3)) 	// index to name and type
								+1))
								+3);		// bytes
							fieldNameLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+1))
								+1);		// length
							// mb jf print type 
							fieldDescr = (u1*)getAddr(
								CP(cN,		// utf8						
								getU2(		// descriptor-index
								CP(cN,getU2(CP(cN,BYTECODEREF)+3)) 	// index to name and type
								+3))
								+3);		// bytes
							fieldDescrLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+3))+1);	// length	
						findClass(
									(u1*)getAddr(CP(cN,getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1))+3),
									getU2(CP(cN,getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1))+1));
							u2 numFields = findNumFields();
							for(i=0; i < numFields; i++){
								if(fieldNameLength == getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i]+2)]+1)){
									if(strncmp((char*)fieldName, (char*)getAddr(cs[cN].constant_pool[getU2(cs[cN].field_info[i]+2)]+3), getU2(cs[cN].constant_pool[getU2(cs[cN].field_info[i]+2)]+1)) == 0){
										break;
									}
								}
							}
							// jetzt hab ich alles
							// den Typ
							// die Stelle auf dem heap
							u4 val = opStackPop().UInt;	//mb jf doesn't work without variable ?!?!
								heapSetElement(( slot)val, opStackPop().stackObj.pos+i+1);
						}
						pc+=2;
						DEBUGPRINTSTACK;
						DEBUGPRINTLOCALS;
						DEBUGPRINTHEAP;
						cN=methodStackPop();
*/
		CASE	INVOKESPECIAL:		
		case	INVOKEVIRTUAL:
		case	INVOKEINTERFACE:	// not tested yet	
#ifdef DEBUG
										if (code == INVOKEVIRTUAL)	DEBUGPRINT1("invokevirtual: ");
										if (code == INVOKEINTERFACE)	DEBUGPRINT1("invokeinterface: ");
 										if (code == INVOKESPECIAL)		DEBUGPRINT1("invoke special: ");
#endif
						methodStackPush(local);
						methodStackPush(cN);
						methodStackPush(mN);
						if (code == INVOKEINTERFACE) pc+=2;
						methodStackPush(pc);
						k=findNumArgs(BYTECODEREF);
						methodStackPush((opStackGetSpPos()-k-1));//(BYTECODEREF)-1));
						// method resolution
						local = opStackGetSpPos()-findNumArgs(BYTECODEREF)-1;// nachdenken ->mhrmals benutzt
						// get cN from.stackObjRef
						//  get method from cN or superclasses
						methodName = (u1*)getAddr(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+1))+3);
						methodNameLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+1))+1);
						DEBUGPRINTLNSTRING(methodName,methodNameLength);
						methodDescr = (u1*)getAddr(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+3))+3);
						methodDescrLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+3))+1);
						DEBUGPRINTLNSTRING(methodDescr,methodDescrLength);		
						className = NULL;
						if((code == INVOKEVIRTUAL)|| (code == INVOKEINTERFACE))			{

//bh2008
if(opStackGetValue(local).stackObj.magic==CPSTRINGMAGIC)	{
								if (!findClass((u1*)"java/lang/String",16))	
									{printf("string calls error\n"); exit(-4);}
}
else

								cN=opStackGetValue(local).stackObj.classNumber;//bh2007 
								className = (u1*)getAddr(cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].this_class)	]+1)]+3);
								classNameLength = getU2(cs[cN].constant_pool[getU2(	cs[cN].constant_pool[getU2(cs[cN].this_class)]+1)]+1);
																}
						else									{ //INVOKESPECIAL
								className = (u1*)getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+1))+3);
								classNameLength = getU2(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+1))+1);
																}
						DEBUGPRINTLNSTRING(className,classNameLength);											
						cN = findMethod(className,classNameLength,methodName,methodNameLength,methodDescr,methodDescrLength);	// out cN,mN
						opStackSetSpPos(opStackGetSpPos()+((getU2(METHODBASE(cN,mN))&ACC_NATIVE)?0:findMaxLocals()));
								if(getU2(METHODBASE(cN,mN))&ACC_SYNCHRONIZED)	{
									if ( HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex==MUTEXNOTBLOCKED)	{
										// mutex is free, I (the thread) have not the mutex and I can get the mutex for the object
										actualThreadCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;
										HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex=MUTEXBLOCKED;	// get the lock
										// I had not the mutex for this object (but perhaps for others), now I have the look
										for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)	
											if (actualThreadCB->hasMutexLockForObject[i].UInt != NULLOBJECT.UInt) continue;else break;
										if (i==MAXLOCKEDTHREADOBJECTS) {printf("too many locks\n"); exit(-1);	}
										// entry for this object in the array of mutexed objects for the thread
										actualThreadCB->lockCount[i]=1;		// count (before 0)
										actualThreadCB->hasMutexLockForObject[i]=opStackGetValue(local);				}
									else	{	// mutex is blocked, is it my mutex ? have I always the lock ?
										for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)	
											if (actualThreadCB->hasMutexLockForObject[i].UInt==opStackGetValue(local).UInt) break;
										if (i==MAXLOCKEDTHREADOBJECTS) { // another thread has the lock
											actualThreadCB->state=THREADMUTEXBLOCKED;	//mutex blocked
											actualThreadCB->isMutexBlockedOrWaitingForObject=opStackGetValue(local);
											// thread sleeps, try it later
											opStackSetSpPos(methodStackPop()+k+1);//(BYTECODEREF)+1); //native!!!
											pc = methodStackPop()-1;	// before invoke
											mN = methodStackPop();
											cN = methodStackPop();
											local = methodStackPop();
											break;						}		// let the scheduler work
										else // yes I have lock
											actualThreadCB->lockCount[i]++;	// count
											}	
									}
// no synchronized,or I have the lock
// jetzt call die methode
							if(getU2(METHODBASE(cN,mN))&ACC_NATIVE)							{
								if (nativeDispatch(cN,mN,local))		goto nativeValueReturn;
								else										goto nativeVoidReturn;			}				
									pc=getStartPC();
									DEBUGPRINTSTACK;
									DEBUGPRINTLOCALS;
									DEBUGPRINTHEAP;
		CASE	INVOKESTATIC:	DEBUGPRINT1("invoke static: ");	// a static method
						methodStackPush(local);
						methodStackPush(cN);
						methodStackPush(mN);
						methodStackPush(pc);
						k=findNumArgs(BYTECODEREF);
						methodStackPush(opStackGetSpPos()-k);//(BYTECODEREF));
						// method resolution
						local = (u2)opStackGetSpPos()-findNumArgs(BYTECODEREF);//bh2007
						className=(u1*)getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+1))+3);
						classNameLength=getU2(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+1))+1);
						methodName=(u1*)getAddr(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+1))+3);
						methodNameLength=getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+1))+1);
						methodDescr = (u1*)getAddr(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+3))+3);
						methodDescrLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+3))+1);
						findMethod(	className,classNameLength,methodName,methodNameLength,
										methodDescr,methodDescrLength);
						DEBUGPRINTLNSTRING(methodName,methodNameLength);
						DEBUGPRINTLNSTRING(className,classNameLength);		
						opStackSetSpPos(opStackGetSpPos()+((getU2(METHODBASE(cN,mN))&ACC_NATIVE)?0:findMaxLocals()));
								if(getU2(METHODBASE(cN,mN))&ACC_SYNCHRONIZED)	{
//printf("invoke sync static %04x %d %d\n",cs[cN].classInfo,cN,mN);

									if ( HEAPOBJECTMARKER(cs[cN].classInfo.stackObj.pos).mutex==MUTEXNOTBLOCKED)	{
									actualThreadCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;
									HEAPOBJECTMARKER(cs[cN].classInfo.stackObj.pos).mutex=MUTEXBLOCKED;	// get the lock
									for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)	
										if (actualThreadCB->hasMutexLockForObject[i].UInt!=NULLOBJECT.UInt)continue;else break;
										if (i==MAXLOCKEDTHREADOBJECTS) {printf("1too many locks\n"); exit(-1);	}
								actualThreadCB->lockCount[i]=1;	// count
								actualThreadCB->hasMutexLockForObject[i]=cs[cN].classInfo;
									}
									else	{	// mutex ==0
									// have I always the lock ?
										for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)	
										if (actualThreadCB->hasMutexLockForObject[i].UInt==cs[cN].classInfo.UInt) break;
										if (i==MAXLOCKEDTHREADOBJECTS) {
										actualThreadCB->state=THREADMUTEXBLOCKED;	//mutex blocked
										actualThreadCB->isMutexBlockedOrWaitingForObject=cs[cN].classInfo;
										// thread sleeps, try it later
										opStackSetSpPos(methodStackPop()+k);//(BYTECODEREF));
										pc = methodStackPop()-1;	// before invoke
										if (code == INVOKEINTERFACE) pc-=2;
										mN = methodStackPop();
										cN = methodStackPop();
										local = methodStackPop();
										break;	// let the scheduler work
											}
										else // yes I have lock
										actualThreadCB->lockCount[i]++;	// count
										}	
									};
// no synchronized,or I have the lock
// jetzt call die methode
						if (getU2(METHODBASE(cN,mN)) & ACC_NATIVE)							{ 
							if (nativeDispatch(cN,mN,local))		goto nativeValueReturn;	
							else								goto nativeVoidReturn;				}			
						pc=getStartPC();	
						DEBUGPRINTSTACK;
						DEBUGPRINTLOCALS;
						DEBUGPRINTHEAP;
						break;
nativeValueReturn:
case	IRETURN:
case	FRETURN:
case	ARETURN:
						switch (code)	{
						case	IRETURN:	DEBUGPRINTLN1("i");	//mb jf
						CASE	FRETURN:	DEBUGPRINTLN1("f");	// mb jf
						CASE	ARETURN:	DEBUGPRINTLN1("a");
										}
						code=IRETURN;
nativeVoidReturn:	DEBUGPRINTLN1("native ");
case	RETURN:	DEBUGPRINTLN1("return");
						if(getU2(METHODBASE(cN,mN))&ACC_SYNCHRONIZED)	{
							// have I always the lock ?
							if(getU2(METHODBASE(cN,mN))&ACC_STATIC)
								first=cs[cN].classInfo;
							else first=opStackGetValue(local);	
						for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)	// must be in
							if ((actualThreadCB->hasMutexLockForObject[i]).UInt==first.UInt) break;
						if (actualThreadCB->lockCount[i]>1)	actualThreadCB->lockCount[i]--; // fertig
						else	{ // last lock
						actualThreadCB->lockCount[i]=0;
						actualThreadCB->hasMutexLockForObject[i]=NULLOBJECT; // give lock free
						actualThreadCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;
						HEAPOBJECTMARKER(first.stackObj.pos).mutex=MUTEXNOTBLOCKED;
						ThreadControlBlock* myTCB=actualThreadCB;
						for (i=1; i < numThreads; i++)	{	// alle blocked for object wecken!
						myTCB=myTCB-> succ;
						if  ((myTCB->isMutexBlockedOrWaitingForObject.UInt==first.UInt)&&
											(myTCB->state==THREADMUTEXBLOCKED))	{
							myTCB->state=THREADNOTBLOCKED; //!!
							myTCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;		}
														}
								}										}
						if (methodStackEmpty())	{
							//printf("empty method stack\n");
							if(strncmp("<clinit>",(char*)findMethodByMethodNumber(),8) == 0){	//mb jf if not <clinit> you're done :-D
							DEBUGPRINTLN1(" from <clinit>");
							DEBUGPRINTSTACK;
							DEBUGPRINTLOCALS;
							DEBUGPRINTHEAP;
							return;					}
						else				{
								DEBUGPRINTSTACK;
								PRINTEXITTHREAD("normally terminated Thread: %d\n",actualThreadCB->tid);
								break;		}}
								first=opStackPop();							//ret val
					opStackSetSpPos(methodStackPop());
					pc = methodStackPop()+2;
					mN = methodStackPop();
					cN = methodStackPop();
					local = methodStackPop();
					DEBUGPRINTSTACK;
					DEBUGPRINTLOCALS;
					DEBUGPRINTHEAP;
					if (code== IRETURN) opStackPush(first);
		CASE	DRETURN:	PRINTSEXIT("dreturn",nry,4);	// mb jf
		CASE	LRETURN:	PRINTSEXIT("lreturn",nry,4);	// mb jf
		CASE	NEW:	pc += 2;
						methodStackPush(cN);
						methodStackPush(mN);
						if (!findClass(getAddr(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+3),	// className 
										getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1)))	// classNameLength
						printf("class not found %d %d",cN,mN);
//printf("NEW find class in Constantpool: %x",cN);
methodStackPush(cN);
fNO=findNumFields();
while (findSuperClass()) fNO+=findNumFields();
// all fields of class and super classes on heap
//printf("count %d %d\n",count,cN);
cN=methodStackPop();
//printf(" Gesamt heap slots %x \n", count);
//count=findNumFields();
						heapPos=getFreeHeapSpace(fNO/*count findNumFields()*/+ 1);// + marker
						first.stackObj.pos=heapPos;
						first.stackObj.magic=OBJECTMAGIC;
						first.stackObj.classNumber=cN;
						//first.stackObj.type=STACKNEWOBJECT;
						DEBUGPRINTLN2("new -> push %x\n",heapPos);	// allocate on heap platz fuer.stackObjektvariablen
						opStackPush(first); // reference to.stackObject on opStack
						HEAPOBJECTMARKER(heapPos).status = HEAPALLOCATEDNEWOBJECT;	//.stackObject
						HEAPOBJECTMARKER(heapPos).magic=OBJECTMAGIC;
						//HEAPOBJECTMARKER(heapPos).classNumber = cN;
						HEAPOBJECTMARKER(heapPos).mutex = MUTEXNOTBLOCKED;
//						HEAPOBJECTMARKER(heapPos).waitSetNumber = NUMWAITSETS;		// NULL
						DEBUGPRINTHEAP;
						j=findNumFields();
						for(i=0; i< fNO; i++)		// check access flag
//							if(getU2( cs[cN].field_info[i]) != ACC_STATIC)	// if not static field, initialize with 0
						heapSetElement((slot)(u4)0, heapPos+i+1); // initialize the heap elements
						// initialize static fields (constants)
						DEBUGPRINTSTACK;
						DEBUGPRINTLOCALS;
						DEBUGPRINTHEAP;
						mN = methodStackPop();
						cN = methodStackPop();
						DEBUGPRINTLNSTRING(getAddr(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+3),	// className 
							getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1));
/*
		CASE	NEW:	pc += 2;
						methodStackPush(cN);
						methodStackPush(mN);
						if (!findClass(getAddr(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+3),	// className 
										getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1)))	// classNameLength
							printf("not found %d %d",cN,mN);
						heapPos=getFreeHeapSpace(findNumFields()+ 1);	// + marker
						first.stackObj.pos=heapPos;
						first.stackObj.magic=OBJECTMAGIC;
						first.stackObj.classNumber=cN;
						//first.stackObj.type=STACKNEWOBJECT;
						DEBUGPRINTLN2("new -> push %x\n",heapPos);	// allocate on heap platz fuer.stackObjektvariablen
						opStackPush(first); // reference to.stackObject on opStack
						HEAPOBJECTMARKER(heapPos).status = HEAPALLOCATEDNEWOBJECT;	//.stackObject
						HEAPOBJECTMARKER(heapPos).magic=OBJECTMAGIC;
						//HEAPOBJECTMARKER(heapPos).classNumber = cN;
						HEAPOBJECTMARKER(heapPos).mutex = MUTEXNOTBLOCKED;
//HEAPOBJECTMARKER(heapPos).length=findNumFields()+1; bloß nicht das macht heap.c
//						HEAPOBJECTMARKER(heapPos).waitSetNumber = NUMWAITSETS;		// NULL
						DEBUGPRINTHEAP;
						j=findNumFields();
						for(i=0; i< j; i++)		// check access flag
//							if(getU2( cs[cN].field_info[i]) != ACC_STATIC)	// if not static field, initialize with 0
						heapSetElement((slot)(u4)0, heapPos+i+1); // initialize the heap elements
						// initialize static fields (constants)
						DEBUGPRINTSTACK;
						DEBUGPRINTLOCALS;
						DEBUGPRINTHEAP;
						mN = methodStackPop();
						cN = methodStackPop();
						DEBUGPRINTLNSTRING(getAddr(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+3),	// className 
							getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1));
*/
		CASE	NEWARRAY:	DEBUGPRINTLN1("newarray");	// mb jf
						DEBUGPRINTSTACK;
						count = (s2)opStackPop().UInt;
						if(count < 0){	NEGATIVEARRAYSIZEEXCEPTION;}
 						if (count > (MAXHEAPOBJECTLENGTH-1)) {
 							raiseExceptionFromIdentifier("java/lang/ArrayIndexOutOfBoundsException", 40);
 						}

						heapPos=getFreeHeapSpace(count+ 1);	// + marker
						first.stackObj.pos=heapPos;
						first.stackObj.magic=OBJECTMAGIC;
						first.stackObj.arrayLength=(u1)count;
						opStackPush(first);
						HEAPOBJECTMARKER(heapPos).status=HEAPALLOCATEDARRAY;
						HEAPOBJECTMARKER(heapPos).magic=OBJECTMAGIC;
						HEAPOBJECTMARKER(heapPos++).mutex = MUTEXNOTBLOCKED;
						switch(byte1){		// array type, init array with 0 on heap
							case T_BOOLEAN:	for(i=0; i<count; i++)	heapSetElement(( slot)(u4)0,heapPos++);
							CASE T_CHAR:	for(i=0; i<count; i++)	heapSetElement(( slot)(u4)0,heapPos++);
							CASE T_FLOAT:	for(i=0; i<count; i++)	heapSetElement(( slot)0.f,heapPos++);
							CASE T_DOUBLE:	// no double
							CASE T_BYTE:	for(i=0; i<count; i++)	heapSetElement(( slot)(u4)0,heapPos++);
							CASE T_SHORT:	for(i=0; i<count; i++)	heapSetElement(( slot)(u4)0,heapPos++);
							CASE T_INT:		for(i=0; i<count; i++)	heapSetElement(( slot)(u4)0,heapPos++);
							CASE T_LONG:		;// no long
						} // switch
						pc++;	// skip type
						DEBUGPRINTSTACK;
						DEBUGPRINTHEAP;
					 //2a b7  0  1 2a 10  6 b5  0  2 2a 10 30 b5  0  3 2a  4 b5  0  4 2a 12  5 b5  0  6 2a  6 bc  a b5  0  7 b1
		CASE	ANEWARRAY:	DEBUGPRINTLN1("anewarray");	// mb jf
						count =(s2) opStackPop().Int;
 						if (count < 0) {
 							raiseExceptionFromIdentifier("java/lang/NegativeArraySizeException", 36);
 						} else if (count > (MAXHEAPOBJECTLENGTH-1)) {
 							raiseExceptionFromIdentifier("java/lang/ArrayIndexOutOfBoundsException", 39);
						} else {
						// resolve type of array / interface /.stackObject from constant pool at index
						// check access flags (public etc.) ... --> VM specs
						heapPos=getFreeHeapSpace(count+ 1);	// + marker
						first.stackObj.pos=heapPos;
						first.stackObj.magic=OBJECTMAGIC;
						//first.stackObj.type=STACKANEWARRAYOBJECT;
						first.stackObj.classNumber=cN;
						first.stackObj.arrayLength=count;
						opStackPush(first);
						HEAPOBJECTMARKER(heapPos).status=HEAPALLOCATEDARRAY;
						HEAPOBJECTMARKER(heapPos++).magic=OBJECTMAGIC;
//						HEAPOBJECTMARKER(heapPos++).classNumber = cN;
						for(i=0; i<count; i++)	heapSetElement(( slot)(u4)0,heapPos++);
						pc += 2;	// skip indexbyte 1 & 2
						DEBUGPRINTSTACK;
						DEBUGPRINTHEAP;
						}
		CASE	ARRAYLENGTH:	DEBUGPRINTLN1("arraylength");	// mb jf
						first=opStackPop();
/*2008 an array is not a string!!
//						u2 ref = opStackPop().UInt;	// get reference to array from stack
//						if(ref == 0)			NULLPOINTEREXCEPTION;
						if (first.stackObj.type==STACKCPSTRING)	{
						oldcN=cN;
						u2 c;
cN=(u1)(first.stackObj.pos >>8);
		c=getU2(CP(cN,getU2(CP(cN,first.stackObj.pos&0x00ff) + 1)));
opStackPush((slot)(u4)c);
cN=oldcN;
}
else*/
						opStackPush((slot)(u4)first.stackObj.arrayLength);
//?????????????
//opStackPoke((slot)(u4)HEAPOBJECTMARKER(opStackPeek().UInt).length);		// push length of array at ref on opStack
						DEBUGPRINTSTACK;
		CASE	MONITORENTER:	first=opStackPop();
						if ( HEAPOBJECTMARKER(first.stackObj.pos).mutex==MUTEXNOTBLOCKED)	{
							HEAPOBJECTMARKER(first.stackObj.pos).mutex=MUTEXBLOCKED;	// get the lock
							for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)	
								if ((actualThreadCB->hasMutexLockForObject[i]).UInt!=NULLOBJECT.UInt)
									continue;
								else break;
							if (i==MAXLOCKEDTHREADOBJECTS) {printf("too many locks\n"); exit(-1);	}
							actualThreadCB->lockCount[i]=1;	// count
							actualThreadCB->hasMutexLockForObject[i]=first;					}
						else	{	// mutex ==0
									// have I always the lock ?
							for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)	
							if (actualThreadCB->hasMutexLockForObject[i].UInt==first.UInt) break;
							if (i==MAXLOCKEDTHREADOBJECTS) {
							actualThreadCB->state=THREADMUTEXBLOCKED;	//mutex blocked
							actualThreadCB->isMutexBlockedOrWaitingForObject=first;
										// thread sleeps, try it later
							opStackSetSpPos(methodStackPop()+findNumArgs(BYTECODEREF)+1);
							pc = pc-1;	// before monitorenter
							break;	// let the scheduler work
														}
							else // yes I have lock
								actualThreadCB->lockCount[i]++;	// count
								}
		CASE	MONITOREXIT:			// have I always the lock ?
						first=opStackPop();
						for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)	// must be in
							if (actualThreadCB->hasMutexLockForObject[i].UInt==first.UInt) break;
						if (actualThreadCB->lockCount[i]>1)	actualThreadCB->lockCount[i]--; // fertig
						else	{
							actualThreadCB->lockCount[i]=0;
							actualThreadCB->hasMutexLockForObject[i]=NULLOBJECT; // give lock free
							HEAPOBJECTMARKER(opStackGetValue(first.stackObj.pos).UInt).mutex=MUTEXNOTBLOCKED;
							ThreadControlBlock* myTCB=actualThreadCB;
							for (i=0; i < numThreads; i++)	{	// alle wecken!
								myTCB=myTCB-> succ;
									if  (myTCB->isMutexBlockedOrWaitingForObject.UInt==opStackGetValue(first.stackObj.pos).UInt)	{
						myTCB->state=THREADNOTBLOCKED; //!!
//						myTCB->isMutexBlockedForObjectOrWaiting=NULLOBJECT.UInt;
}
															}
								}
		CASE	ATHROW:	DEBUGPRINTLN1("athrow");
					handleException();

        CASE    CHECKCAST: DEBUGPRINTLN1("checkcast");
                    u1 class_cp2 = getU1(cs[cN].constant_pool[getU2(0)]);
                    if (class_cp2 != CONSTANT_Class) {
                        printf("invalid class file. Trying to call CHECKCAST for non-class/array/interface type\n");
                        exit(-1);
                    }
                    first = opStackPop();
                    if (first.UInt != ((slot)NULLOBJECT).UInt) {
                        // Do some check here
                    }

					opStackPush(first);

        CASE    INSTANCEOF: DEBUGPRINTLN1("instanceof");
                    u1 class_cp3 = getU1(cs[cN].constant_pool[getU2(0)]);
                    if (class_cp3 != CONSTANT_Class) {
                        printf("invalid class file. Trying to call INSTANCEOF for non-class/array/interface type\n");
                        exit(-1);
                    }
                    first = opStackPop();
                    if (first.UInt != ((slot)NULLOBJECT).UInt) {
                        // Do some check here
                        opStackPush((slot) 1);
                    } else {
                        opStackPush((slot) 0);
                    }

		CASE	WIDE:		DEBUGPRINTLN1("wide (not tested)");	// mb jf
					{
						// not tested because so many locals are hard to implement on purpose  14.12.2006
						u2 nextOp = getU2(0);	// which operation to extend?
						count = getU2(0);

						if(ILOAD <= nextOp && nextOp <= DLOAD){	// if load operation...
							opStackPush(opStackGetValue(local+count));	// embedded op code for load
						}
						if(ISTORE <= nextOp && nextOp <= DSTORE){	// if store operation...
							opStackSetValue(local+count,opStackPop());	// embedded op code for store
								// write into locals (position, value from stack)
						}
						if(nextOp == RET){	// if ret operation...
							// embedded op code for ret
							//not tested because no exceptions implemented yet 14.12.2006
							// the opcode of athrow is required
							u2 addr = opStackGetValue(local+count).UInt;
							pc = addr+getStartPC();//pcMethodStart;	//assumtion: the address is the relative address, absolute address may be required
						}
						if(nextOp == IINC){	// if iinc operation...
							// embedded op code for load
							u2 constB = (u2)(getU1(pc++)<<8 | getU1(pc++));	// constByte - only available with iinc in wide operation
							opStackSetValue((u2)(local + count), // position
								( slot)(u4)(opStackGetValue(local + count).Int	// old value
								+ constB)); // add const
						}
						DEBUGPRINTLOCALS;
						DEBUGPRINTSTACK;
					}
		CASE	MULTIANEWARRAY:	DEBUGPRINTLN1("multianewarray");	// mb jf
					{
				//	exit(-77);
 						u2 index = getU2(0); // index into the constant_pool
 						u1 dim = getU1(0);	// dimensions

						u2 *cnt = (u2 *) malloc(sizeof(u2));
						*cnt = 0;
						opStackPush(createDims(dim, cnt));	// call recursive function to allocate heap for arrays
						DEBUGPRINTSTACK;
						DEBUGPRINTHEAP;
					}
		CASE	GOTO_W:		DEBUGPRINTLN1("goto_w (not tested)");	// mb jf
						{
						// not tested because wide jumps are hard to implement on purpose  14.12.2006
						u4 addr = (u4)((u4)getU1(pc++)<<24 | (u4)getU1(pc++)<<16 | (u4)getU1(pc++)<<8 | getU1(pc++));
						pc = addr +getStartPC();//pcMethodStart; //assumtion: the address is the relative address, absolute address may be required
					}

		CASE	JSR_W:		DEBUGPRINTLN3("jsr_w (not tested)%d %d",byte1, byte2);	// mb jf
					{
						// not tested because no exceptions implemented yet 14.12.2006
						// the opcode of athrow is required
						u4 addr = (u4)((u4)getU1(pc++)<<24 | (u4)getU1(pc++)<<16 | (u4)getU1(pc++)<<8 | getU1(pc++));
						opStackPush(( slot)addr);
						DEBUGPRINTSTACK;
					}
		} // switch
scheduler();
	} while(1);//do		
printf("schluss\n");
}

// not tested yet
slot createDims(u4 dimsLeft, u2 *count){
	if (dimsLeft == 0) {
		return (slot)0;
	}
	u4 heapPos; u2 i; slot first;
	if (*count == 0) {
		*count = (s2) opStackPop().Int;
	}
	if (*count < 0) {
		raiseExceptionFromIdentifier("java/lang/NegativeArraySizeException", 36);
	} else if (*count > (MAXHEAPOBJECTLENGTH-1)) {
		raiseExceptionFromIdentifier("java/lang/ArrayIndexOutOfBoundsException", 39);
	} else {
		heapPos=getFreeHeapSpace(*count + 1); // + marker
		first.stackObj.pos=heapPos;
		first.stackObj.magic=OBJECTMAGIC;
		first.stackObj.classNumber=cN;
		first.stackObj.arrayLength=*count;
		HEAPOBJECTMARKER(heapPos).status=HEAPALLOCATEDARRAY;
		HEAPOBJECTMARKER(heapPos++).magic=OBJECTMAGIC;
		u2 *cnt = (u2 *) malloc(sizeof(u2));
		*cnt = 0;
		for ( i = 0 ; i < *count ; ++i) {
			heapSetElement((slot) createDims(dimsLeft - 1, cnt), heapPos++);
		}
	}
	return first;
}

/*
** Realizes a interpreter-raised ArithmeticException
*/

void handleArithmeticException() {
	raiseExceptionFromIdentifier("java/lang/ArithmeticException", 29);
}

void raiseExceptionFromIdentifier(char identifier[], u1 length) {

	methodStackPush(cN);
	methodStackPush(mN);

	// Create a class of the given type
	if (findClass(identifier, length) == 0) {
		if (length != 15 || !strncmp(identifier, "java/lang/Error", 15)) {
			raiseExceptionFromIdentifier("java/lang/Error", 15);
		} else {
			printf("cannot find and therefore not raise java/lang/Error\n");
			exit(-1);
		}
	}
	
	heapPos = getFreeHeapSpace(findNumFields()+ 1);	// + marker
	first.stackObj.pos=heapPos;
	first.stackObj.magic=OBJECTMAGIC;
	first.stackObj.classNumber=cN;
	opStackPush(first); // reference to.stackObject on opStack

	HEAPOBJECTMARKER(heapPos).status = HEAPALLOCATEDNEWOBJECT;
	HEAPOBJECTMARKER(heapPos).magic = OBJECTMAGIC;
	HEAPOBJECTMARKER(heapPos).mutex = MUTEXNOTBLOCKED;
	j=findNumFields();
	for(i=0; i< j; i++) {
		heapSetElement((slot)(u4)0, heapPos+i+1);
	}
	
	// call of init is missing!

	mN=methodStackPop();
	cN=methodStackPop();

	handleException();
}

void handleException(){
	// this is actually the class we have to catch
	u1 classNumberFromPushedObject=opStackPeek().stackObj.classNumber;

	// number of catches the try block has
	u2 n = getU2(METHODCODEEXCEPTIONBASE(cN,mN));

DEBUGPRINTLN2("trying to catch class number %d", classNumberFromPushedObject);
DEBUGPRINTLN2("%d catch clauses", n);
	u2 i;
	for (i = 0 ; i < n ; ++i) {

		/* checking if catch range is usable */
		if (!(	((pc-getStartPC()-1) >= getU2(METHODCODEEXCEPTIONBASE(cN,mN)+2+8*i))
			&&	((pc-getStartPC()-1) < getU2(METHODCODEEXCEPTIONBASE(cN,mN)+4+8*i)))) {
			DEBUGPRINTLN2("pc: %d", pc-getStartPC()-1);
			DEBUGPRINTLN2("start: %d", getU2(METHODCODEEXCEPTIONBASE(cN,mN)+2+8*i));
			DEBUGPRINTLN2("end: %d", getU2(METHODCODEEXCEPTIONBASE(cN,mN)+4+8*i));
			DEBUGPRINTLN1("not my range");
			continue;
		}

		// checking whether the catch's catched class is in the code exception table
		methodStackPush(cN);
		methodStackPush(mN);
		if (findClass(getAddr(CP(cN, getU2(CP(cN,getU2(METHODCODEEXCEPTIONBASE(cN,mN)+8+8*i))+1))+3),	// className 
				getU2(CP(cN,  getU2(CP(cN,getU2(METHODCODEEXCEPTIONBASE(cN,mN)+8+8*i))+1))+1))	// classNameLength
				== 0) {
			DEBUGPRINTLN2("Exception class not found:  %d\n",cN);
			mN=methodStackPop();
			cN=methodStackPop();
			continue; // class is not in the class table - broken code.
		}

		// Ya well, this is the catched class's number in code exception table
		u1 classNumberInCodeExceptionTable=cN;
		DEBUGPRINTLN2("classNumberInCodeExceptionTable: %d", classNumberInCodeExceptionTable);
		mN=methodStackPop();
		
		cN = classNumberFromPushedObject;

		/* start catching */
		while (cN != 0) {
			DEBUGPRINTLN2("trying to catch %d",cN);
			// Class name we are trying to catch
			DEBUGPRINTLN2("which is %s\n", getAddr(cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].this_class)]+1)]+3));


			if (classNumberInCodeExceptionTable == cN) {
				DEBUGPRINTLN1("catching!");
				cN=methodStackPop();
				pc=getStartPC()+getU2(METHODCODEEXCEPTIONBASE(cN,mN)+6+8*i);
				return;
			}

			if (getU2(cs[cN].super_class) == 0) {
				// sadly, you are not my man. let's try the next one.
				break;
			}

			// location of the super class's utf8_info
			//i = cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].super_class)]+1)];
			findClass(getAddr(cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].super_class)]+1)]+3), getU2(cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].super_class)]+1)]+1));
		}
		cN=methodStackPop();
	}

	// keine catch clause gefunden, also weiter im nächsten stack frame
	if (methodStackEmpty())	{
		DEBUGPRINTLN1("we are thru, this was the top frame");
		cN = classNumberFromPushedObject;
		printf("unhandled %s\n", getAddr(cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].this_class)]+1)]+3));
		exit(-1);
	} else {
		DEBUGPRINTLN1("popping stack frame");
		first=opStackPop();
		opStackSetSpPos(methodStackPop());
		pc = methodStackPop()+2;
		mN = methodStackPop();
		cN = methodStackPop();
		local = methodStackPop();
		opStackPush(first);
		handleException();
	}
}
