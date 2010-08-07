/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
/* fuer lehrzwecke,...*/
/* 14.8.06 sie lief das erste mal durch (in Unix)*/
/* april 2007-> threads, scheduling, funtionsüberladung, native-dispatch*/
/* mai 2007 -> garbage collection, exception handling*/
/* jan08 synchronized, wait notify*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "typedefinitions.h"
#include "classfile.h"
#include "definitions.h"
#include "bajvm.h"
#include "stack.h"
#include "heap.h"
#include "interpreter.h"
#include "scheduler.h"

#ifdef AVR8

#include <avr/pgmspace.h>

#define GETSTARTPC(offset)	((strncmpRamFlash(	"Code",\
			getAddr( cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+8+offset)]+3	),\
			4)==0)? (u2)METHODBASE(cN,mN)+8+14+offset :\
			GETSTARTPC(offset+getU4(METHODBASE(cN,mN)+8)+6))
#else
#define GETSTARTPC(offset)	((strncmp(	"Code",\
			getAddr( cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+8+offset)]+3	),\
			4)==0)? (u2)METHODBASE(cN,mN)+8+14+offset :\
			GETSTARTPC(offset+getU4(METHODBASE(cN,mN)+8)+6))
#endif
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
static char*	className;
static u2 	classNameLength;
static char* 	name;		/* field or method*/
static u2 	nameLength;
static char* 	descr;		/* field or method*/
static u2	descrLength;
/* static u1	numFields; <-- not used?*/
static u2 	i,j,k;
static s2	count;


