#ifndef OS_H
#define OS_H

#include <fstream>
#include <list>
#include <queue>
#include <string>

#include "VirtualMachine.h"
#include "Assembler.h"
#include "PT.h"

using namespace std;

class OS;

class PCB
{
	int pc;
	int r[4];
	int ir;
	int sr;
	int sp;
	int base;
	int limit;
    PT PT;

	string prog;
	fstream in;
	fstream out;
	fstream stackIn;
	fstream stackOut;

	int io_completion;

	int turn_around_time;
	int cpu_time;
	int wait_time=0;  int wait_time_begin=0;
	int io_time;    int io_time_begin;
	

public:
	PCB(const string &p, const int &b, const int &l) :
		prog(p), base(b), limit(l), pc(b), sr(2), sp(256/*vm.size*/),
		cpu_time(0), wait_time(0), io_time(0), wait_time_begin(0)
	{
		for (int i = 0; i < 4; ++i)
		{
			r[i] = 0;
		}
	}

	friend
		class OS;
};

class OS
{
	VirtualMachine vm;
	Assembler as;

	list<PCB*> jobs; //list of all jobs
	queue<PCB*> readyQ;
	queue<PCB*> waitQ;
	PCB* running;
	const static int TIME_SLICE = 15;
	const static int CONTEXT_SWITCH_TIME = 5;

	fstream progs;
	float idle_time;
	float sys_time;   //total sys time
	float user_CPU;
	float pro_num;

public:
	OS();
	~OS();
	void run();
	void loadState();
	void saveState();
	void contextSwitch();
	void dma();
};

#endif

