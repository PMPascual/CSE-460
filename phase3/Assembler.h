//CSE 460 - Operating Systems
//Winter 2019
//Instructor: Dr. Zemoudeh
//Names: Phillip Pascual / Charles Kinzel
//
//Filename: Assembler.h
//Description: Header file for the Assembler class.  Description of the
//             Assembler class operations is in Assembler.cpp.

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <fstream>

using namespace std;

class Assembler
{
public:
    Assembler();
    int assemble(fstream& in, fstream& out);
};

#endif