void run() {	/* in: classNumber,  methodNumber cN, mN*/
	u1 code, byte1, byte2;
	u2 heapPos;
	pc	= getStartPC();
	do {		/*while(1)*/
		code	= getU1(0);
		byte1	= getU1(pc);
		byte2	= getU1(pc+1);
		DEBUGPRINT("-> ");
		DEBUGPRINTE(currentThreadCB->tid,x);
		DEBUGPRINT(", pc: %x ",pc-getStartPC()-1);
		DEBUGPRINTE(code,2x);
		DEBUGPRINTE(byte1,2x);
		DEBUGPRINTE(byte2,2x\t);
		DEBUGPRINT("sp: %d local: %d\n", opStackGetSpPos(), local);
		DEBUGPRINTSTACK;
		DEBUGPRINTLOCALS;
		DEBUGPRINT("\t\t\t\t");
		switch (code)	{

		CASE	NOP:
			DEBUGPRINTLN("NOP");

		CASE	ACONST_NULL:
			DEBUGPRINTLN("aconst_null -> push\t...,=> NULLOBJECT");
			opStackPush(NULLOBJECT);

		CASE	ICONST_M1:
		case	ICONST_0:
		case 	ICONST_1:
		case	ICONST_2:
		case	ICONST_3:
		case	ICONST_4:
		case	ICONST_5:
			DEBUGPRINTLN("ICONST_%4x -> push\t...,=> %4x",code-ICONST_0,code - ICONST_0);
			opStackPush(( slot)(s4)(code - ICONST_0));

		CASE	LCONST_0:
		case	LCONST_1:
			DNOTSUPPORTED;

		CASE	FCONST_0:
		case	FCONST_1:
		case	FCONST_2:
			DEBUGPRINTLN("FCONST_%d  -> push\t...,=> %f",
			              code - FCONST_0,(f4)(code - FCONST_0));
			opStackPush((slot)(f4)(code - FCONST_0));

		CASE DCONST_0:
		case	DCONST_1:
			DNOTSUPPORTED;

		CASE	BIPUSH:
			DEBUGPRINTLN("BIPUSH  -> push\t...,=> %d",(s1)byte1);
			/* BIPUSH is defined as follows:*/
			/* reads a signed 8 bit constant from byte1,*/
			/* extends it to int signed (32bit)*/
			/* and saves it on stack*/
			opStackPush( (slot) (((getU1(pc) & 0x80) >> 7) * 0xffffff80 | (getU1(0) & 0x7f)) );

		CASE	SIPUSH:
			DEBUGPRINTLN("SIPUSH  -> push\t...,=> %x",(s2)BYTECODEREF);
			opStackPush(( slot)((s4)((s2)getU2(0))));

		CASE	LDC:
			DEBUGPRINT("ldc  push\t...");
			if (getU1(CP(cN,getU1(0))) == CONSTANT_String)	{
				first.stackObj.magic=CPSTRINGMAGIC;
				first.stackObj.classNumber=cN;
				first.stackObj.pos=(u2)(/*((u2)cN <<8)*/+byte1);
				opStackPush(first);
			} else 	opStackPush(( slot)getU4(CP(cN,byte1)+1));	/* int or float const value on stack*/
			DEBUGPRINTLN(",=> x%x",opStackPeek().UInt);

		CASE	LDC_W:
				errorExit(-4, "LDC_W not yet realized\n");				

		CASE	LDC2_W:
			DNOTSUPPORTED;

		CASE	ILOAD:
			DEBUGPRINTLN("ILOAD -> local(%x) -> push\t...,=>",byte1);
			opStackPush(opStackGetValue(local+getU1(0)));	/*mb jf*/

		CASE	LLOAD:
			DNOTSUPPORTED;

		CASE	FLOAD:
			DEBUGPRINTLN("FLOAD -> local(%d: %d) -> push\t...,=>",byte1,getU1(byte1));
			opStackPush(opStackGetValue(local+getU1(0)));

		CASE	DLOAD:
			DNOTSUPPORTED;

		CASE	ALOAD:
			DEBUGPRINTLN("aload -> local(%x) -> push\t,=>",byte1);
			opStackPush(opStackGetValue(local+getU1(0)));	/*mb jf changed getU1() --> getU1(0)*/

		CASE	ILOAD_0:
		case	ILOAD_1:
		case	ILOAD_2:
		case	ILOAD_3:
			DEBUGPRINTLN("ILOAD_%d local -> push\t...,=>",code - ILOAD_0);
			opStackPush(opStackGetValue(local + code - ILOAD_0));

		CASE	FLOAD_0:
		case	FLOAD_1:
		case	FLOAD_2:
		case	FLOAD_3:
			DEBUGPRINTLN("FLOAD_%d local -> push\t...,=>",code - FLOAD_0);
			opStackPush(opStackGetValue(local + code - FLOAD_0));

		CASE	DLOAD_0:
		case	DLOAD_1:
		case	DLOAD_2:
		case	DLOAD_3:
			DNOTSUPPORTED;

		CASE	ALOAD_0:
		case	ALOAD_1:
		case	ALOAD_2:
		case	ALOAD_3:
			DEBUGPRINTLN("aload_%d local -> push\t...,=>",code - ALOAD_0);
			opStackPush(opStackGetValue(local + code - ALOAD_0));

		CASE	IALOAD:
		case	FALOAD:
		case	AALOAD:
		case	BALOAD:
		case	CALOAD:
		case	SALOAD:
			count = (s2)opStackPop().Int;			/*mb jf*/
			first = opStackPop();
#ifdef DEBUG
			switch (code)								{
			case IALOAD: DEBUGPRINTLN("iaload");
			CASE FALOAD: DEBUGPRINTLN("faload");
			CASE AALOAD: DEBUGPRINTLN("aaload");
			CASE BALOAD: DEBUGPRINTLN("baload");
			CASE CALOAD: DEBUGPRINTLN("caload");
			CASE SALOAD: DEBUGPRINTLN("saload");
			}

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("%x, =>"),first.UInt);
#else
	printf("%x, =>",first.UInt);
#endif

#endif
			lengthArray = first.stackObj.arrayLength;
			if (first.UInt == NULLOBJECT.UInt)	{
				NULLPOINTEREXCEPTION;
			} else if (count < 0 || lengthArray < count || count > (MAXHEAPOBJECTLENGTH-1)) {
				ARRAYINDEXOUTOFBOUNDSEXCEPTION;
			} else {
				opStackPush(heapGetElement((u2) first.stackObj.pos + count + 1));
				DEBUGPRINTLN(", %x",opStackPeek().UInt);
			}

		CASE	LALOAD:
		case	DALOAD:
			DNOTSUPPORTED;

		CASE	ISTORE:
		case	FSTORE:
		case	ASTORE:
#ifdef DEBUG
			switch (code)	{
			case	ISTORE: DEBUGPRINTLN("ISTORE  pop -> local(%d)=>,\n",byte1);
			CASE	FSTORE: DEBUGPRINTLN("FSTORE  pop -> local(%d)=>,\n",byte1);
			CASE	ASTORE: DEBUGPRINTLN("ASTORE  pop -> local(%x)=>,\n",byte1);
			}
#endif
			opStackSetValue(local+getU1(0),opStackPop());

		CASE	LSTORE:
		case	DSTORE:
			DNOTSUPPORTED;

		CASE	ISTORE_0:
		case	ISTORE_1:
		case	ISTORE_2:
		case	ISTORE_3:
			DEBUGPRINTLN("ISTORE_%d pop -> local   %d=>,",code-ISTORE_0,opStackPeek().Int);
			opStackSetValue(local + code - ISTORE_0,opStackPop());

		CASE	LSTORE_0:
		case	LSTORE_1:
		case	LSTORE_2:
		case	LSTORE_3:
			DNOTSUPPORTED;

		CASE	FSTORE_0:
		case	FSTORE_1:
		case	FSTORE_2:
		case	FSTORE_3:
			DEBUGPRINTLN("FSTORE_%d pop -> local   =>,",code - FSTORE_0);
			opStackSetValue(local+code - FSTORE_0,opStackPop());

		CASE	DSTORE_0:
		case	DSTORE_1:
		case	DSTORE_2:
		case	DSTORE_3:
			DNOTSUPPORTED;

		CASE	ASTORE_0:
		case	ASTORE_1:
		case	ASTORE_2:
		case	ASTORE_3:
			DEBUGPRINTLN("ASTORE_%d pop -> local  =>,",code-ASTORE_0);
			opStackSetValue(local+code - ASTORE_0,opStackPop());

		CASE	IASTORE:
		case	FASTORE:
		case	AASTORE:
		case	BASTORE:
		case	CASTORE:
		case	SASTORE:
#ifdef DEBUG
			switch (code)		{
			case	IASTORE: DEBUGPRINTLN("iastore stack -> local");	/*mb jf		//int*/
			CASE	FASTORE: DEBUGPRINTLN("fastore");	/*mb jf		//float*/
			CASE	AASTORE: DEBUGPRINTLN("fastore");	/*mb jf		//float*/
			CASE	BASTORE: DEBUGPRINTLN("bastore");	/*mb jf		//byte or boolean*/
			CASE	CASTORE: DEBUGPRINTLN("castore");	/*mb jf		//char*/
			CASE	SASTORE: DEBUGPRINTLN("sastore");	/*mb jf		//short*/
			}
#endif
			second = opStackPop();
			count = (s2)(opStackPop().Int);
			first = opStackPop();
			lengthArray=first.stackObj.arrayLength;
			if (first.UInt==NULLOBJECT.UInt) {
				NULLPOINTEREXCEPTION;
			} else if (count < 0 || lengthArray < count||count > (MAXHEAPOBJECTLENGTH-1)) {
				ARRAYINDEXOUTOFBOUNDSEXCEPTION;
			} else {
				heapSetElement(second,first.stackObj.pos+count+1);
			}

		CASE	LASTORE:
		case	DASTORE:
			DNOTSUPPORTED;

		CASE	POP:
			DEBUGPRINTLN("POP %x",opStackPeek().UInt);
			opStackPop();

		CASE	POP2:
			DEBUGPRINTLN("POP2");
			opStackPop();
			opStackPop();

		CASE	DUP:
			DEBUGPRINTLN("dup");
			opStackPush(opStackPeek());

		CASE	DUP_X1:
			DEBUGPRINTLN("DUP_X1");
			second = opStackPop();
			first = opStackPop();
			opStackPush( second);
			opStackPush(first);
			opStackPush(second);

		CASE	DUP_X2:
			DEBUGPRINTLN("DUP_X2");
			second = opStackPop();
			first = opStackPop();
			third = opStackPop();
			opStackPush( second);
			opStackPush(third);
			opStackPush(first);
			opStackPush(second);

		CASE	DUP2:
			DEBUGPRINTLN("DUP2");
			second = opStackPop();
			first = opStackPop();
			opStackPush(first);
			opStackPush(second);
			opStackPush(first);
			opStackPush(second);

		CASE	DUP2_X1:
			DEBUGPRINTLN("DUP2_X1");
			second = opStackPop();
			first = opStackPop();
			third = opStackPop();
			opStackPush(first);
			opStackPush(second);
			opStackPush(third);
			opStackPush(first);
			opStackPush(second);

		CASE	DUP2_X2:
			DEBUGPRINTLN("DUP2_X2");
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

		CASE	SWAP:
			DEBUGPRINTLN("SWAP");
			second = opStackPop();
			first= opStackPop();
			opStackPush(second);
			opStackPush(first);

		CASE	IADD:
			DEBUGPRINTLN("IADD");
			opStackPoke(( slot)(opStackPop().Int + opStackPeek().Int));

		CASE	LADD:
			DNOTSUPPORTED;

		CASE	FADD:
			DEBUGPRINTLN("FADD");
			opStackPoke(( slot)(opStackPop().Float + opStackPeek().Float));

		CASE	DADD:
			DNOTSUPPORTED;

		CASE	ISUB:
			DEBUGPRINTLN("ISUB");
			first = opStackPop();		/*mb fj changed substraction order*/
			opStackPoke(( slot)(opStackPeek().Int - first.Int));

		CASE	LSUB:
			DNOTSUPPORTED;

		CASE	FSUB:
			DEBUGPRINTLN("Fsub");
			first = opStackPop();		/*mb fj changed substraction order*/
			opStackPoke(( slot)(opStackPeek().Float - first.Float));

		CASE	DSUB:
			DNOTSUPPORTED;

		CASE	IMUL:
			DEBUGPRINTLN("IMUL");
			opStackPoke(( slot)(opStackPop().Int * opStackPeek().Int));

		CASE	LMUL:
			DNOTSUPPORTED;

		CASE	FMUL:
			DEBUGPRINTLN("FMUL");
			opStackPoke(( slot)(opStackPop().Float * opStackPeek().Float));

		CASE	DMUL:
			DNOTSUPPORTED;

		CASE	IDIV:
			DEBUGPRINTLN("IDIV");
			first = opStackPop();		/*mb fj changed dividend order*/
			if (first.Int == 0)	ARITHMETICEXCEPTION;
			else 			opStackPush(( slot)( opStackPop().Int / first.Int));

		CASE	LDIV:
			DNOTSUPPORTED;

		CASE	FDIV:
			DEBUGPRINTLN("FDIV");
			first = opStackPop();		/*mb fj changed dividend order*/
			if (first.Float == 0.0)	ARITHMETICEXCEPTION;
			else			opStackPoke(( slot)(opStackPeek().Float / first.Float));

		CASE	DDIV:
			DNOTSUPPORTED;

		CASE	IREM:
			DEBUGPRINTLN("IREM");
			if (( (first=opStackPop()).Int)==0)	ARITHMETICEXCEPTION;
			else					opStackPoke((slot)(opStackPeek().Int % first.Int));

		CASE	LREM:
			DNOTSUPPORTED;

		CASE	FREM:
			DEBUGPRINTLN("FREM");
			float divisor = opStackPop().Float;
			float dividend = opStackPop().Float;
			int q = dividend/divisor;
			opStackPush((slot)(f4)(dividend - (divisor * q)));

		CASE	DREM:
			DNOTSUPPORTED;

		CASE	INEG:
			DEBUGPRINTLN("INEG");
			opStackPoke(( slot)(-opStackPeek().Int));

		CASE	LNEG:
			DNOTSUPPORTED;

		CASE	FNEG:
			DEBUGPRINTLN("FNEG");
			opStackPoke(( slot)(-opStackPeek().Float));

		CASE	DNEG:
			DNOTSUPPORTED;

		CASE	ISHL:
			DEBUGPRINTLN("ISHL");
			opStackPoke(( slot)(opStackPop().UInt << opStackPeek().UInt));

		CASE	LSHL:
			DNOTSUPPORTED;

		CASE	ISHR:
			DEBUGPRINTLN("ISHR");
			opStackPoke(( slot)(opStackPop().Int >> opStackPeek().Int));

		CASE	LSHR:
			DNOTSUPPORTED;

		CASE	IUSHR:
			DEBUGPRINTLN("IUSHR");
			first=(slot)(opStackPop().Int&0x0000001f);
			second =opStackPop();
			if (second.Int<0) opStackPush((slot)((second.Int >> first.Int)+ (2 << ~first.Int)));
			else opStackPush((slot)(second.Int >> first.Int));
		CASE	LUSHR:
			DNOTSUPPORTED;

		CASE	IAND:
			DEBUGPRINTLN("IAND");
			opStackPoke(( slot)(opStackPop().UInt & opStackPeek().UInt));

		CASE	LAND:
			DNOTSUPPORTED;

		CASE	IOR:
			DEBUGPRINTLN("IOR");
			opStackPoke(( slot)(opStackPop().UInt | opStackPeek().UInt));

		CASE	LOR:
			DNOTSUPPORTED;

		CASE	IXOR:
			DEBUGPRINTLN("IXOR");
			opStackPoke(( slot)(opStackPop().UInt ^ opStackPeek().UInt));

		CASE	LXOR:
			DNOTSUPPORTED;

		CASE	IINC:
			DEBUGPRINTLN("IINC");	/*mb, jf*/
			opStackSetValue((u2)(local + byte1), /* position*/
			                ( slot)((s4)(opStackGetValue(local + byte1).Int	/* old value*/
			                             + (s4)(s1)byte2))); /* add const*/
			pc += 2;	/* to skip the index + const*/

		CASE	I2F:
			DEBUGPRINTLN("I2F");
			opStackPoke(( slot)(f4)opStackPeek().Int);

		CASE	F2I:
			DEBUGPRINTLN("F2I");
			opStackPoke(( slot)(s4)(opStackPeek().Float));

		CASE	I2L:
		case	I2D:
		case	L2I:
		case	L2F:
		case	L2D:
		case	F2L:
		case	F2D:
		case	D2I:
		case	D2L:
		case	D2F:
			DNOTSUPPORTED;

		CASE	I2C:
			DEBUGPRINTLN("I2C");
			opStackPoke((slot)(opStackPeek().UInt & 0x0000ffff));

		CASE	I2B:
			DEBUGPRINTLN("I2B");
			opStackPoke(( slot)(opStackPeek().UInt & 0x000000ff));

		CASE	I2S:
			DEBUGPRINTLN("I2S");
			opStackPoke(( slot)(s4)((s2)opStackPeek().Int ));

		CASE	LCMP:
			DNOTSUPPORTED;

		CASE	FCMPL:
		case	FCMPG:
			DEBUGPRINTLN("fcmpg");
			second = opStackPop();
			first = opStackPop();
			if (first.Float == 0x7fc00000 || second.Float == 0x7fc00000) 	{
				opStackPush((slot)(s4) (code == FCMPG ? 1 : -1));
			}
			else 	if (first.Float > second.Float) {
				opStackPush(( slot)(s4)1);
			}
			else 	if (first.Float == second.Float) { 	/* corrected by al june 08*/
				opStackPush(( slot)(s4)0);
			}
			else {
				opStackPush(( slot)(s4)-1);
			}

		CASE	DCMPL:
		case	DCMPG:
			DNOTSUPPORTED;

		CASE	IFEQ:
			DEBUGPRINTLN("ifeq");		/*mb, jf*/
			if (opStackPop().Int == 0)
				pc+= (s2)((byte1 << 8) | (byte2))-1;
			else	pc += 2;/* to skip the jump-adress*/

		CASE	IFNULL:
			DEBUGPRINTLN("ifnull");
			if (opStackPop().UInt == NULLOBJECT.UInt)	pc+= BYTECODEREF-1;	/* add offset to pc at ifnull-address*/
			else				pc += 2;	/* skip branch bytes*/

		CASE	IFNONNULL:
			DEBUGPRINTLN("ifnonnull");	/* mb jf*/
			if (opStackPop().UInt != NULLOBJECT.UInt)	pc+= BYTECODEREF-1;	/* add offset to pc at ifnull-address*/
			else				pc+= 2;	/* skip branch bytes*/

		CASE	IFNE:
			DEBUGPRINTLN("ifne");		/*mb, jf*/
			if (opStackPop().Int != 0)	pc+= (s2)((byte1 << 8) | (byte2))-1;
			else				pc += 2;	/* to skip the jump-adress*/

		CASE	IFLT:
			DEBUGPRINTLN("iflt");		/*mb, jf*/
			if (opStackPop().Int < 0)	pc+= (s2)((byte1 << 8) | (byte2))-1;
			else				pc += 2;	/* to skip the jump-adress*/

		CASE	IFLE:
			DEBUGPRINTLN("ifle");		/*mb, jf*/
			if (opStackPop().Int <= 0)	pc+= (s2)((byte1 << 8) | (byte2))-1;
			else				pc += 2;	/* to skip the jump-adress*/

		CASE	IFGT:
			DEBUGPRINTLN("ifgt");		/*mb, jf*/
			if (opStackPop().Int > 0)	pc+= (s2)((byte1 << 8) | (byte2))-1;
			else				pc += 2;	/* to skip the jump-adress*/

		CASE	IFGE:
			DEBUGPRINTLN("ifge");		/*mb, jf*/
			if (opStackPop().Int >= 0)	pc+= (s2)((byte1 << 8) | (byte2))-1;
			else				pc += 2;	/* to skip the jump-adress*/

		CASE	IF_ACMPEQ:
			DEBUGPRINTLN("if_acmpeq");		/*mb, jf*/
		case	IF_ICMPEQ:
			DEBUGPRINTLN("if_icmpeq");		/*mb, jf*/
			if (opStackPop().Int == opStackPop().Int)	pc+= (s2)((u2)((byte1 << 8) | (byte2)))-1;
			else						pc+= 2;	/* to skip the jump-adress*/

		CASE	IF_ACMPNE:
			DEBUGPRINTLN("if_acmpne");		/*mb, jf*/
		case	IF_ICMPNE:
			DEBUGPRINTLN("if_icmpne");		/*mb, jf*/
			if (opStackPop().Int != opStackPop().Int)	pc+= (s2)((u2)((byte1 << 8) | (byte2)))-1;
			else						pc+= 2;	/* to skip the jump-adress*/

		CASE	IF_ICMPLT:
			DEBUGPRINTLN("if_icmplt");		/*mb, jf*/
			if (opStackPop().Int > opStackPop().Int) /*???*/	pc+= (s2)((u2)((byte1 << 8) | (byte2)))-1;
			else						pc+= 2;	/* to skip the jump-adress*/

		CASE	IF_ICMPGE:
			DEBUGPRINTLN("if_icmpge");		/*mb, jf*/
			if (opStackPop().Int <= opStackPop().Int)	pc+= (s2)((u2)((byte1 << 8) | (byte2)))-1;
			else						pc+= 2;	/* to skip the jump-adress*/

		CASE	IF_ICMPGT:
			DEBUGPRINTLN("if_icmpgt");		/*mb, jf*/
			if (opStackPop().Int < opStackPop().Int)		pc+= (s2)((u2)((byte1 << 8) | (byte2)))-1;
			else						pc+= 2;	/* to skip the jump-adress*/

		CASE	IF_ICMPLE:
			DEBUGPRINTLN("if_icmple");		/*mb, jf*/
			if (opStackPop().Int >= opStackPop().Int)	pc+= (s2)((u2)((byte1 << 8) | (byte2)))-1;
			else						pc+= 2;	/* to skip the jump-adress*/

		CASE	GOTO:
			DEBUGPRINTLN("goto");	/* mb, jf*/
			pc+= (s2)BYTECODEREF-1;

		CASE	JSR:
			DEBUGPRINTLN("jsr");	/* mb, jf*/
			opStackPush(( slot)(u4)(pc+2));
			pc += (s2)BYTECODEREF - 1;

		CASE	RET:
			DEBUGPRINTLN("ret");	/* mb, jf*/
			pc = opStackGetValue(local+getU1(0)).UInt;

		CASE	TABLESWITCH:
			DEBUGPRINTLN("tableswitch");	/* mb, jf*/
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
				u2 relPc = pc - getStartPC();/*pcMethodStart;	//calculate relative PC for ByteCode in Method*/
				u4 windex = opStackPop().Int;
				relPc = (u2)((relPc + 4) & 0xfffc);	/* next pc as multiple of 4 --> skip padding bytes*/
				pc = relPc + getStartPC();/*pcMethodStart;	// set pc to begin of default address*/
				u4 offset = getU4(0);//(u4)((u4)getU1(pc++)<<24 | (u4)getU1(pc++)<<16 | (u4)getU1(pc++)<<8 | getU1(pc++));	// default offset
				u4 lowbyte = getU4(0);//(u4)((u4)getU1(pc++)<<24 | (u4)getU1(pc++)<<16 | (u4)getU1(pc++)<<8 | getU1(pc++));
				u4 highbyte = getU4(0);//(u4)((u4)getU1(pc++)<<24 | (u4)getU1(pc++)<<16 | (u4)getU1(pc++)<<8 | getU1(pc++));
				if (lowbyte <= windex && windex <= highbyte) {
					u4 tableoffset = windex - lowbyte;
					pc+= tableoffset * 4;	/* skip 4 byte of previous address(es)*/
					offset = getU4(0);//(u4)((u4)getU1(0)<<24 | (u4)getU1(0)<<16 | (u4)getU1(0)<<8 | (u4)getU1(0));
				}
				pc = startPc + offset;

			}

		CASE	LOOKUPSWITCH:
			DEBUGPRINTLN("lookupswitch");	/* mb, jf*/
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
				u2 relPc = pc - getStartPC();/*pcMethodStart;	//calculate relative PC for ByteCode in Method*/
				u4 key = opStackPop().Int;

				relPc = (u2)((relPc + 4) & 0xfffc); /* next pc as multiple of 4 from address of 0xab (lookupswitch)*/
				pc = relPc +getStartPC();/* pcMethodStart;	// set pc to begin of default address*/
				u4 offset = getU4(0);	/* default offset*/
				u4 matches;
				for (matches = getU4(0) ; matches > 0; --matches) {
					u4 match = getU4(0);
					u4 tmpOffset = getU4(0);
					if (key == match) {
						offset = tmpOffset;
						break;
					}
				}
				pc = startPc + offset;

			}

		CASE	GETSTATIC:
			DEBUGPRINTLN("getstatic ");	/*mb jf ... corrected funtion*/
			methodStackPush(cN);
			fieldName = (char*)getAddr(	CP(cN,		/* utf8*/
			                               getU2(		/* name-index*/
			                                   CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) 	/* index to name and type*/
			                                   + 1))
			                            + 3);		/* bytes*/
			fieldNameLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) + 1))
			                        + 1);		/* length*/
			fieldDescr = (char*)getAddr(	CP(cN,		/* utf8*/
			                                getU2(		/* descriptor-index*/
			                                    CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) 	/* index to name and type*/
			                                    + 3))
			                             + 3);		/* bytes*/
			fieldDescrLength = getU2(CP(cN, getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+ 3))+ 3))
			                         + 1);		/* length*/
			if (!
#ifdef AVR8
findClassFlash
#else
findClass
#endif
(getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 3),
			          getU2(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 1)) ) { 
				CLASSNOTFOUNDERR(getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 3),getU2(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 1));
			}

			if (!findFieldByName(fieldName,fieldNameLength,fieldDescr,fieldDescrLength)) {
				FIELDNOTFOUNDERR(fieldName, getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 3));
			}
			/* got position in constant pool --> results in position on heap*/
			DEBUGPRINTLNSTRING(fieldName,fieldNameLength);
			opStackPush(heapGetElement( cs[cN].classInfo.stackObj.pos+fNC+1));
			pc += 2;
			cN = methodStackPop();

		CASE	PUTSTATIC:
			DEBUGPRINTLN("putstatic -> stack in static field");	/*mb jf*/
			methodStackPush(cN);
			fieldName = (char*)getAddr(
			                CP(cN,		/* utf8*/
			                   getU2(		/* name-index*/
			                       CP(cN,getU2(CP(cN,BYTECODEREF)+3)) 	/* index to name and type*/
			                       +1))
			                +3);		/* bytes*/
			fieldNameLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+1))
			                        +1);		/* length*/
			fieldDescr = (char*)getAddr(
			                 CP(cN,		/* utf8*/
			                    getU2(		/* descriptor-index*/
			                        CP(cN,getU2(CP(cN,BYTECODEREF)+3)) 	/* index to name and type*/
			                        +3))
			                 +3);		/* bytes*/
			fieldDescrLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+3))+1); /* length*/
			if (!
#ifdef AVR8
findClassFlash
#else

findClass
#endif
(getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 3),
			          getU2(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 1)) ) {
				CLASSNOTFOUNDERR(getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 3),getU2(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 1));
			}	

			if (!findFieldByName(fieldName,fieldNameLength,fieldDescr,fieldDescrLength)) {
				FIELDNOTFOUNDERR(fieldName, getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 3));
			}

			heapSetElement(opStackPop(), cs[cN].classInfo.stackObj.pos+/*i*/fNC+1);/*opStackPop().UInt+i+1);*/
			pc+=2;

			cN=methodStackPop();	/* restore cN*/

		CASE	GETFIELD:
			DEBUGPRINTLN("getfield ->   heap to stack:");
			methodStackPush(cN);
			first=opStackPop();
			fieldName = (char*)getAddr( CP(cN,		/* utf8*/
			                               getU2(		/* name-index*/
			                                   CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) 	/* index to name and type*/
			                                   + 1))
			                            + 3);		/* bytes*/
			fieldNameLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) + 1))
			                        + 1);		/* length*/
			fieldDescr = (char*)getAddr( CP(cN,		/* utf8*/
			                                getU2(		/* descriptor-index*/
			                                    CP(cN,getU2(CP(cN,BYTECODEREF) + 3)) 	/* index to name and type*/
			                                    + 3))
			                             + 3);		/* bytes*/
			fieldDescrLength = getU2(CP(cN, getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+ 3))+ 3))
			                         + 1);		/* length*/
