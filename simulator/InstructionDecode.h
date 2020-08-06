#ifndef _INSTRUCTIONDECODE_H
#define _INSTRUCTIONDECODE_H

extern void InstructionDecode();
extern int signExtend16(int);
extern int signExtend26(int);
extern bool needRs(unsigned, unsigned);
extern bool needRt(unsigned, unsigned);
extern bool usedEX(unsigned);
extern bool usedMEM(unsigned);
extern bool usedWB(unsigned);
extern bool checkEX();
extern bool checkMEM();

#endif