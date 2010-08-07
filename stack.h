/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07

#ifndef __STACK__
#define __STACK__
void	opStackInit(slot**);
void	opStackPush(slot val);
slot	opStackPop();
slot	opStackPeek();
void	opStackPoke(slot val);
slot	opStackGetValue(u2 pos);
void	opStackSetValue(u2 pos,slot val);
u2	opStackGetSpPos();
void	opStackSetSpPos(u2 pos);

void	methodStackInit(u2** m);
void	methodStackPush(u2 val);
u2	methodStackPop();
u2 	methodStackPeek();
u1	methodStackEmpty();
u2	methodStackGetSpPos();
void	methodStackSetSpPos(u2 pos);
#endif
