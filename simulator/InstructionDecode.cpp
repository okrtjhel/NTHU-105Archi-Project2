#include <iostream>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "libraries.h"

using namespace std;

extern fstream snapshot;
extern Stage stage;
int PC_next;

void InstructionDecode(){
	if(stage.needToFlush){
		PC_next = reg[PC] + 4;
		delete stage.IFID;
		stage.ID = new Instruction();
		stage.needToFlush = false;
	}
	else{
		PC_next = reg[PC] + 4;
		if(stage.ID->needToStall == false){
			stage.ID = stage.IFID;
		}
		stage.ID->needToStall = false;
		stage.InstructionDecode();
		if(stage.ID->needToStall){
			string s;
			stage.ID->output >> s;
			stage.ID->output.str("");
			stage.ID->output.clear();
			stage.ID->output << s << " to_be_stalled";			
		}
	}
	stage.IDEX = stage.ID;
}

void Stage::InstructionDecode(){
	ID->output.str("");
	ID->output.clear();

	haltID = false;

	switch(ID->opcode){
		//R-format
		case 0x00:
			switch(ID->func){
				case 0x20:	//add
					//snapshot << "add " << dec << ID->rd << " " << dec << ID->rs << " " << dec << ID->rt << endl;
					ID->output << "ADD";
					break;
				case 0x21:	//addu
					//snapshot << "addu " << dec << ID->rd << " " << dec << ID->rs << " " << dec << ID->rt << endl;
					ID->output << "ADDU";
					break;
				case 0x22:	//sub
					//snapshot << "sub " << dec << ID->rd << " " << dec << ID->rs << " " << dec << ID->rt << endl;
					ID->output << "SUB";
					break;
				case 0x24:	//and	
					//snapshot << "and " << dec << ID->rd << " " << dec << ID->rs << " " << dec << ID->rt << endl;			
					ID->output << "AND";
					break;
				case 0x25:	//or
					//snapshot << "or " << dec << ID->rd << " " << dec << ID->rs << " " << dec << ID->rt << endl;
					ID->output << "OR";
					break;
				case 0x26:	//xor
					//snapshot << "xor " << dec << ID->rd << " " << dec << ID->rs << " " << dec << ID->rt << endl;
					ID->output << "XOR";
					break;
				case 0x27:	//nor
					//snapshot << "nor " << dec << ID->rd << " " << dec << ID->rs << " " << dec << ID->rt << endl;
					ID->output << "NOR";
					break;
				case 0x28:	//nand
					//snapshot << "nand " << dec << ID->rd << " " << dec << ID->rs << " " << dec << ID->rt << endl;				
					ID->output << "NAND";
					break;
				case 0x2A:	//slt
					//snapshot << "slt " << dec << ID->rd << " " << dec << ID->rs << " " << dec << ID->rt << endl;
					ID->output << "SLT";
					break;
				case 0x00:	//sll
					//snapshot << "sll " << dec << ID->rd << " " << dec << ID->rt << " " << dec << shamt << endl;
					if(ID->rt == 0 && ID->rd == 0 && ID->shamt == 0){
						ID->output << "NOP";
					}
					else{
						ID->output << "SLL";
					}
					break;
				case 0x02:	//srl
					//snapshot << "srl " << dec << ID->rd << " " << dec << ID->rt << " " << dec << shamt << endl;
					ID->output << "SRL";
					break;
				case 0x03:	//sra
					//snapshot << "sra " << dec << ID->rd << " " << dec << ID->rt << " " << dec << shamt << endl;
					ID->output << "SRA";
					break;
				case 0x08:	//jr
					//snapshot << "jr " << dec << ID->rs << endl;
					ID->output << "JR";
					break;
				case 0x18:	//mult
					//snapshot << "mult " << dec << ID->rs << " " << dec << ID->rt << endl;
					ID->output << "MULT";
					break;			
				case 0x19:	//multu
					//snapshot << "multu " << dec << ID->rs << " " << dec << ID->rt << endl;
					ID->output << "MULTU";
					break;
				case 0x10:	//mfhi
					//snapshot << "mfhi " << dec << ID->rd << endl;
					ID->output << "MFHI";
					break;
				case 0x12:	//mflo
					//snapshot << "mflo " << dec << ID->rd << endl;
					ID->output << "MFLO";
					break;
				default:
					cout << "illegal instruction found at 0x" << setfill('0') << setw(8) << hex << uppercase << reg[PC]-4 << endl;
					exit(0);
					break;
			}
			break;
		//I-format
		case 0x08:	//addi
			//snapshot << "addi " << dec << ID->rt << " " << dec << ID->rs << " " << dec << ID->imme << endl;
			ID->imme = signExtend16(ID->imme);
			ID->output << "ADDI";
			break;
		case 0x09:	//addiu
			//snapshot << "addiu " << dec << ID->rt << " " << dec << ID->rs << " " << dec << ID->imme << endl;
			ID->imme = signExtend16(ID->imme);
			ID->output << "ADDIU";
			break;
		case 0x23:	//lw
			//snapshot << "lw " << dec << ID->rt << " " << dec << ID->imme << "(" << dec << ID->rs << ")" << endl;
			ID->imme = signExtend16(ID->imme);
			ID->output << "LW";
			break;
		case 0x21:	//lh
			//snapshot << "lh " << dec << ID->rt << " " << dec << ID->imme << "(" << dec << ID->rs << ")" << endl;
			ID->imme = signExtend16(ID->imme);
			ID->output << "LH";
			break;
		case 0x25:	//lhu
			//snapshot << "lhu " << dec << ID->rt << " " << dec << ID->imme << "(" << dec << ID->rs << ")" << endl;
			ID->imme = signExtend16(ID->imme);
			ID->output << "LHU";
			break;
		case 0x20:	//lb
			//snapshot << "lb " << dec << ID->rt << " " << dec << ID->imme << "(" << dec << ID->rs << ")" << endl;
			ID->imme = signExtend16(ID->imme);
			ID->output << "LB";
			break;
		case 0x24:	//lbu
			//snapshot << "lbu " << dec << ID->rt << " " << dec << ID->imme << "(" << dec << ID->rs << ")" << endl;
			ID->imme = signExtend16(ID->imme);
			ID->output << "LBU";
			break;
		case 0x2B:	//sw
			//snapshot << "sw " << dec << ID->rt << " " << dec << ID->imme << "(" << dec << ID->rs << ")" << endl;
			ID->imme = signExtend16(ID->imme);
			ID->output << "SW";
			break;
		case 0x29:	//sh
			//snapshot << "sh " << dec << ID->rt << " " << dec << ID->imme << "(" << dec << ID->rs << ")" << endl;
			ID->imme = signExtend16(ID->imme);
			ID->output << "SH";
			break;
		case 0x28:	//sb
			//snapshot << "sb " << dec << ID->rt << " " << dec << ID->imme << "(" << dec << ID->rs << ")" << endl;
			ID->imme = signExtend16(ID->imme);
			ID->output << "SB";
			break;
		case 0x0F:	//lui
			//snapshot << "lui " << dec << ID->rt << " " << dec << ID->imme << endl;
			ID->output << "LUI";
			break;
		case 0x0C:	//andi
			//snapshot << "andi " << dec << ID->rt << " " << dec << ID->rs << " " << dec << ID->imme << endl;
			ID->output << "ANDI";
			break;
		case 0x0D:	//ori
			//snapshot << "ori " << dec << ID->rt << " " << dec << ID->rs << " " << dec << ID->imme << endl;	
			ID->output << "ORI";
			break;
		case 0x0E:	//nori
			//snapshot << "nori " << dec << ID->rt << " " << dec << ID->rs << " " << dec << ID->imme << endl;
			ID->output << "NORI";
			break;
		case 0x0A:	//slti
			//snapshot << "slti " << dec << ID->rt << " " << dec << ID->rs << " " << dec << ID->imme << endl;
			ID->imme = signExtend16(ID->imme);
			ID->output << "SLTI";
			break;
		case 0x04:	//beq
			//snapshot << "beq " << dec << ID->rs << " " << dec << ID->rt << " " << dec << ID->imme << endl;
			ID->imme = signExtend16(ID->imme);
			ID->output << "BEQ";
			break;
		case 0x05:	//bne
			//snapshot << "bne " << dec << ID->rs << " " << dec << ID->rt << " " << dec << ID->imme << endl;
			ID->imme = signExtend16(ID->imme);
			ID->output << "BNE";
			break;
		case 0x07:	//bgtz
			//snapshot << "beq " << dec << ID->rs << " " << dec << ID->imme << endl;
			ID->imme = signExtend16(ID->imme);
			ID->output << "BGTZ";
			break;
		//J-format
		case 0x02:	//j
			//snapshot << "j " << dec << ID->addr << endl;
			ID->addr = signExtend26(ID->addr);
			ID->output << "J";
			break;
		case 0x03:	//jal
			//snapshot << "jal " << dec << ID->addr << endl;
			ID->addr = signExtend26(ID->addr);
			ID->output << "JAL";
			break;
		//S-format
		case 0x3F:	//halt
			ID->output << "HALT";
			haltID = true;
			break;
		//illegal
		default:
			cout << "illegal instruction found at 0x" << setfill('0') << setw(8) << hex << uppercase << (reg[PC] - 4) << endl;
			exit(0);
			break;
	}

	//forward rs
	if(needRs(ID->opcode, ID->func) && ID->rs != 0){
		switch(ID->opcode){
			case 0x00:
				if(ID->func == 0x08){
					if(!usedEX(ID->rs) && !usedMEM(ID->rs)){
						ID->input1 = reg[ID->rs];
					}
					else if(usedMEM(ID->rs) && !usedEX(ID->rs)){
						if(checkMEM()){
							ID->input1 = MEM->calc;
							ID->output << " fwd_EX-DM_rs_$" << dec << ID->rs;
						}
						else{
							ID->needToStall = true;
							return;
						}
					}
					else{
						ID->needToStall = true;
						return;
					}
				}
				else{
					if(usedEX(ID->rs)){
						if(!checkEX()){
							ID->needToStall = true;
							return;
						}
					}
				}
			break;
			case 0x04:
			case 0x05:
			case 0x07:
				if(!usedEX(ID->rs) && !usedMEM(ID->rs)){
					ID->input1 = reg[ID->rs];
				}
				else if(usedMEM(ID->rs) && !usedEX(ID->rs)){
					if(checkMEM()){
						ID->input1 = MEM->calc;
						ID->output << " fwd_EX-DM_rs_$" << dec << ID->rs;
					}
					else{
						ID->needToStall = true;
						return;
					}
				}
				else{
					ID->needToStall = true;
					return;
				}
			break;
			default:
				if(usedEX(ID->rs)){
					if(!checkEX()){
						ID->needToStall = true;
						return;
					}						
				}
			break;
		}
	}

	//forward rt
	if(needRt(ID->opcode, ID->func) && ID->rt != 0){
		switch(ID->opcode){
			case 0x00:
				if(ID->func == 0x08){
					if(!usedEX(ID->rt) && !usedMEM(ID->rt)){
						ID->input2 = reg[ID->rt];
					}
					else if(usedMEM(ID->rt) && !usedEX(ID->rt)){
						if(checkMEM()){
							ID->input2 = MEM->calc;
							ID->output << " fwd_EX-DM_rt_$" << dec << ID->rt;
						}
						else{
							ID->needToStall = true;
							return;
						}
					}
					else{
						ID->needToStall = true;
						return;
					}
				}
				else{
					if(usedEX(ID->rt)){
						if(!checkEX()){
							ID->needToStall = true;
							return;
						}
					}
				}
			break;
			case 0x04:
			case 0x05:
			case 0x07:
				if(!usedEX(ID->rt) && !usedMEM(ID->rt)){
					ID->input2 = reg[ID->rt];
				}
				else if(usedMEM(ID->rt) && !usedEX(ID->rt)){
					if(checkMEM()){
						ID->input2 = MEM->calc;
						ID->output << " fwd_EX-DM_rt_$" << dec << ID->rt;
					}
					else{
						ID->needToStall = true;
						return;
					}
				}
				else{
					ID->needToStall = true;
					return;
				}
				break;
			default:
				if(usedEX(ID->rt)){
					if(!checkEX())
						ID->needToStall = true;
						return;
				}
				break;
		}
	}	

	//branch
	switch(ID->opcode){
		case 0x00:
			if(ID->func == 0x08){	//jar
				PC_next = ID->input1;
				needToFlush = true;
			}
			break;
		case 0x04:	//beq
			if(ID->input1 == ID->input2){
				PC_next = reg[PC] + 4 * ID->imme;
				needToFlush = true;
			}
			break;
		case 0x05:	//bne
			if(ID->input1 != ID->input2){
				PC_next = reg[PC] + 4 * ID->imme;
				needToFlush = true;
			}
			break;
		case 0x07:	//bgtz
			if(ID->input1 > 0){
				PC_next = reg[PC] + 4 * ID->imme;
				needToFlush = true;
			}
			break;
		case 0x02:	//j
			PC_next = (((reg[PC]) >> 28) << 28) | 4 * ID->addr;
			needToFlush = true;
			break;
		case 0x03:	//jal
			ID->calc = reg[PC];
			PC_next = (((reg[PC]) >> 28) << 28) | 4 * ID->addr;
			needToFlush = true;
			break;
	}
}

