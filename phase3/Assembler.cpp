//CSE 460 - Operating Systems
//Winter 2019
//Instructor: Dr. Zemoudeh
//Names: Phillip Pascual / Charles Kinzel
//
//Filename: Assemble.cpp
//Description:  This program takes in a source file and output file, written in 
//              the assembly language specified in the Phase One documentation.
//
//              This program reads one line of the source file at a time and
//              creates a string.  It then uses an istringstream operation to 
//              read first the operation, then the variables.
//
//              Depending on the operation read from the source file, a
//              specific opcode is generated and pushed onto bits [11:15] of a 
//              16-bit instruction integer.  Then, depending on the opcode, the
//              operands following the operation in the source file is encoded
//              into the destination register (rd), source register (rs), or
//              address/constant bits in the instruction integer.  The rd bits
//              are [10:9], rs bits are [7:6] and address/constant bits are 
//              [7:0].
//
//              Also, depending on the operation in the source file, the
//              immediate bit [8] is set as a 0 or 1.  1 indicates immediate
//              mode is in effect, 0 indicates immediate mode is not in effect.
//
//              Once each instruction is encoded into the instruction integer,
//              the integer is written onto the output file, one line at a time,
//              and the next instruction is read from the source file.  This
//              read-encode-write process is continued until there are no more
//              lines to read in the source file.
//
//              If at any time the instruction on the source file does not match
//              an instruction indicated in this program, the program returns
//              and error.  If the program reads through the last line of the
//              source file successfully, the program returns success.
//
//              This program is called by the OS program and returns a bool to
//              indicate success or failure of the assembly process.

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

#include "Assembler.h"

using namespace std;

Assembler::Assembler()
{
}

