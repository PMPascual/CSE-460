#include <fstream>
#include <iostream>

#include "OS.h"

using namespace std;

OS::OS()
{
	int i = 0;

	//Initializes system timers
	sys_time = 0;
	idle_time = 0;
	user_CPU = 0;
	pro_num = 0;

	system("ls *.s > progs");
	

	progs.open("progs", ios::in);

	int base = 0;
	int limit = 0;
	string prog;
	while (progs >> prog)
	{
		int dot;
		string progName, objName, inName, outName, stackName;
		fstream source, objectIn, objectOut;

		dot = prog.find(".");
		cout << dot << endl;
		progName = prog.substr(0, dot); //is this the right syntax?
		objName = progName + ".o";

		//Debug
		cout << objName << endl;
		cout << prog << endl;

		source.open(prog.c_str(), ios::in);
		objectOut.open(objName.c_str(), ios::out);
		if (!objectOut.is_open())
			cout << "objectOut not open!" << endl;

		objectIn.open(objName.c_str(), ios::in);
		if (!objectIn.is_open())
			cout << "objectIn not open!" << endl;
		as.assemble(source, objectOut);

		vm.load(objectIn, base, limit);

		PCB* p = new PCB(progName, base, limit);

		//Opens the user-created input
		//Creates output file
		//Creates stack file

		inName = progName + ".in";
		outName = progName + ".out";
		stackName = progName + ".st";

		cout << inName << endl;
		cout << outName << endl;
		cout << stackName << endl;

		p->in.open(inName.c_str(), ios::in);
		p->out.open(outName.c_str(), ios::out);
		p->stackOut.open(stackName.c_str(), ios::out);
		p->stackIn.open(stackName.c_str(), ios::in);

		source.close();
		objectIn.close();
		objectOut.close();

		jobs.push_back(p);
		base = limit;

		//Debug
		cout << "PCB's created and loaded." << endl;
		cout << "Jobs Size " << jobs.size() << endl;
		

	}
}
void OS::loadState()
{
	vm.pc = running->pc;
	vm.base = running->base;
	vm.limit = running->limit;
	vm.sr = running->sr;
	vm.ir = running->ir;
	vm.sp = running->sp;
	for (int i = 0; i < 4; i++)
		vm.r[i] = running->r[i];
	running->stackIn.seekg(0, ios::beg);
	for (int i = vm.sp; i < vm.msize; i++)
		running->stackIn >> vm.mem[i];

}

void OS::saveState()
{
	cout.flush();
	running->pc = vm.pc;
	
	running->base = vm.base;
	
	running->limit = vm.limit;
	running->sr = vm.sr;
	running->ir = vm.ir;
	running->sp = vm.sp;
	
	for (int i = 0; i < 4; i++)
		running->r[i] = vm.r[i];
	running->stackOut.seekp(0, ios::beg);
	for (int i = vm.sp; i < vm.msize; i++)
		running->stackOut << vm.mem[i] << endl;
	

}

void OS::run()
{
	for (list<PCB*>::iterator it = jobs.begin(); it != jobs.end(); ++it)
	{
		readyQ.push(*it);
		pro_num++;
	}
	//cout << "Ready Q count = " << readyQ.size() << endl;
	running = readyQ.front();
	readyQ.pop();
	//loadState();
	//vm.run(TIME_SLICE, running->in, running->out);
	
	//while (!readyQ.empty() || !waitQ.empty() || running != 0)
	while ( running != 0)
	{
			
		vm.clock += CONTEXT_SWITCH_TIME;
		sys_time += CONTEXT_SWITCH_TIME;

			loadState();
			vm.run(TIME_SLICE, running->in, running->out);
			contextSwitch();
	
	}
	/*
	System Time = sum of all Context Switch Times and Idle Times
	System CPU Utilization : percent of time CPU is busy = (final clock - sum of all Idle Times) / final clock
	User CPU Utilization : percent of the time CPU executes user jobs = (sum of all jobs' CPU time) / final clock 
	Throughput: number of processes completed per second.Assume 1 second = 1000 clock ticks.*/

	cout << "Run complete!" << endl;
	cout << "System_Time: " << (idle_time + sys_time) << endl;
	cout << "System CPU Utilization: " << (vm.clock - idle_time) / vm.clock << endl;
	cout << "User CPU Utilization: " << user_CPU / vm.clock << endl;
	cout << "Throughput: " << (vm.clock / (pro_num * 1000)) << endl;
	cout << "**************************" << endl;
	cout << "use_CPU " << user_CPU << endl;
	cout << "clock " << vm.clock << endl;

    for (list<PCB*>::iterator it = jobs.begin(); it != jobs.end(); ++it)
    {
        (*it)->out << endl;
        (*it)->out << "Run complete!" << endl;
        (*it)->out << "System_Time: " << (idle_time + sys_time) << endl;
        (*it)->out << "System CPU Utilization: " << (vm.clock - idle_time) / vm.clock << endl;
        (*it)->out << "User CPU Utilization: " << user_CPU / vm.clock << endl;
        (*it)->out << "Throughput: " << (vm.clock / (pro_num * 1000)) << endl;
        (*it)->out << "**************************" << endl;
        (*it)->out << "use_CPU " << user_CPU << endl;
        (*it)->out << "clock " << vm.clock << endl;
    }


	return;
}

