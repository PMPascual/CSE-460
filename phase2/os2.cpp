#include "os.h"
#include "Assembler.h"
#include "VirtualMachine.h"

using namespace std;

OS::OS()
{
    int i = 0;
    int base = 0;
    
    string prog;
    
    system("ls *.s > progs");

    progs.open("progs", ios::in);

    while (progs >> prog)
    {
        int dot;
        int limit;
        string progName, outName;
        fstream source, object;

        dot = progs.find(".");
        progName = prog.substr(0, dot); //is this the right syntax?
        outName = progName + ".o";
        
        source.open(prog.c_str(), ios::in);
        object.open(outName.c_str(), ios::in, ios::out);

        as.assemble(source, object);       //Assembles .o file from .s file

        limit = vm.memLoad(object);     //Loads .o file into memory, function
                                        //returns index of last instruction
                                        //loaded into memory
        source.close();
        object.close();

        PCB* p = new PCB(progName, base, limit);
        
        //Opens the user-created input
        //Creates output file
        //Creates stack file
        p->in.open(progName + ".in".c_str(), ios::in);
        p->out.open(progName + ".out".c_str(), ios::out);
        p->stack.open(progName + ".st".c_str(), ios::in, ios::out);
        
        base = limit + 1;   //Moves base to index following previous limit
                            //to prepare for next file

        jobs.push_back(p);
    }
}

OS::~OS()
{
    list<PCB*>::iterator i;
    for (i = jobs.begin(); i != jobs.end(); ++i)
    {
        delete jobs[i];
    }
}