/*
#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("GETFIELD popRef %x von findClass %x numfields von dieser kl.%x"),first,cN,numFields);
#else
	printf("GETFIELD popRef %x von findClass %x numfields von dieser kl.%x",first,cN,numFields);
#endif
*/
/* findClass liefert nur die Klasse, in der das field verwendet wird*/
/* nicht die Klasse in der das field definiert wird (kann eine superclass sein)*/
/* jetzt ist es noch besser, im stackobject die classnummer zu ermitteln*/

			cN=first.stackObj.classNumber;
			if (!findFieldByName(fieldName,fieldNameLength,fieldDescr,fieldDescrLength)) {
				FIELDNOTFOUNDERR(fieldName, getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 3));
			}
			opStackPush(( slot)heapGetElement(first.stackObj.pos +fNO/*count+ i*/ +1).Int);/*bh2007!!!!!!!*/
			pc += 2;
			cN = methodStackPop();

		CASE	PUTFIELD:
			DEBUGPRINTLN("putfield -> stack to heap");
			methodStackPush(cN);
			{
				/* mb jf print name*/
				fieldName = (char*)getAddr(
				                CP(cN,		/* utf8*/
				                   getU2(		/* name-index*/
				                       CP(cN,getU2(CP(cN,BYTECODEREF)+3)) 	/* index to name and type*/
				                       +1))
				                +3);		/* bytes*/
				fieldNameLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+1))
				                        +1);		/* length*/
				/* mb jf print type*/
				fieldDescr = (char*)getAddr(
				                 CP(cN,		/* utf8*/
				                    getU2(		/* descriptor-index*/
				                        CP(cN,getU2(CP(cN,BYTECODEREF)+3)) 	/* index to name and type*/
				                        +3))
				                 +3);		/* bytes*/
				fieldDescrLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+3))+1);	/* length*/
				/*kann weg*/
				if (!
#ifdef AVR8
findClassFlash(
				    getAddr(CP(cN,getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1))+3),
				    getU2(CP(cN,getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1))+1))
