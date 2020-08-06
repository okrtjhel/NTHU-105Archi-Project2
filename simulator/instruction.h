#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H

#include <sstream>
#include <ostream>

using namespace std;

class Instruction{
public:
	unsigned int instr, opcode, func, shamt;
	unsigned int rs, rt, rd;
	int input1, input2, imme, addr, calc, mdata;
	bool needNOP, needToStall;
	stringstream output;
	Instruction(){
		instr = 0;
		opcode = 0;
		rs = 0;
		rt = 0;
		rd = 0;
		shamt = 0;
		func = 0;
		imme = 0;
		addr = 0;
		input1 = 0;	//reg[rs]
		input2 = 0;	//reg[rt]
		calc = 0;
		mdata = 0;
		needNOP = false;
		needToStall = false;
		output.str("NOP");
	}
	Instruction(unsigned instruction){
		instr = instruction;
		opcode = instruction >> 26;
		rs = (instruction << 6) >> 27;
		rt = (instruction << 11) >> 27;
		rd = (instruction << 16) >> 27;
		shamt = (instruction << 21) >> 27;
		func = (instruction << 26) >> 26;
		imme = (instruction << 16) >> 16;
		addr = (instruction << 6) >> 6;
		input1 = 0;
		input2 = 0;
		calc = 0;
		mdata = 0;
		needNOP = false;
		needToStall = false;
		output.str("");
	}
	friend ostream& operator<<(ostream& , const Instruction *);
};

class Stage{
public:
	Instruction *IF, *ID, *EX, *MEM, *WB;
	Instruction *IFID, *IDEX, *EXMEM, *MEMWB;
	bool haltIF, haltID, haltEX, haltMEM, haltWB;
	bool needToFlush;
	bool EXMEMtoID_Rs, EXMEMtoID_Rt, EXMEMtoEX_Rs, EXMEMtoEX_Rt, MEMWBtoEX_Rs, MEMWBtoEX_Rt;
	void InstructionDecode();
	void Execute();
	void MemoryAccess();
	void WriteBack();
	void showStage();
	void initial();
};

extern ostream& operator<<(ostream& os, const Instruction *in);

#endif