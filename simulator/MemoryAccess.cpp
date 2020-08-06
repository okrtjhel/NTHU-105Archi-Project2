#include "libraries.h"
#include <sstream>
#include <iostream>

using namespace std;

extern Stage stage;

void MemoryAccess(){
	stage.MEM = stage.EXMEM;
	stage.MemoryAccess();
	string s;
	stage.MEM->output >> s;
	stage.MEM->output.str("");
	stage.MEM->output.clear();
	stage.MEM->output << s;
	stage.MEMWB = stage.MEM;
}

void Stage::MemoryAccess(){
	haltMEM = false;
	switch(MEM->opcode){
		case 0x23:	//lw
			MEM->mdata = loadMemory(MEM->calc, 4);
		break;
		case 0x21:	//lh
			MEM->mdata = loadMemory(MEM->calc, 2);
			MEM->mdata = (MEM->mdata << 16 ) >> 16;
		break;
		case 0x25:	//lhu
			MEM->mdata = loadMemory(MEM->calc, 2);
		break;
		case 0x20:	//lb
			MEM->mdata = loadMemory(MEM->calc, 1);
			MEM->mdata = (MEM->mdata << 24) >> 24;
		break;
		case 0x24:	//lbu
			MEM->mdata = loadMemory(MEM->calc, 1);
		break;
		case 0x2B:	//sw
			saveMemory(MEM->calc, 4, reg[MEM->rt]);
		break;
		case 0x29:	//sh
			saveMemory(MEM->calc, 2, reg[MEM->rt]);
		break;
		case 0x28:	//sb
			saveMemory(MEM->calc, 1, reg[MEM->rt]);
		break;
		case 0x3F: //halt
			haltMEM = true;
		break;
	}
}