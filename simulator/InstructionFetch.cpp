#include <iomanip>
#include <sstream>
#include <iostream>
#include "libraries.h"

using namespace std;

extern Stage stage;
extern int PC_next;
extern int cycle;

void InstructionFetch(){
	stage.haltIF = false;
	if(stage.needToFlush){
		stage.IF = new Instruction(insMemory[(reg[PC])/4]);
		stage.IF->output << "0x" << setfill('0') << setw(8) << hex << uppercase << insMemory[(reg[PC]/4)] << " to_be_flushed";
		reg[PC] = PC_next;
		stage.IF->needToStall = false;
	}
	else if(stage.ID->needToStall){
		if(stage.ID == stage.IF){
			stage.IF = new Instruction(insMemory[(reg[PC])/4]);
			stage.IF->output << "0x" << setfill('0') << setw(8) << hex << uppercase << insMemory[(reg[PC]/4)] << " to_be_stalled";
			stage.IF->needToStall = true;
		}
	}
	else if(stage.IF->needToStall){
		reg[PC] = PC_next;
		string s;
		stage.IF->output >> s;
		stage.IF->output.str("");
		stage.IF->output.clear();
		stage.IF->output << s;
		stage.IF->needToStall = false;
	}
	else{
		stage.IF = new Instruction(insMemory[(reg[PC])/4]);
		stage.IF->output << "0x" << setfill('0') << setw(8) << hex << uppercase << insMemory[(reg[PC]/4)];
		reg[PC] = PC_next;
	}
	if(stage.IF->opcode == 0x3F){
		stage.haltIF = true;
	}
	stage.IFID = stage.IF;
}