#include <iostream>
#include <iomanip>
#include <sstream>
#include "libraries.h"

using namespace std;

extern Stage stage;
extern int cycle;
extern int signExtend16(int);
extern bool usedMEM(unsigned);
extern bool usedWB(unsigned);
extern bool checkMEM();
bool needRs_EX(unsigned, unsigned);
bool needRt_EX(unsigned, unsigned);
bool checkWB_calc();
bool checkWB_mdata();

void Execute(){
	if(stage.IDEX->needToStall){
		stage.EX = new Instruction();
	}
	else{
		stage.EX = stage.IDEX;
		string s;
		stage.EX->output >> s;
		stage.EX->output.str("");
		stage.EX->output.clear();
		stage.EX->output << s;
		stage.Execute();
	}
	stage.EXMEM = stage.EX;
}

void Stage::Execute(){
	haltEX = false;
	if(needRs_EX(EX->opcode, EX->func) && EX->rs != 0){
		if(!usedMEM(EX->rs) && !usedWB(EX->rs)){
			EX->input1 = reg[EX->rs];
		}
		else if(usedMEM(EX->rs)){
			if(checkMEM()){
				EX->input1 = MEM->calc;
				EX->output << " fwd_EX-DM_rs_$" << dec << EX->rs;
			}
		}
		else if(usedWB(EX->rs)){
			if(checkWB_calc()){
				EX->input1 = WB->calc;
				EX->output << " fwd_DM-WB_rs_$" << dec << EX->rs;
			}
			else if(checkWB_mdata()){
				EX->input1 = WB->mdata;
				EX->output << " fwd_DM-WB_rs_$" << dec << EX->rs;
			}
		}
	}
	
	if(needRt_EX(EX->opcode, EX->func) && EX->rt != 0){
		if(!usedMEM(EX->rt) && !usedWB(EX->rt)){
			EX->input2 = reg[EX->rt];
		}
		else if(usedMEM(EX->rt)){
			if(checkMEM()){
				EX->input2 = MEM->calc;
				EX->output << " fwd_EX-DM_rt_$" << dec << EX->rt;
			}
		}
		else if(usedWB(EX->rt)){
			if(checkWB_calc()){
				EX->input2 = WB->calc;
				EX->output << " fwd_DM-WB_rt_$" << dec << EX->rt;
			}
			else if(checkWB_mdata()){
				EX->input2 = WB->mdata;
				EX->output << " fwd_DM-WB_rt_$" << dec << EX->rt;
			}
		}
	}

	switch(EX->opcode){
		//R-format
		case 0x00:
			switch(EX->func){
				case 0x20:	//add					
					EX->calc = EX->input1 + EX->input2;
					NumberOverflow_Check(EX->calc, EX->input1, EX->input2);
					break;
				case 0x21:	//addu					
					EX->calc = EX->input1 + EX->input2;
					break;
				case 0x22:	//sub
					EX->calc = EX->input1 - EX->input2;
					NumberOverflow_Check(EX->calc, EX->input1, 0 - EX->input2);
					break;
				case 0x24:	//and	
					EX->calc = EX->input1 & EX->input2;
					break;
				case 0x25:	//or
					EX->calc = EX->input1 | EX->input2;
					break;
				case 0x26:	//xor
					EX->calc = EX->input1 ^ EX->input2;
					break;
				case 0x27:	//nor
					EX->calc = ~(EX->input1 | EX->input2);
					break;
				case 0x28:	//nand
					EX->calc = ~(EX->input1 & EX->input2);
					break;
				case 0x2A:	//slt
					EX->calc = (EX->input1 < EX->input2) ? 1 : 0;
					break;
				case 0x00:	//sll
					EX->calc = EX->input2 << EX->shamt;
					break;
				case 0x02:	//srl
					EX->calc = (unsigned)EX->input2 >> EX->shamt;
					break;
				case 0x03:	//sra
					EX->calc = EX->input2 >> EX->shamt;
					break;
				case 0x18:{	//mult
					OverwriteHI_LO_Check(true);
					long long ll = (long long)EX->input1 * (long long)EX->input2;
					int tempHI = reg[HI];
					int tempLO = reg[LO];
					reg[HI] = ll >> 32;
					reg[LO] = ll;
					if(tempHI != reg[HI])
						valueSet.insert(HI);
					if(tempLO != reg[LO])
						valueSet.insert(LO);
					break;
				}					
				case 0x19:{	//multu
					OverwriteHI_LO_Check(true);
					unsigned long long ull = (unsigned long long)(unsigned)EX->input1 * (unsigned long long)(unsigned)EX->input2;
					int tempHI = reg[HI];
					int tempLO = reg[LO];
					reg[HI] = ull >> 32;
					reg[LO] = ull;
					if(tempHI != reg[HI])
						valueSet.insert(HI);
					if(tempLO != reg[LO])
						valueSet.insert(LO);
					break;
				}
				case 0x10:	//mfhi
					OverwriteHI_LO_Check(false);
					EX->calc = reg[HI];
					break;
				case 0x12:	//mflo
					OverwriteHI_LO_Check(false);
					EX->calc = reg[LO];
					break;
			}
			break;
		//I-format
		case 0x08:	//addi
			EX->calc = EX->input1 + EX->imme;
			NumberOverflow_Check(EX->calc , EX->input1, EX->imme);	
			break;
		case 0x09:	//addiu
			EX->calc = EX->input1 + EX->imme;
			break;
		case 0x23:	//lw
			EX->imme = signExtend16(EX->imme);
			EX->calc = EX->input1 + EX->imme;
			NumberOverflow_Check(EX->calc , EX->input1, EX->imme);
			break;
		case 0x21:	//lh
			EX->imme = signExtend16(EX->imme);
			EX->calc = EX->input1 + EX->imme;
			NumberOverflow_Check(EX->calc , EX->input1, EX->imme);	
			break;
		case 0x25:	//lhu
			EX->imme = signExtend16(EX->imme);
			EX->calc = EX->input1 + EX->imme;
			NumberOverflow_Check(EX->calc , EX->input1, EX->imme);	
			break;
		case 0x20:	//lb
			EX->imme = signExtend16(EX->imme);
			EX->calc = EX->input1 + EX->imme;
			NumberOverflow_Check(EX->calc , EX->input1, EX->imme);	
			break;
		case 0x24:	//lbu
			EX->imme = signExtend16(EX->imme);
			EX->calc = EX->input1 + EX->imme;
			NumberOverflow_Check(EX->calc , EX->input1, EX->imme);	
			break;
		case 0x2B:	//sw
			EX->imme = signExtend16(EX->imme);
			EX->calc = EX->input1 + EX->imme;
			NumberOverflow_Check(EX->calc , EX->input1, EX->imme);	
			break;
		case 0x29:	//sh
			EX->imme = signExtend16(EX->imme);
			EX->calc = EX->input1 + EX->imme;
			NumberOverflow_Check(EX->calc , EX->input1, EX->imme);	
			break;
		case 0x28:	//sb
			EX->imme = signExtend16(EX->imme);
			EX->calc = EX->input1 + EX->imme;
			NumberOverflow_Check(EX->calc , EX->input1, EX->imme);	
			break;
		case 0x0F:	//lui
			EX->calc = EX->imme << 16;
			break;
		case 0x0C:	//andi
			EX->calc = EX->input1 & EX->imme;
			break;
		case 0x0D:	//ori
			EX->calc = EX->input1 | EX->imme;
			break;
		case 0x0E:	//nori
			EX->calc = ~(EX->input1 | EX->imme);
			break;
		case 0x0A:	//slti
			EX->calc = (EX->input1 < EX->imme) ? 1 : 0;
			break;
		//S-format
		case 0x3F:	//halt
			haltEX = true;
			break;
	}
}

