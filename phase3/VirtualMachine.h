
/*

Phillip Pasqual
Charles Kinzel

CSE 460
Dr. Zemoudeh

Phase1
VirtualMachine.h

*/

#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <fstream>
#include <vector>

#include "PT.h"

using namespace std;

class VirtualMachine
{
	int msize;
	int rsize;
	int pc, ir, sr, sp, clock;
	vector<int> mem;
	vector<int> r;
	int base, limit;
	int limitmax;
    int elapsedClock;
    bool isSwitch;
    PT TLB;

    int frameReg[32];

public:
	VirtualMachine() : msize(256), rsize(4), clock(0)
	{
		mem = vector<int>(msize);
		r = vector<int>(rsize);
	}
	void run(int, fstream&, fstream&);
	void load(fstream&, int base, int & limit);
	int get_clock();
friend
	class OS;

};

#endif

