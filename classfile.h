/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/

u2 readClassFile(char* fileName,u1* addr);

u1 getU1(u2 pos);
u2 getU2(u2 pos);
u4 getU4(u2 pos);
u8 getU8(u2 pos);
f4 getFloat(u2 pos);

void* getAddr(u2 pos);

void analyzeClass(classStructure* c);
void analyseConstantPool(classStructure* c);
void analyseMethods(classStructure* c);
void analyzeFields(classStructure* c);
u1 findClass(char* name,u1 len);
u1 findSuperClass();
u1 findNumArgs(u2 method);
u2 findMaxLocals();	//mb jf
u1 findNumFields();
u1 findFieldByName(const char* fieldName,u1 fieldNameLength, char* fieldDescr,u1 fieldDescrLength);
u1 findMain();
u1 findMethod(char* className, u1 classNameLength,
		char* methodName, u1 methodNameLength,
		char* methodDescr,u1 methodDescrLength);
u1 findMethodByName(	const char* name,u1 len,
			char* methodDescr,u1 methodDescrLength);
u1* findMethodByMethodNumber();	// mb jf

u2	getStartPC();
