#include <iostream>
#include <fstream>
#include <iomanip>
#include "libraries.h"

using namespace std;

fstream snapshot("_snapshot.rpt", fstream::out);
Stage stage;
int cycle = 0;
bool shutdown = false;
bool isEnd();

int main(){
	ifstream iin("./iimage.bin", ios::in | ios::binary);
    ifstream din("./dimage.bin", ios::in | ios::binary);
    stage.initial();
    initReg();
    initMemory();
    loadIimage(&iin);
    loadDimage(&din);
    for(int i = 0; i < 34; i++){
		valueSet.insert(i);
	}
	while(!isEnd() && !shutdown){
		showRegValue();
		WriteBack();
		MemoryAccess();
		Execute();
		InstructionDecode();
		InstructionFetch();
		stage.showStage();
		cycle++;
	}
	return 0;
}

void Stage::initial(){
	haltIF = false;
	haltID = false;
	haltEX = false;
	haltMEM = false;
	haltWB = false;
	needToFlush = false;
	EXMEMtoID_Rs = false;
	EXMEMtoID_Rt = false;
	EXMEMtoEX_Rs = false;
	EXMEMtoEX_Rt = false;
	MEMWBtoEX_Rs = false;
	MEMWBtoEX_Rt = false;
	IF = new Instruction();
	ID = new Instruction();
	EX = new Instruction();
	MEM = new Instruction();
	WB = new Instruction();
	IFID = new Instruction();
	IDEX = new Instruction();
	EXMEM = new Instruction();
	MEMWB = new Instruction();
}

bool isEnd(){
	if(stage.haltIF && stage.haltID && stage.haltEX && stage.haltMEM && stage.haltWB){
		return true;
	}
	else{
		return false;
	}
}