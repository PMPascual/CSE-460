//CSE 460 - Operating Systems
//Winter 2019
//Instructor: Dr. Zemoudeh
//Names: Phillip Pascual / Charles Kinzel
//
//Filename: os.cpp
//Description: Main for toy VM and Operating System.  Takes in source file.
//
//             Program starts by checking for source file.  If no file passed
//             in, or passed-in file does not end in *.s suffix, program outputs
//             an error message and exits.  If source file exists, program
//             creates an object file to pass to Assembler.  Program attempts to
//             open both the *.s and the *.o file, if unsuccessful on either
//             outputs an error and exits.
//
//             If successful, os.cpp passes both the *.s and *.o file into the
//             assemble function in Assembler.cpp.  If function returns fail,
//             program outputs 'Assembler Error', closes both files, and exits.
//             If the assemble function successfully completes, os.cpp closes
//             both files.
//
//             Program then again attempts to re-open the object file, and
//             outputs an error and closes if unsuccessful.  If successful,
//             program opens the user-created *.in file and creates an output
//             file with the suffix *.out.  Program then attempts to open both
//             files, and outputs an error and exits if unsuccessful.
//
//             Once both the *.in, *.out, and *.o file are opened, all three
//             files are passed into the run function in the VirtualMachine
//             class.  Once VM run function is completed, os.cpp writes the
//             clock to the *.out file, closes the *.o, *.in, and *.out files
//             and terminates.

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

#include "Assembler.h"
#include "VirtualMachine.h"

using namespace std;

int main(int argc, char *argv[])
{
    Assembler as;
    VirtualMachine vm;

    if (argc == 1)
    {
        cout << "Must supply an assembly file name.\n";
        exit(1);
    }

    string assemblyFile = argv[1];
    int pos = assemblyFile.find(".");
    if (pos > assemblyFile.length() || assemblyFile.substr(pos) != ".s")
    {
        cout << "No .s suffix.\n";
        exit(2);
    }

    string name = assemblyFile.substr(0, pos);
    string objectFile = name + ".o";

    fstream assembly, objectCode;
    assembly.open(assemblyFile.c_str(), ios::in);
    objectCode.open(objectFile.c_str(), ios::out);
    if (!assembly.is_open() or !objectCode.is_open())
    {
        cout << "couldn't open " << assemblyFile << " and/or " << objectFile
            << endl;
        exit(3);
    }

    if (as.assemble(assembly, objectCode))
    {
        cout << "Assembler Error\n";
        assembly.close();
        objectCode.close();
        exit(4);
    }

    assembly.close();
    objectCode.close();

    objectCode.open(objectFile.c_str(), ios::in);
    if (!objectCode.is_open())
    {
        cout << "Couldn't open " << objectFile << endl;
        exit(5);
    }

    string inFile = name + ".in";
    string outFile = name + ".out";
    fstream in, out;
    in.open(inFile.c_str(), ios::in);
    out.open(outFile.c_str(), ios::out);
    if (!in.is_open() or !out.is_open())
    {
        cout << "Couldn't open " << inFile << " and/or " << outFile << endl;
        exit(6);
    }

    vm.run(objectCode, in, out);
    out << "Clock = " << vm.get_clock() << endl;

    cout << "Clock = " << vm.get_clock() << endl; 

    objectCode.close();
    in.close();
    out.close();

    return 0;
} //main