#else
findClass(
				    getAddr(CP(cN,getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1))+3),
				    getU2(CP(cN,getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1))+1))
#endif
) { 
					CLASSNOTFOUNDERR( getAddr(CP(cN,getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1))+3),getU2(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 1));
				}

				first = opStackPop();	/*mb jf doesn't work without variable ?!?!*/
				second =opStackPop();

				if (second.UInt == NULLOBJECT.UInt) {
					pc+=2;
					cN=methodStackPop();
					NULLPOINTEREXCEPTION;
				} else {

					cN=second.stackObj.classNumber;
					if (!findFieldByName(fieldName,fieldNameLength,fieldDescr,fieldDescrLength)) {
						FIELDNOTFOUNDERR(fieldName, getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 3));
					}
					/* jetzt hab ich alles*/
					/* den Typ*/
					/* die Stelle auf dem heap*/

					if (
#ifdef AVR8
strncmpRamFlash( "B",fieldDescr, 1)
#else
strncmp( "B",fieldDescr, 1)
#endif
 == 0) {
						/* Truncate Integer input for Byte output */
						first.Int = first.Int & 0x000000ff;
					}
/*
#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("classnumber: %d nummer %d was von stack %d\n"),cN,i,val);
#else
	printf("classnumber: %d nummer %d was von stack %d\n",cN,i,val);
#endif
*/
					heapSetElement(first, second.stackObj.pos+/*count+i*/fNO+1);
				}
				pc+=2;

				cN=methodStackPop();
			}

		CASE	INVOKESPECIAL:
		case	INVOKEVIRTUAL:
		case	INVOKEINTERFACE:
