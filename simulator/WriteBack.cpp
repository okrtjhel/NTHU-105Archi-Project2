#include "libraries.h"

using namespace std;

extern Stage stage;

void WriteBack(){
	delete stage.WB;
	stage.WB = stage.MEMWB;
	stage.WriteBack();
}

void Stage::WriteBack(){
	haltWB = false;
	switch(WB->opcode){
		case 0x00:
			switch(WB->func){
				case 0x00:	//sll
					if(WB->rt != 0 || WB->rd != 0 || WB->shamt != 0){
						if(WriteToZero_Check(WB->rd) == false){
							if(reg[WB->rd] != WB->calc)
								valueSet.insert(WB->rd);
							reg[WB->rd] = WB->calc;
						}						
					}
				break;
				case 0x08:	//jr
				case 0x18:	//mult
				case 0x19:	//multu
				break;
				default:
				if(WriteToZero_Check(WB->rd) == false){
					if(reg[WB->rd] != WB->calc)
						valueSet.insert(WB->rd);
					reg[WB->rd] = WB->calc;
				}
				break;
			}
		break;
		case 0x08:	//addi
		case 0x09:	//addiu
		case 0x0F:	//lui
		case 0x0C:	//andi
		case 0x0D:	//ori
		case 0x0E:	//nori
		case 0x0A:	//slti
		if(WriteToZero_Check(WB->rt) == false){
			if(reg[WB->rt] != WB->calc)
				valueSet.insert(WB->rt);
			reg[WB->rt] = WB->calc;
		}			
		break;
		case 0x23:	//lw
		case 0x21:	//lh
		case 0x25:	//lhu
		case 0x20:	//lb
		case 0x24:	//lbu
		if(WriteToZero_Check(WB->rt) == false){
			if(reg[WB->rt] != WB->mdata)
				valueSet.insert(WB->rt);
			reg[WB->rt] = WB->mdata;
		}			
		break;
		case 0x03:	//jal
		if(reg[31] != WB->calc)
			valueSet.insert(31);
		reg[31] = WB->calc;
		break;
		case 0x3F:	//halt
			haltWB = true;
			return;
		break;
		default:
		break;
	}
}