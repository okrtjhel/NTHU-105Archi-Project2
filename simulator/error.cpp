#include <iostream>
#include <bitset>
#include <fstream>
#include "libraries.h"

using namespace std;

fstream error("_error_dump.rpt", ios::out);
extern int cycle;
extern bool shutdown;

bool WriteToZero_Check(unsigned destination){
	//R-format
	if(destination == 0){		
		error << "In cycle " << dec << cycle + 1 << ": Write $0 Error" << endl;
		return true;
	}
	else{
		return false;
	}	
}

void NumberOverflow_Check(int out, int in1, int in2){
	bitset<32> b0 = out, b1 = in1, b2 = in2;
	if(b1[31] == b2[31] && b1[31] != b0[31])
		error << "In cycle " << dec << cycle + 1 << ": Number Overflow" << endl;
}

void OverwriteHI_LO_Check(bool doMult){
	static bool needAccessed = false;
	if(doMult && needAccessed){
		error << "In cycle " << dec << cycle + 1 << ": Overwrite HI-LO registers" << endl;
	}
	else if(doMult)
		needAccessed = true;
	else
		needAccessed = false;
}

void AddressOverflow_Check(int location, int byte){
	if((location < 0) || (location > 1024) || (location + byte > 1024)){
		error << "In cycle " << dec << cycle + 1 << ": Address Overflow" << endl;
		shutdown = true;
	}
}

void Misaligned_Check(int location, int byte){
	if(location % byte){
		error << "In cycle " << dec << cycle + 1 << ": Misalignment Error" << endl;
		shutdown = true;
	}
}