#ifdef DEBUG
			if (code == INVOKEVIRTUAL)	DEBUGPRINT("invokevirtual: ");
			if (code == INVOKEINTERFACE)	DEBUGPRINT("invokeinterface: ");
			if (code == INVOKESPECIAL)	DEBUGPRINT("invoke special: ");
#endif
			methodStackPush(local);
			methodStackPush(cN);
			methodStackPush(mN);
			if (code == INVOKEINTERFACE) pc+=2;
			methodStackPush(pc);
			k=findNumArgs(BYTECODEREF);
			methodStackPush((opStackGetSpPos()-k-1));/*(BYTECODEREF)-1));*/
			/* method resolution*/
			local = opStackGetSpPos()-findNumArgs(BYTECODEREF)-1;/* nachdenken ->mhrmals benutzt*/
			/* get cN from.stackObjRef*/
			/*  get method from cN or superclasses*/
			methodName = (char*)getAddr(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+1))+3);
			methodNameLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+1))+1);
			DEBUGPRINTLNSTRING(methodName,methodNameLength);
			methodDescr = (char*)getAddr(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+3))+3);
			methodDescrLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+3))+1);
			DEBUGPRINTLNSTRING(methodDescr,methodDescrLength);
			className = NULL;

			if ((code == INVOKEVIRTUAL)|| (code == INVOKEINTERFACE))			{

/*bh2008*/
				if (opStackGetValue(local).stackObj.magic==CPSTRINGMAGIC)	{
					if (!findClass("java/lang/String",16))
					{ 
						CLASSNOTFOUNDERR("java/lang/String",16);
					}
				}
				else
					cN=opStackGetValue(local).stackObj.classNumber;/*bh2007*/
				className = (char*)getAddr(cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].this_class)	]+1)]+3);
				classNameLength = getU2(cs[cN].constant_pool[getU2(	cs[cN].constant_pool[getU2(cs[cN].this_class)]+1)]+1);
			}
			else									{ /*INVOKESPECIAL*/
				className = (char*)getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+1))+3);
				classNameLength = getU2(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+1))+1);
			}
//bh			DEBUGPRINTLNSTRING(className,classNameLength);
			if(!findMethod(className,classNameLength,methodName,methodNameLength,methodDescr,methodDescrLength)) {
				METHODNOTFOUNDERR(methodName, className);
			}
				
			opStackSetSpPos(opStackGetSpPos()+((getU2(METHODBASE(cN,mN))&ACC_NATIVE)?0:findMaxLocals()));
			#ifndef TINYBAJOS_MULTITASKING
			if (getU2(METHODBASE(cN,mN))&ACC_SYNCHRONIZED)	{
				if ( HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex==MUTEXNOTBLOCKED)	{
					/* mutex is free, I (the thread) have not the mutex and I can get the mutex for the object*/
					currentThreadCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;
					HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex=MUTEXBLOCKED;	/* get the lock*/
					/* I had not the mutex for this object (but perhaps for others), now I have the look*/
					for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)
						if (currentThreadCB->hasMutexLockForObject[i].UInt != NULLOBJECT.UInt) continue;
						else break;
					if (i==MAXLOCKEDTHREADOBJECTS) {
							errorExit(-1, "too many locks\n");
						
					}
					/* entry for this object in the array of mutexed objects for the thread*/
					currentThreadCB->lockCount[i]=1;		/* count (before 0)*/
					currentThreadCB->hasMutexLockForObject[i]=opStackGetValue(local);
				}
				else	{	/* mutex is blocked, is it my mutex ? have I always the lock ?*/
					for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)
						if (currentThreadCB->hasMutexLockForObject[i].UInt==opStackGetValue(local).UInt) break;
					if (i==MAXLOCKEDTHREADOBJECTS) { /* another thread has the lock*/
						currentThreadCB->state=THREADMUTEXBLOCKED;	/*mutex blocked*/
						currentThreadCB->isMutexBlockedOrWaitingForObject=opStackGetValue(local);
						/* thread sleeps, try it later*/
						opStackSetSpPos(methodStackPop()+k+1);/*(BYTECODEREF)+1); //native!!!*/
						pc = methodStackPop()-1;	/* before invoke*/
						mN = methodStackPop();
						cN = methodStackPop();
						local = methodStackPop();
						break;
					}		/* let the scheduler work*/
					else /* yes I have lock*/
						currentThreadCB->lockCount[i]++;	/* count*/
				}
			}
			#endif
/* no synchronized,or I have the lock*/
/* now call method*/
			if (getU2(METHODBASE(cN,mN))&ACC_NATIVE)			{
if ((cs[cN].nativeFunction!=NULL)&&(cs[cN].nativeFunction[mN]!=NULL))	{	
				if (cs[cN].nativeFunction[mN]())goto nativeValueReturn;
				else				goto nativeVoidReturn;
			}
			else	{
					errorExit(-3,"native method not found cN: %d mN: %d",cN,mN);
			} 
}
			pc=getStartPC();
		CASE	INVOKESTATIC:
			DEBUGPRINT("invoke static: ");	/* a static method*/
			methodStackPush(local);
			methodStackPush(cN);
			methodStackPush(mN);
			methodStackPush(pc);
			k=findNumArgs(BYTECODEREF);
			methodStackPush(opStackGetSpPos()-k);/*(BYTECODEREF));*/
			/* method resolution*/
			local = (u2)opStackGetSpPos()-findNumArgs(BYTECODEREF);/*bh2007*/
			className=(char*)getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+1))+3);
			classNameLength=getU2(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+1))+1);
			methodName=(char*)getAddr(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+1))+3);
			methodNameLength=getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+1))+1);
			methodDescr = (char*)getAddr(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+3))+3);
			methodDescrLength = getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF)+3))+3))+1);
			if(!findMethod(className,classNameLength,methodName,methodNameLength,methodDescr,methodDescrLength)) {
				METHODNOTFOUNDERR(methodName, className);
			}
			DEBUGPRINTLNSTRING(methodName,methodNameLength);
			DEBUGPRINTLNSTRING(className,classNameLength);
			opStackSetSpPos(opStackGetSpPos()+((getU2(METHODBASE(cN,mN))&ACC_NATIVE)?0:findMaxLocals()));
			#ifndef TINYBAJOS_MULTITASKING
			if (getU2(METHODBASE(cN,mN))&ACC_SYNCHRONIZED)	{
/*
#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("invoke sync static %04x %d %d\n"),cs[cN].classInfo,cN,mN);
#else
	printf("invoke sync static %04x %d %d\n",cs[cN].classInfo,cN,mN);
#endif
*/

				if ( HEAPOBJECTMARKER(cs[cN].classInfo.stackObj.pos).mutex==MUTEXNOTBLOCKED)	{
					currentThreadCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;
					HEAPOBJECTMARKER(cs[cN].classInfo.stackObj.pos).mutex=MUTEXBLOCKED;	/* get the lock*/
					for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)
						if (currentThreadCB->hasMutexLockForObject[i].UInt!=NULLOBJECT.UInt)continue;
						else break;
					if (i==MAXLOCKEDTHREADOBJECTS) {
							errorExit(-1, "too many locks\n");
					}
					currentThreadCB->lockCount[i]=1;	/* count*/
					currentThreadCB->hasMutexLockForObject[i]=cs[cN].classInfo;
				}
				else	{	/* mutex ==0*/
					/* have I always the lock ?*/
					for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)
						if (currentThreadCB->hasMutexLockForObject[i].UInt==cs[cN].classInfo.UInt) break;
					if (i==MAXLOCKEDTHREADOBJECTS) {
						currentThreadCB->state=THREADMUTEXBLOCKED;	/*mutex blocked*/
						currentThreadCB->isMutexBlockedOrWaitingForObject=cs[cN].classInfo;
						/* thread sleeps, try it later*/
						opStackSetSpPos(methodStackPop()+k);/*(BYTECODEREF));*/
						pc = methodStackPop()-1;	/* before invoke*/
						if (code == INVOKEINTERFACE) pc-=2;
						mN = methodStackPop();
						cN = methodStackPop();
						local = methodStackPop();
						break;	/* let the scheduler work*/
					}
					else /* yes I have the lock*/
						currentThreadCB->lockCount[i]++;	/* count*/
				}
			}
			#endif