bool needRs_EX(unsigned opcode, unsigned func){
	switch(opcode){
		case 0x00:
			switch(func){
				case 0x00:
				case 0x02:
				case 0x03:
				case 0x08:
				case 0x10:
				case 0x12:
					return false;
				break;
				default:
					return true;
				break;
			}
		break;
		case 0x0F:
		case 0x04:
		case 0x05:
		case 0x07:
		case 0x02:
		case 0x03:
		case 0x3F:
			return false;
		break;
		default:
			return true;
		break;
	}
}

bool needRt_EX(unsigned opcode, unsigned func){
	switch(opcode){
		case 0x00:
			switch(func){
				case 0x08:
				case 0x10:
				case 0x12:
					return false;
				break;
				default:
					return true;
				break;
			}
		break;
		case 0x2B:
		case 0x29:
		case 0x28:
			return true;
		break;
		default:
			return false;
		break;
	}
}

bool checkWB_calc(){
	switch(stage.WB->opcode){
		case 0x00:
			switch(stage.WB->func){
				case 0x00:
					if(stage.WB->rt != 0 || stage.WB->rd != 0 || stage.WB->shamt != 0){
						return true;
					}
					else{
						return false;
					}
				break;
				case 0x08:
				case 0x18:
				case 0x19:
					return false;
				break;
				default:
					return true;
				break;
			}
		break;
		case 0x08:
		case 0x09:
		case 0x0F:
		case 0x0C:
		case 0x0D:
		case 0x0E:
		case 0x0A:
		case 0x03:
			return true;
		break;
		default:
			return false;
		break;
	}
}

bool checkWB_mdata(){
	switch(stage.WB->opcode){
		case 0x23:
		case 0x21:
		case 0x25:
		case 0x20:
		case 0x24:
			return true;
		break;
		default:
			return false;
		break;
	}
}