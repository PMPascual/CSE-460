//CSE 460 - Operating Systems
//Winter 2019
//Instructor: Dr. Zemoudeh
//Names: Phillip Pascual / Charles Kinzel
//
//Filename: VirtualMachine.h
//Definition: Header file for VirtualMachine.cpp. Contains class declarations,
//            default constructor and function declarations.  Full descriptions
//            of VirtualMachine class and function operations is in
//            VirtualMachine.cpp

#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <fstream>
#include <vector>

using namespace std;

class VirtualMachine
{
    int msize;
    int rsize;
    int pc, ir, sr, sp, clock;
    vector<int> mem;
    vector<int> r;
    int base, limit;
public:
    VirtualMachine(): msize(256), rsize(4), clock(0)
    {
        mem = vector<int>(msize);
        r = vector<int>(rsize);
    }
    void run(fstream&, fstream&, fstream&);
    int get_clock();
    int memLoad(fstream&);
    int get_pc();
    int get_sr();
    int get_register(int);
    void set_pc(int);
    void set_sr(int);
    void set_register(int, int);
}; //VirtualMachine

#endif