int signExtend16(int val){
	return (val << 16) >> 16;
}

int signExtend26(int val){
	return (val << 6) >> 6;
}

bool needRs(unsigned opcode, unsigned func){
	switch(opcode){
		case 0x00:
			switch(func){
				case 0x00:
				case 0x02:
				case 0x03:
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

bool needRt(unsigned opcode, unsigned func){
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
		case 0x04:
		case 0x05:
			return true;
		break;
		default:
			return false;
		break;
	}
}

bool usedEX(unsigned input){
	//EX
	switch(stage.EX->opcode){
		case 0x00:
			switch(stage.EX->func){
				case 0x00:
					if(stage.EX->rt != 0 || stage.EX->rd != 0 || stage.EX->shamt != 0){
						if(input == stage.EX->rd){
							return true;
						}
						else{
							return false;
						}
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
					if(input == stage.EX->rd){
						return true;
					}
					else{
						return false;
					}
				break;
			}
		break;
		case 0x2B:
		case 0x29:
		case 0x28:
		case 0x04:
		case 0x05:
		case 0x07:
		case 0x02:
		case 0x3F:
			return false;
		break;
		case 0x03:
			if(input == 31){
				return true;
			}
			else{
				return false;
			}
		break;
		default:
			if(input == stage.EX->rt){
				return true;
			}
			else{
				return false;
			}
		break;
	}	
}

bool usedMEM(unsigned input){
	//MEM
	switch(stage.MEM->opcode){
		case 0x00:
			switch(stage.MEM->func){
				case 0x00:
					if(stage.MEM->rt != 0 || stage.MEM->rd != 0 || stage.MEM->shamt != 0){
						if(input == stage.MEM->rd){
							return true;
						}
						else{
							return false;
						}
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
					if(input == stage.MEM->rd){
						return true;
					}
					else{
						return false;
					}
				break;
			}
		break;
		case 0x2B:
		case 0x29:
		case 0x28:
		case 0x04:
		case 0x05:
		case 0x07:
		case 0x02:
		case 0x3F:
			return false;
		break;
		case 0x03:
			if(input == 31){
				return true;
			}
			else{
				return false;
			}
		break;
		default:
			if(input == stage.MEM->rt){
				return true;
			}
			else{
				return false;
			}
		break;
	}
}

bool usedWB(unsigned input){
	//WB
	switch(stage.WB->opcode){
		case 0x00:
			switch(stage.WB->func){
				case 0x00:
					if(stage.WB->rt != 0 || stage.WB->rd != 0 || stage.WB->shamt != 0){
						if(input == stage.WB->rd){
							return true;
						}
						else{
							return false;
						}
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
					if(input == stage.WB->rd){
						return true;
					}
					else{
						return false;
					}
				break;
			}
		break;
		case 0x2B:
		case 0x29:
		case 0x28:
		case 0x04:
		case 0x05:
		case 0x07:
		case 0x02:
		case 0x3F:
			return false;
		break;
		case 0x03:
			if(input == 31){
				return true;
			}
			else{
				return false;
			}
		break;
		default:
			if(input == stage.WB->rt){
				return true;
			}
			else{
				return false;
			}
		break;
	}
}

bool checkEX(){
	switch(stage.EX->opcode){
		case 0x00:
			switch(stage.EX->func){
				case 0x00:
					if(stage.EX->output.str() != "NOP"){
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

bool checkMEM(){
	switch(stage.MEM->opcode){
		case 0x00:
			switch(stage.MEM->func){
				case 0x00:
					if(stage.MEM->rt != 0 || stage.MEM->rd != 0 || stage.MEM->shamt != 0){
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