/* no synchronized,or I have the lock*/
/* now call the method*/
			if (getU2(METHODBASE(cN,mN)) & ACC_NATIVE)		{
			if ((cs[cN].nativeFunction!=NULL)&&(cs[cN].nativeFunction[mN]!=NULL))	{
				if (cs[cN].nativeFunction[mN]())goto nativeValueReturn;
				else				goto nativeVoidReturn;
			}
			else{
					errorExit(-3,"native method not found cN: %d mN: %d",cN,mN);							
			} 
}
			pc=getStartPC();

			break;
nativeValueReturn:
		case	IRETURN:
		case	FRETURN:
		case	ARETURN:
			switch (code)	{
			case	IRETURN: DEBUGPRINT("i");	/*mb jf*/
			CASE	FRETURN: DEBUGPRINT("f");	/* mb jf*/
			CASE	ARETURN: DEBUGPRINT("a");
			}
			code=IRETURN;
nativeVoidReturn:
			DEBUGPRINT("native ");
		case	RETURN:
			DEBUGPRINTLN("return");
			#ifndef TINYBAJOS_MULTITASKING
			if (getU2(METHODBASE(cN,mN))&ACC_SYNCHRONIZED)	{
				/* have I always the lock ?*/
				if (getU2(METHODBASE(cN,mN))&ACC_STATIC)
					first=cs[cN].classInfo;
				else first=opStackGetValue(local);
				for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)	/* must be in*/
					if ((currentThreadCB->hasMutexLockForObject[i]).UInt==first.UInt) break;
				if (currentThreadCB->lockCount[i]>1)	currentThreadCB->lockCount[i]--; /* fertig*/
				else	{ /* last lock*/
					currentThreadCB->lockCount[i]=0;
					currentThreadCB->hasMutexLockForObject[i]=NULLOBJECT; /* give lock free*/
					currentThreadCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;
					HEAPOBJECTMARKER(first.stackObj.pos).mutex=MUTEXNOTBLOCKED;
					ThreadControlBlock* myTCB=currentThreadCB;

					u1 k,max;
										
					for(i=0;i<(MAXPRIORITY);i++){
						max=(threadPriorities[i].count);
						myTCB=threadPriorities[i].cb;
						for(k=0;k<max;k++){
							if  ((myTCB->isMutexBlockedOrWaitingForObject.UInt==first.UInt)&&
								(myTCB->state==THREADMUTEXBLOCKED))	{
								myTCB->state=THREADNOTBLOCKED; /*!!*/
								myTCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;
							}							
							myTCB=myTCB->succ;
						}	
					}				
				}
			}
			#endif
			if (methodStackEmpty())	{
				/*
#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("empty method stack\n"));
#else
	printf("empty method stack\n");
#endif
*/
				if (
#ifdef AVR8
strncmpRamFlash
#else
strncmp
#endif
("<clinit>",(char*)findMethodByMethodNumber(),8) == 0) {	/*mb jf if not <clinit> you're done :-D*/
					DEBUGPRINTLN(" from <clinit>");

					return;
				}
				else				{
					#ifndef TINYBAJOS_MULTITASKING
						PRINTEXITTHREAD("normally terminated Thread: %d\n",currentThreadCB->tid);
					#else
						free(methodStackBase);
						free(opStackBase);
					#endif
					break;
				}
			}
			first=opStackPop();							/*ret val*/
			opStackSetSpPos(methodStackPop());
			pc = methodStackPop()+2;
			mN = methodStackPop();
			cN = methodStackPop();
			local = methodStackPop();
			if (code== IRETURN) opStackPush(first);
		CASE	DRETURN:
		case	LRETURN:
			DNOTSUPPORTED;

		CASE	NEW:
			DEBUGPRINT("new");
			pc += 2;
			methodStackPush(cN);
			methodStackPush(mN);
			if (!
#ifdef AVR8
findClassFlash
#else
findClass
#endif
(getAddr(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+3),	/* className*/
			               getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1)) )	/* classNameLength*/
			{
				mN = methodStackPop();
				cN = methodStackPop();
				CLASSNOTFOUNDERR((char *) getAddr(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+3),getU2(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 1));
			}
			methodStackPush(cN);
			fNO= getU2(cs[cN].fields_count);
			while (findSuperClass()) fNO+=getU2(cs[cN].fields_count);
			cN=methodStackPop();
			heapPos=getFreeHeapSpace(fNO/*count getU2(cs[cN].fields_count)*/+ 1);/* + marker*/
			first.stackObj.pos=heapPos;
			first.stackObj.magic=OBJECTMAGIC;
			first.stackObj.classNumber=cN;
			/*first.stackObj.type=STACKNEWOBJECT;*/
			DEBUGPRINTLN(" -> push %x\n",heapPos);	/* allocate on heap platz fuer.stackObjektvariablen*/
			opStackPush(first); /* reference to.stackObject on opStack*/
			HEAPOBJECTMARKER(heapPos).status = HEAPALLOCATEDNEWOBJECT;	/*.stackObject*/
			HEAPOBJECTMARKER(heapPos).magic=OBJECTMAGIC;
			/*HEAPOBJECTMARKER(heapPos).classNumber = cN;*/
			HEAPOBJECTMARKER(heapPos).mutex = MUTEXNOTBLOCKED;
/*						HEAPOBJECTMARKER(heapPos).waitSetNumber = NUMWAITSETS;		// NULL*/

			j=getU2(cs[cN].fields_count);
			for (i=0; i< fNO; i++)		/* check access flag*/
