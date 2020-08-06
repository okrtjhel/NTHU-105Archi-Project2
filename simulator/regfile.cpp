#include <iostream>
#include <iomanip>
#include <fstream>
#include <ostream>
#include <set>
#include "libraries.h"

using namespace std;

extern int cycle;
extern fstream snapshot;
int reg[35];
set<int> valueSet;

void initReg(){
	for(int i = 0; i < 35; i++){
		reg[i] = 0;
	}
}

void showRegValue(){
	snapshot << "cycle " << dec << cycle << endl;	
	for(set<int>::iterator it = valueSet.begin(); it != valueSet.end(); ++it){
		if(*it <= 31){
			snapshot << "$" << setfill('0') << setw(2) << dec << *it << ": 0x" << setfill('0') << setw(8) << hex << uppercase << reg[*it] << endl;
		}
		if(*it == HI){
			snapshot << "$HI: 0x" << setfill('0') << setw(8) << hex << uppercase << reg[HI] << endl;
		}
		if(*it == LO){
			snapshot << "$LO: 0x" << setfill('0') << setw(8) << hex << uppercase << reg[LO] << endl;
		}
	}
	snapshot << "PC: 0x" << setfill('0') << setw(8) << hex << uppercase << reg[PC] << endl;	
	valueSet.clear();
}

void Stage::showStage(){
	//print IF
	snapshot << "IF: " << IF << endl;

    //print ID
	snapshot << "ID: " << ID << endl;	
    /*
	if(EXMEMtoID_Rs && EXMEMtoID_Rt){
        snapshot << " fwd_EX-DM_rs_$" << ID->rs << " fwd_EX-DM_rt_$" << ID->rt << endl;
        EXMEMtoID_Rs = false;
        EXMEMtoID_Rt = false;
    }
    else if(EXMEMtoID_Rs == true){
        snapshot << " fwd_EX-DM_rs_$" << ID->rs << endl;
        EXMEMtoID_Rs = false;
    }
    else if(EXMEMtoID_Rt == true){
        snapshot << " fwd_EX-DM_rt_$" << ID->rt << endl;
        EXMEMtoID_Rt = false;
    }
    else{
        snapshot << endl;
    }
    */

    //print EX
	snapshot << "EX: " << EX << endl;
    /*	
	if(EXMEMtoEX_Rs == true){
        snapshot << " fwd_EX-DM_rs_$" << EX->rs;
        EXMEMtoEX_Rs = false;
    }
    else if(MEMWBtoEX_Rs == true){
        snapshot << " fwd_DM-WB_rs_$" << EX->rs;
        MEMWBtoEX_Rs = false;
    }
    if(EXMEMtoEX_Rt == true){
        snapshot << " fwd_EX-DM_rt_$" << EX->rt << endl;
        EXMEMtoEX_Rs = false;
    }
    else if(MEMWBtoEX_Rt == true){
        snapshot << " fwd_DM-WB_rt_$" << EX->rt << endl;
        MEMWBtoEX_Rt= false;
    }
    else{
        snapshot << endl;
    }
    */   
	
	//print DM
	snapshot << "DM: " << MEM << endl;
	
	//print WB
	snapshot << "WB: " << WB << endl;

	snapshot << endl << endl;
}

ostream& operator<<(ostream& os, const Instruction *instr){
	os << instr->output.str();
	return os;
}