void OS::dma()
{
	//Debug
	for (int whatever = 0; whatever < waitQ.size(); whatever++) {
		if (waitQ.front()->io_completion <= vm.clock) {
			waitQ.front()->io_time += (vm.clock - waitQ.front()->io_time_begin);
			waitQ.front()->wait_time_begin = vm.clock;
			readyQ.push(waitQ.front());
			waitQ.pop();
		}
	}
	
	if (!readyQ.empty()) {
		
		
		running = readyQ.front();
		readyQ.pop();
		running->wait_time += (vm.clock - running->wait_time_begin);
		
	}
	else if (!waitQ.empty())
	{
		while (!waitQ.empty() && waitQ.front()->io_completion > vm.clock)
		{
			//Debug
			

			vm.clock++;
			idle_time++;
			
		}
		
		//while (waitQ.front()->io_completion <= vm.clock)
		//{
			
			//readyQ.push(waitQ.front());
			running = waitQ.front();
			
			
			cout.flush();
			if (waitQ.empty())
			{
				cout << "WaitQ empty!" << endl;
				cout.flush();
			}
			waitQ.pop();
			
			cout.flush();
			running->io_time += (vm.clock - running->io_time_begin);
		//}
	}
	
}

void OS::contextSwitch()
{

	//dma();  //Checks waitQ and pushes to ready if i/o done
	int vmStatus = (vm.sr >> 5) & 07;
	//int vmStatus = (vm.sr >> 5);//////////////////////////////////////////////////////////////////
	//vmStatus = (vm.sr & 0xd0) >> 5;
	int ioReg = (vm.sr & 0x300) >> 8;
	running->cpu_time += vm.elapsedClock;
	

	if (vmStatus == 0)  //Time for a slice!
	{
		
		//sys_time += 5;
		
		readyQ.push(running);
		
		running->wait_time_begin = vm.clock;
		
		
	}
	else if (vmStatus == 1) //Halt instruction passed
	{
		
		//sys_time += 5;
		running->stackOut << "Out!" << endl;

		//Take this out later
		running->out << "cpu_time:  " << running->cpu_time << endl;
		running->out << "I/O_time: " << running->io_time << endl;
		running->out << "wait_time: " << running->wait_time << endl;
		running->out << "turnaround_time " << (running->cpu_time + running->io_time + running->wait_time) << endl;
		user_CPU += running->cpu_time;
		
	}
	else if (vmStatus == 2) //Out of Bounds
	{
		cout << "out of bounds error" << endl;
		running->out << "cpu_time:  " << running->cpu_time << endl;
		running->out << "I/O_time: " << running->io_time << endl;
		running->out << "wait_time: " << running->wait_time << endl;
		running->out << "turnaround_time " << (running->cpu_time + running->io_time + running->wait_time) << endl;
		user_CPU += running->cpu_time;
		
	}
	else if (vmStatus == 3) //Stack Overflow
	{
		running->out << "cpu_time:  " << running->cpu_time << endl;
		running->out << "I/O_time: " << running->io_time << endl;
		running->out << "wait_time: " << running->wait_time << endl;
		running->out << "turnaround_time " << (running->cpu_time + running->io_time + running->wait_time) << endl;
		user_CPU += running->cpu_time;

	}
	else if (vmStatus == 4) //Stack Underflow
	{
		running->out << "cpu_time:  " << running->cpu_time << endl;
		running->out << "I/O_time: " << running->io_time << endl;
		running->out << "wait_time: " << running->wait_time << endl;
		running->out << "turnaround_time " << (running->cpu_time + running->io_time + running->wait_time) << endl;
		user_CPU += running->cpu_time;
	}
	else if (vmStatus == 5) //Invalid Opcode
	{
		
	}
	else if (vmStatus == 6)    //Read Operation
	{
		
		sys_time += 5;

		//Simulates DMA, reads from *.in file into destination register
		running->in >> running->r[ioReg];
		running->io_time_begin = vm.clock;
		running->io_completion = vm.clock + 27;
		waitQ.push(running);
		//running = 0;
	}
	else if (vmStatus == 7)    //Write Operation
	{
		
		sys_time += 5;
		saveState();

		//Simulates DMA, writes from source register into *.out file
		running->out << running->r[ioReg];

		running->io_time_begin = vm.clock;
		running->io_completion = vm.clock + 27;
		waitQ.push(running);
		//running = 0;
	}
	vm.isSwitch = false;
	
	
	saveState();
	
	cout.flush();
    running = 0;
	dma();

	/*
	if (!readyQ.empty()) {
		running = readyQ.front();
		readyQ.pop();
	}
	if (not readyQ.empty()) {
		cout << "ready ready Q popped!" << endl;
		cout.flush();
		running = readyQ.front();
		readyQ.pop();
		
	}
	else if (not waitQ.empty()) {
		running = waitQ.front();
		waitQ.pop();
		cout << "Wait wait Q popped!" << endl;
		cout.flush();
	}*/


}

OS::~OS()
{
	for (list<PCB*>::iterator it = jobs.begin(); it != jobs.end(); ++it)
	{
		delete *it;
	}
}

/*
//Main program
int main()
{
	OS os;
	os.run();
}*/