/*							if(getU2( cs[cN].field_info[i]) != ACC_STATIC)	// if not static field, initialize with 0*/
				heapSetElement((slot)(u4)0, heapPos+i+1); /* initialize the heap elements*/
			/* initialize static fields (constants)*/

			mN = methodStackPop();
			cN = methodStackPop();
			DEBUGPRINTLNSTRING(getAddr(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+3),	/* className*/
			                   getU2(CP(cN,  getU2(CP(cN,BYTECODEREF)+1))+1));

		CASE	NEWARRAY:
			DEBUGPRINTLN("newarray");	/* mb jf*/
			count = (s2)opStackPop().UInt;
			if (count < 0) {
				NEGATIVEARRAYSIZEEXCEPTION;
			}
			if (count > (MAXHEAPOBJECTLENGTH-1)) {
				ARRAYINDEXOUTOFBOUNDSEXCEPTION;
			}

			heapPos=getFreeHeapSpace(count+ 1);	/* + marker*/
			first.stackObj.pos=heapPos;
			first.stackObj.magic=OBJECTMAGIC;
			first.stackObj.arrayLength=(u1)count;
			opStackPush(first);
			HEAPOBJECTMARKER(heapPos).status=HEAPALLOCATEDARRAY;
			HEAPOBJECTMARKER(heapPos).magic=OBJECTMAGIC;
			HEAPOBJECTMARKER(heapPos++).mutex = MUTEXNOTBLOCKED;
			switch (byte1) {		/* array type, init array with 0 on heap*/
			case T_BOOLEAN:
				for (i=0; i<count; i++)	heapSetElement(( slot)(u4)0,heapPos++);

			CASE T_CHAR:
				for (i=0; i<count; i++)	heapSetElement(( slot)(u4)0,heapPos++);

			CASE T_FLOAT:
				for (i=0; i<count; i++)	heapSetElement(( slot)0.f,heapPos++);

			CASE T_DOUBLE:
				DNOTSUPPORTED;

			CASE T_BYTE:
				for (i=0; i<count; i++)	heapSetElement(( slot)(u4)0,heapPos++);

			CASE T_SHORT:
				for (i=0; i<count; i++)	heapSetElement(( slot)(u4)0,heapPos++);

			CASE T_INT:
				for (i=0; i<count; i++)	heapSetElement(( slot)(u4)0,heapPos++);

			CASE T_LONG:
				DNOTSUPPORTED;
			} /* switch*/
			pc++;	/* skip type*/

		CASE	ANEWARRAY:
			DEBUGPRINTLN("anewarray");	/* mb jf*/
			pc+=2; /* index into the constant_pool. Bajos performs no verification*/
			s2 *cnt = (s2 *) malloc(sizeof(s2));
			*cnt = 0;
			opStackPush(createDims(1, cnt));	/* call recursive function to allocate heap for arrays*/
			free (cnt);

		CASE	ARRAYLENGTH:
			DEBUGPRINTLN("arraylength");	/* mb jf*/
			first = opStackPop();
			if (first.UInt == NULLOBJECT.UInt) {
				NULLPOINTEREXCEPTION;
			} else {
				opStackPush((slot) (u4)first.stackObj.arrayLength);
			}

		CASE	MONITORENTER:
		#ifndef TINYBAJOS_MULTITASKING
			first=opStackPop();
			if ( HEAPOBJECTMARKER(first.stackObj.pos).mutex==MUTEXNOTBLOCKED)	{
				HEAPOBJECTMARKER(first.stackObj.pos).mutex=MUTEXBLOCKED;	/* get the lock*/
				for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)
					if ((currentThreadCB->hasMutexLockForObject[i]).UInt!=NULLOBJECT.UInt)
						continue;
					else break;
				if (i==MAXLOCKEDTHREADOBJECTS) {
						errorExit(-1, "too many locks\n");
				}
				currentThreadCB->lockCount[i]=1;	/* count*/
				currentThreadCB->hasMutexLockForObject[i]=first;
			}
			else	{	/* mutex ==0*/
				/* have I always the lock ?*/
				for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)
					if (currentThreadCB->hasMutexLockForObject[i].UInt==first.UInt) break;
				if (i==MAXLOCKEDTHREADOBJECTS) {
					currentThreadCB->state=THREADMUTEXBLOCKED;	/*mutex blocked*/
					currentThreadCB->isMutexBlockedOrWaitingForObject=first;
					/* thread sleeps, try it later*/
					opStackSetSpPos(methodStackPop()+findNumArgs(BYTECODEREF)+1);
					pc = pc-1;	/* before monitorenter*/
					break;	/* let the scheduler work*/
				}
				else /* yes I have lock*/
					currentThreadCB->lockCount[i]++;	/* count*/
			}
		#else
			DNOTSUPPORTED;
		#endif
		CASE	MONITOREXIT:			/* have I always the lock ?*/
		#ifndef TINYBAJOS_MULTITASKING
			first=opStackPop();
			for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)	/* must be in*/
				if (currentThreadCB->hasMutexLockForObject[i].UInt==first.UInt) break;
			if (currentThreadCB->lockCount[i]>1)	currentThreadCB->lockCount[i]--; /* fertig*/
			else	{
				currentThreadCB->lockCount[i]=0;
				currentThreadCB->hasMutexLockForObject[i]=NULLOBJECT; /* give lock free*/
				HEAPOBJECTMARKER(opStackGetValue(first.stackObj.pos).UInt).mutex=MUTEXNOTBLOCKED;

				ThreadControlBlock* myTCB;
				u2 max,k;
				for(i=0;i<(MAXPRIORITY);i++){
					max=(threadPriorities[i].count);
					myTCB=threadPriorities[i].cb;
					for(k=0;k<max;k++){
						if  (myTCB->isMutexBlockedOrWaitingForObject.UInt==opStackGetValue(first.stackObj.pos).UInt)	{
							myTCB->state=THREADNOTBLOCKED; /*!!*/
	/*						myTCB->isMutexBlockedForObjectOrWaiting=NULLOBJECT.UInt;*/
						}						
						myTCB=myTCB->succ;
					}	
				}
			}
		#else
			DNOTSUPPORTED;
		#endif
		CASE	ATHROW:
			DEBUGPRINTLN("athrow");
			handleException();

		CASE    CHECKCAST:
			DEBUGPRINTLN("checkcast");
			/* In general, we try to cast as much as possible.*/
			/* Only if we perfectly know that this cast is invalid, break it.*/
			first = opStackPeek();
			char performcheck = 1;
			char invalidcast = 0;
			/* a nullobject can always be casted */
			if (first.UInt != NULLOBJECT.UInt) {
				/* the cast's target class */
				u2 targetclass = getU2(0);
				char *classname = getAddr(CP(cN, getU2(CP(cN,targetclass)+1))+3);
				int len = getU2(CP(cN, getU2(CP(cN,targetclass)+1))+1);

				/* we have to make some dirty hacks here
  				 since we are not storing typing informations for arrays */
				if (
//getU1(classname)=='['
*classname == '['
) {
					while (
//getU1(classname)=='['
'[' == *classname
) {
						/* we hope to get useful information
	 						 from the objects stored in the array.
							 this only takes the first object in the array,
							 yet it could be extended to gathering
							 all stored object's typing informations */
						if (first.stackObj.magic != OBJECTMAGIC || first.UInt == NULLOBJECT.UInt || HEAPOBJECTMARKER(first.stackObj.pos).status != HEAPALLOCATEDARRAY) {
							invalidcast = 1;
							performcheck = 0;
							break;
						}
						first = heapGetElement(first.stackObj.pos + 1);
						/* remove the leading '[' */
						--len;
						++classname;
					}
					if (first.UInt == NULLOBJECT.UInt) {
						performcheck = 0;
					}
					/* A class identifier is Lclassname; */
					if (
//getU1(classname)=='L'
'L' == *classname
) {
						len -= 2;
						++classname;
					} else {
						/* a primitive type */
						performcheck = 0;
					}
				}

				if (performcheck == 1) {
					methodStackPush(cN);
					methodStackPush(mN);
					if (!findClass(classname, len)) { 
						CLASSNOTFOUNDERR(classname,len);
					}
					u2 target = cN;
					cN = first.stackObj.classNumber;
					if (!checkInstance(target)) {
						invalidcast = 1;
					}
					mN=methodStackPop();
					cN=methodStackPop();
				}
			} else {
				pc += 2;
			}
			if (invalidcast == 1) {
				opStackPop();
				CLASSCASTEXCEPTION;
			}	

		CASE    INSTANCEOF:
			DEBUGPRINTLN("instanceof");
			first = opStackPop();
			u2 targetclass = getU2(0);
			performcheck = 1;
			if (first.UInt != NULLOBJECT.UInt) {
				char *classname = getAddr(CP(cN, getU2(CP(cN,targetclass)+1))+3);
				int len = getU2(CP(cN, getU2(CP(cN,targetclass)+1))+1);

				/* we have to make some dirty hacks here
  				 since we are not storing typing informations for arrays */
				if (
//getU1(classname)=='['
*classname == '['
) {
					while (
//getU1(classname)=='['
'[' == *classname
) {
						/* we hope to get useful information
	 						 from the objects stored in the array.
							 this only takes the first object in the array,
							 yet it could be extended to gathering
							 all stored object's typing informations */
						if (first.stackObj.magic != OBJECTMAGIC || first.UInt == NULLOBJECT.UInt || HEAPOBJECTMARKER(first.stackObj.pos).status != HEAPALLOCATEDARRAY) {
							performcheck = 0;
							opStackPush((slot) (u4)0);
							break;
						}
						first = heapGetElement(first.stackObj.pos + 1);
						/* remove the leading '[' */
						--len;
						++classname;
					}
					if (first.UInt == NULLOBJECT.UInt) {
						performcheck = 0;
						opStackPush((slot) (u4)1);
					}
					/* A class identifier is Lclassname; */
					if (
//getU1(classname)=='L'
'L' == *classname
) {
						len -= 2;
						++classname;
					} else {
						/* a primitive type */
						performcheck = 0;
						opStackPush((slot) (u4)1);
					}
				}

				if (performcheck == 1) {
					methodStackPush(cN);
					methodStackPush(mN);
					if (!findClass(classname, len)) { 
						CLASSNOTFOUNDERR(classname,len);
					}
					u2 target = cN;
					cN = first.stackObj.classNumber;
					if (checkInstance(target)) {
						opStackPush((slot) (u4)1);
					} else {
						opStackPush((slot) (u4)0);
					}
					mN=methodStackPop();
					cN=methodStackPop();
				} else {
					opStackPush((slot) (u4)0);
				}
			} else {
				opStackPush((slot) (u4)0);
			}

		CASE	WIDE:
			DEBUGPRINTLN("wide");	/* mb jf*/
			/* not tested because so many locals are hard to implement on purpose  14.12.2006*/
			u2 nextOp = getU1(0);	/* which operation to extend?*/
			count = getU2(0);

			if (ILOAD <= nextOp && nextOp <= DLOAD) {	/* if load operation...*/
				opStackPush(opStackGetValue(local+count));	/* embedded op code for load*/
			}
			if (ISTORE <= nextOp && nextOp <= DSTORE) {	/* if store operation...*/
				opStackSetValue(local+count,opStackPop());	/* embedded op code for store*/
				/* write into locals (position, value from stack)*/
			}
			if (nextOp == RET) {	/* if ret operation...*/
				/* embedded op code for ret*/
				/*not tested because no exceptions implemented yet 14.12.2006*/
				/* the opcode of athrow is required*/
				u2 addr = opStackGetValue(local+count).UInt;
				pc = addr+getStartPC();/*pcMethodStart;	//assumtion: the address is the relative address, absolute address may be required*/
			}
			if (nextOp == IINC) {	/* if iinc operation...*/
				/* embedded op code for load*/
				u2 constB = getU2(0);	/* constByte - only available with iinc in wide operation*/
				opStackSetValue((u2)(local + count), /* position*/
				                ( slot)(u4)(opStackGetValue(local + count).Int	/* old value*/
				                            + constB)); /* add const*/
			}

		CASE	MULTIANEWARRAY:
			DEBUGPRINTLN("multianewarray");	/* mb jf*/
			pc+=2; /* index into the constant_pool. Bajos performs no verification*/
			u1 dim = getU1(0);	/* dimensions*/

			s2 *local_cnt = (s2 *) malloc(sizeof(s2));
			*local_cnt = 0;
			opStackPush(createDims(dim, local_cnt));	/* call recursive function to allocate heap for arrays*/
			free (local_cnt);

		CASE	GOTO_W:
			DEBUGPRINTLN("goto_w (not tested)");	/* mb jf*/
			/* not tested because wide jumps are hard to implement on purpose  14.12.2006*/
			u4 addr = getU4(0);
			pc = addr +getStartPC();/*pcMethodStart; //assumtion: the address is the relative address, absolute address may be required*/

		CASE	JSR_W:
			DEBUGPRINTLN("jsr_w (not tested)%d %d",byte1, byte2);	/* mb jf*/
			/* not tested because no exceptions implemented yet 14.12.2006*/
			/* the opcode of athrow is required*/
			u4 my_addr = getU4(0);
			opStackPush((slot)my_addr);
		} /* switch*/
	#ifndef TINYBAJOS_MULTITASKING
		scheduler();
	#endif
	} while (1);/*do*/

	verbosePrintf("Termination\n");

}