int Assembler::assemble(fstream& in, fstream& out)
{
    string line;
    string opcode;
    int rd, rs, addr, constant;
    int instruction;

    const int success = false;
    const int error = true;
    //const int debug = false;
    
    getline(in, line);
    while (!in.eof())
    {
        if (line[0] == '!' or line == "")
        {
            getline(in, line);
            continue;
        }

        istringstream str(line.c_str());
        str >> opcode;

        if (opcode == "load")
        {
            str >> rd >> addr;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            if (addr < 0 || addr > 255)
            {
                return error;
            }
            instruction = 0;
            instruction = instruction | rd<<9 | addr;
        }
        else if (opcode == "loadi")
        {
            str >> rd >> constant;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            if (constant < -128 || constant > 127)
            {
                return error;
            }
            instruction = 0;
            instruction = instruction | rd<<9 | 1<<8 | (0xff & constant);
        }
        else if (opcode == "store")
        {
            str >> rd >> addr;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            if (addr < 0 || addr > 255)
            {
                return error;
            }
            instruction = 1;
            instruction = instruction<<11 | rd<<9 | 1<<8| addr;
        }
        else if (opcode == "add")
        {
            str >> rd >> rs;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            if (rs < 0 || rs > 3)
            {
                return error;
            }
            instruction = 2;
            instruction = instruction<<11 | rd<<9 | rs<<6;
        }
        else if (opcode == "addi")
        {
            str >> rd >> constant;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            if (constant < -128 || constant > 127)
            {
                return error;
            }
            instruction = 2;
            instruction = instruction<<11 | rd<<9 | 1<<8 | (0xff & constant);
        }
        else if (opcode == "addc")
        {
            str >> rd >> rs;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            if (rs < 0 || rs > 3)
            {
                return error;
            }
            instruction = 3;
            instruction = instruction<<11| rd<<9 | rs<<6;
        }
        else if (opcode == "addci")
        {
            str >> rd >> constant;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            if (constant < -128 || constant > 127)
            {
                return error;
            }
            instruction = 3;
            instruction = instruction<<11 | rd<<9 | 1<<8 | (0xff & constant);
        }
        else if (opcode == "sub")
        {
            str >> rd >> rs;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            if (rs < 0 || rs > 3)
            {
                return error;
            }
            instruction = 4;
            instruction = instruction<<11 | rd<<9 | rs<<6;
        }
        else if (opcode == "subi")
        {
            str >> rd >> constant;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            if (constant < -128 || constant > 127)
            {
                return error;
            }
            instruction = 4;
            instruction = instruction<<11 | rd<<9 | 1<<8 | (0xff & constant);
        }
        else if (opcode == "subc")
        {
            str >> rd >> rs;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            if (rs < 0 || rs > 3)
            {
                return error;
            }
            instruction = 5;
            instruction = instruction<<11 | rd<<9 | rs<<6;
        }
        else if (opcode == "subci")
        {
            str >> rd >> constant;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            if (constant < -128 || constant > 127)
            {
                return error;
            }
            instruction = 5;
            instruction = instruction<<11 | rd<<9 | 1<<8 | (0xff & constant);
        }
        else if (opcode == "and")
        {
            str >> rd >> rs;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            if (rs < 0 || rs > 3)
            {
                return error;
            }
            instruction = 6;
            instruction = instruction<<11 | rd<<9 | rs<<6;
        }
        else if (opcode == "andi")
        {
            str >> rd >> constant;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            if (constant < -128 || constant > 127)
            {
                return error;
            }
            instruction = 6;
            instruction = instruction<<11 | rd<<9 | 1<<8 | (0xff & constant);
        }
        else if (opcode == "xor")
        {
            str >> rd >> rs;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            if (rs < 0 || rs > 3)
            {
                return error;
            }
            instruction = 7;
            instruction = instruction<<11 | rd<<9 | rs<<6;
        }
        else if (opcode == "xori")
        {
            str >> rd >> constant;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            if (constant < -128 || constant > 127)
            {
                return error;
            }
            instruction = 7;
            instruction = instruction<<11 | rd<<9 | 1<<8 | (0xff & constant);
        }
        else if (opcode == "compl")
        {
            str >> rd;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            instruction = 8;
            instruction = instruction<<11 | rd<<9 | 0<<8; //Since DNC is 0
        }                                                 //by convention
        else if (opcode == "shl")
        {
            str >> rd;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            instruction = 9;
            instruction = instruction<<11 | rd<<9 | 0<<8; //i is DNC
        }
        else if (opcode == "shla")
        {
            str >> rd;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            instruction = 10;
            instruction = instruction<<11 | rd<<9 | 0<<8; //i is DNC
        }
        else if (opcode == "shr")
        {
            str >> rd;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            instruction = 11;
            instruction = instruction<<11 | rd<<9 | 0<<8; //i is DNC
        }
        else if (opcode == "shra")
        {
            str >> rd;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            instruction = 12;
            instruction = instruction<<11 | rd<<9 | 0<<8; //i is DNC
        }
        else if (opcode == "compr")
        {
            str >> rd >> rs;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            if (rs < 0 || rs > 3)
            {
                return error;
            }
            instruction = 13;
            instruction = instruction<<11 | rd<<9 | rs<<6;
        }
        else if (opcode == "compri")
        {
            str >> rd >> constant;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            if (constant < -128 || constant > 127)
            {
                return error;
            }
            instruction = 13;
            instruction = instruction<<11 | rd<<9 | 1<<8 | (0xff & constant);
        }
        else if (opcode == "getstat")
        {
            str >> rd;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            instruction = 14;
            instruction = instruction<<11 | rd<<9 | 0<<8; //i is DNC
        }
        else if (opcode == "putstat")
        {
            str >> rd;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            instruction = 15;
            instruction = instruction<<11 | rd<<9 | 0<<8; //i is DNC
        }
        else if (opcode == "jump")
        {
            str >> addr;
            if (addr < 0 || addr > 255)
            {
                return error;   //256 addresses, so range is 0-255
            }
            instruction = 16;
            instruction = instruction<<11 | 1<<8 | addr;
        }
        else if (opcode == "jumpl")
        {
            str >> addr;
            if (addr < 0 || addr > 255)
            {
                return error;
            }
            instruction = 17;
            instruction = instruction<<11 | 1<<8 | addr;
        }
        else if (opcode == "jumpe")
        {
            str >> addr;
            if (addr < 0 || addr > 255)
            {
                return error;
            }
            instruction = 18;
            instruction = instruction<<11 | 1<<8 | addr;
        }
        else if (opcode == "jumpg")
        {
            str >> addr;
            if (addr < 0 || addr > 255)
            {
                return error;
            }
            instruction = 19;
            instruction = instruction<<11 | 1<<8 | addr;
        }
        else if (opcode == "call")
        {
            str >> addr;
            if (addr < 0 || addr > 255)
            {
                return error;
            }
            instruction = 20;
            instruction = instruction<<11 | 1<<8 | addr;
        }
        else if (opcode == "return")
        {
            instruction = 21;
            instruction = instruction<<11;
        }
        else if (opcode == "read")
        {
            str >> rd;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            instruction = 22;
            instruction = instruction<<11 | rd<<9;
        }
        else if (opcode == "write")
        {
            str >> rd;
            if (rd < 0 || rd > 3)
            {
                return error;
            }
            instruction = 23;
            instruction = instruction<<11 | rd<<9;
        }
        else if (opcode == "halt")
        {
            instruction = 24;
            instruction = instruction<<11;
        }
        else if (opcode == "noop")
        {
            instruction = 25;
            instruction = instruction<<11;
        }
        else
        {
            return error;
        }

        out << setfill('0') << setw(5) << instruction << endl;
        getline(in, line);
    }
    return success;
} //assemble