/* generalized single comparison of target class with class at addr in cN's constant pool.i*/
/* keeps cN unchanged if target is no super class of cN.*/
/* else cN is the super class of former cN which has target as super class.*/
void subCheck (u2 target, u2 addr) {
	u2 super_class = cs[cN].constant_pool[getU2(cs[cN].constant_pool[addr]+1)];
	methodStackPush(cN);
#ifdef AVR8
	findClassFlash(getAddr(super_class+3), getU2(super_class+1));
#else
	findClass(getAddr(super_class+3), getU2(super_class+1));
#endif
	if (!checkInstance(target)) {
		cN = methodStackPop();
	} else {
		methodStackPop();
	}
}

/* receives object's class via cN and target class as parameter*/
/* returns true / false*/
u1 checkInstance(const u2 target) {
	if (cN != 0 && cN != target) {
		/* trying the super class.*/
		if (getU2(cs[cN].super_class) > 0) {
			subCheck(target, getU2(cs[cN].super_class));
		}
		/* trying the interfaces.*/
		if (cN != 0 && cN != target) {
			u2 n = getU2(cs[cN].interfaces_count);
			while (n-- && cN != target) {
				subCheck(target, getU2(cs[cN].interfaces+n*2));
			}
		}
	}
	return (target == cN);
}

slot createDims(u4 dimsLeft, s2 *dimSize) {
	slot act_array = NULLOBJECT;
	if (dimsLeft == 0) {
		return act_array;
	}
	if (*dimSize == 0) {
		*dimSize = (s2) opStackPop().Int;
	}
	if (*dimSize < 0) {
		NEGATIVEARRAYSIZEEXCEPTION;
	} else if (*dimSize > (MAXHEAPOBJECTLENGTH-1)) {
		ARRAYINDEXOUTOFBOUNDSEXCEPTION;
	} else {
		u2 heapPos = getFreeHeapSpace(*dimSize + 1); /* + marker*/
		act_array.stackObj.pos=heapPos;
		act_array.stackObj.magic=OBJECTMAGIC;
		act_array.stackObj.arrayLength=*dimSize;
		HEAPOBJECTMARKER(heapPos).status=HEAPALLOCATEDARRAY;
		HEAPOBJECTMARKER(heapPos++).magic=OBJECTMAGIC;
		s2 *cnt = (s2 *) malloc(sizeof(s2));
		*cnt = 0;
		int i;
		for ( i = 0 ; i < *dimSize ; ++i) {
			heapSetElement(createDims(dimsLeft - 1, cnt), heapPos++);
		}
		free (cnt);
	}
	return act_array;
}

/*
** Realizes an interpreter-raised Exception
*/
//BH AM not tested
void raiseExceptionFromIdentifier(const char *identifier, const u1 length) {

	methodStackPush(cN);
	methodStackPush(mN);

#ifdef DEBUG
	if (strlen(identifier) != length) {

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("ERROR: Wrong length for %s\n"), identifier);
#else
	printf("ERROR: Wrong length for %s\n", identifier);
#endif

	}
#endif

	/* Create a class of the given type*/
	if (findClass(identifier, length) == 0) { 
		CLASSNOTFOUNDERR(identifier,length);
	}

	u2 heapPos = getFreeHeapSpace(getU2(cs[cN].fields_count) + 1);	/* + marker*/
	first.stackObj.pos=heapPos;
	first.stackObj.magic=OBJECTMAGIC;
	first.stackObj.classNumber=cN;
	opStackPush(first); /* reference to stackObject on opStack*/

	HEAPOBJECTMARKER(heapPos).status = HEAPALLOCATEDNEWOBJECT;
	HEAPOBJECTMARKER(heapPos).magic = OBJECTMAGIC;
	HEAPOBJECTMARKER(heapPos).mutex = MUTEXNOTBLOCKED;
	j=getU2(cs[cN].fields_count);
	for (i=0; i< j; i++) {
		heapSetElement((slot)(u4)0, heapPos+i+1);
	}

	/*	if (!findMethodByName("<init>", 6, "()V", 3)) {
			METHODNOTFOUNDERR("<init>", identifier);
		}

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("running <init> of %s\n"), identifier);
#else
	printf("running <init> of %s\n", identifier);
#endif

		methodStackPush(pc);
		methodStackPush(local);
		run();
		methodStackPop(local);
		methodStackPop(pc);
	*/

	mN=methodStackPop();
	cN=methodStackPop();

	handleException();
}

//BH AM not tested
void handleException() {
	/* this is actually the class we have to catch*/
	u1 classNumberFromPushedObject=opStackPeek().stackObj.classNumber;

	/* number of catches the try block has*/
	u2 n = getU2(METHODCODEEXCEPTIONBASE(cN,mN));

	DEBUGPRINTLN("trying to catch class number %d", classNumberFromPushedObject);
	DEBUGPRINTLN("%d catch clauses", n);
	for (i = 0 ; i < n ; ++i) {
		u2 cur_catch = METHODCODEEXCEPTIONBASE(cN,mN) + 8*i;

		/* checking if catch range is usable */
		if (	pc - getStartPC() - 1 < getU2(cur_catch + 2)
		        ||	pc - getStartPC() - 1 >= getU2(cur_catch + 4)) {
			DEBUGPRINTLN("pc: %d", pc - getStartPC() - 1);
			DEBUGPRINTLN("start: %d", getU2(cur_catch + 2));
			DEBUGPRINTLN("end: %d", getU2(cur_catch + 4));
			DEBUGPRINTLN("not my range");
			continue;
		}

		/* checking whether the catch's catched class is in the code exception table*/
		methodStackPush(cN);
		if (
#ifdef AVR8
findClassFlash
#else
findClass
#endif
(getAddr(CP(cN, getU2(CP(cN,getU2(cur_catch + 8))+1))+3),	/* className*/
		              getU2(CP(cN,  getU2(CP(cN,getU2(cur_catch + 8))+1))+1))	/* classNameLength*/
		        == 0) {
			DEBUGPRINTLN("Exception class not found:  %d\n",cN);
			cN=methodStackPop();
			continue; /* class is not in the class table - broken code.*/
		}

		/* Ya well, this is the catched class's number in code exception table*/
		u1 classNumberInCodeExceptionTable=cN;
		DEBUGPRINTLN("classNumberInCodeExceptionTable: %d", classNumberInCodeExceptionTable);

		cN = classNumberFromPushedObject;

		/* start catching */
		if (checkInstance(classNumberInCodeExceptionTable)) {
			DEBUGPRINTLN("catching!");
			cN=methodStackPop();
			pc=getStartPC()+getU2(cur_catch + 6);
			return;
		}
		cN=methodStackPop();
	}

	/* keine catch clause gefunden, also weiter im nächsten stack frame*/
	if (methodStackEmpty())	{
		DEBUGPRINTLN("we are thru, this was the top frame");
		cN = classNumberFromPushedObject;
		UNHANDLEDEXCEPTIONERR((char *) getAddr(cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].this_class)]+1)]+3));
	} else {
		DEBUGPRINTLN("popping stack frame");
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
