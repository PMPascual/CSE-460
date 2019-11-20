
/*

Phillip Pasqual
Charles Kinzel

CSE 460
Dr. Zemoudeh

Phase1
VirtualMachine.cpp

*/

#include <iostream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <cassert>

#include "VirtualMachine.h"

using namespace std;

void VirtualMachine::load(fstream& code, int base, int & limit)
{
	for (limit = base; code >> mem[limit]; limit++);
}


int VirtualMachine::get_clock()
{
	return clock;
}

void VirtualMachine::run(int timeSlice, fstream& in, fstream& out)
{
	const int debug = false;
	int opcode, rd, i, rs, constant, addr, j, sliceTime, clockStart;

    sliceTime = clock + timeSlice; //When to time slice
    clockStart = clock;

    //Don't need this.  Handled by load()
	/*base = 0;
	for (limit = 0; objectCode >> mem[limit]; limit++);*/

    sr &= 0x1f;

	while (pc < limit)
	{
		ir = mem[pc];
		pc++;
		opcode = (ir & 0xf800) >> 11;
		rd = (ir & 0x600) >> 9;
		i = (ir & 0x100) >> 8;
		rs = (ir & 0xc0) >> 6;
		addr = ir & 0xff;
		constant = addr;

		clock++;
       
		if (opcode == 0) //load loadi
		{
			if (i)
			{
				if (constant & 0x80)
				{
					constant |= 0xff00;
				}
				r[rd] = constant;
			}
			else
			{
				if (base + addr >= limit)
				{
					out << "Out of bound error.\n";
                    cout << "Out of bounds!" << endl;
                    sr &= 0xffffff1f;
                    sr |= 0x40;
					//return;
				}
                else
                {
				    r[rd] = mem[base + addr];
				    clock += 3;
                }
			}
		}
		else if (opcode == 1) //store
		{
			if (base + addr >= base + limit)
			{
				out << "Out of bound error.\n";
                cout << "Out of bounds!" << endl;
                sr &= 0xffffff1f;
                sr |= 0x40;
				return;
			}
			mem[base + addr] = r[rd];
			clock += 3;
		}
		else if (opcode == 2) //add addi
		{
			int sign1 = (r[rd] & 0x8000) >> 15;
			int sign2;
			if (i)
			{
				sign2 = (constant & 0x80) >> 7;
				//sign extend both operands to perform operation
				if (sign2)
				{
					constant |= 0xffffff00;
				}
				if (r[rd] & 0x8000)
				{
					r[rd] |= 0xffff0000;
				}
				r[rd] = r[rd] + constant;
			}
			else
			{
				sign2 = (r[rs] & 0x8000) >> 15;
				int temp = r[rs];
				//sign extend both operands to perform operation
				if (sign2)
				{
					temp |= 0xffff0000;
				}
				if (r[rd] & 0x8000)
				{
					r[rd] |= 0xffff0000;
				}
				r[rd] = r[rd] + temp;
			}

			//set CARRY
			if (r[rd] & 0x10000)
			{
				sr |= 0x1;
			}
			else
			{
				sr &= 0xfffe;
			}

			//set OVERFLOW
			if (sign1 == sign2 and sign1 != (r[rd] & 0x8000) >> 15)
			{
				sr |= 0x10;
			}
			else
			{
				sr &= 0xffef;
			}

			//keep it at 16 bits
			r[rd] &= 0xffff;
		}
		else if (opcode == 3) //addc addci
		{
			int sign1 = (r[rd] & 0x8000) >> 15;
			int sign2;
			if (i)
			{
				sign2 = (constant & 0x80) >> 7;
				//sign extend both operands to perform operation
				if (sign2)
				{
					constant |= 0xffffff00;
				}
				if (r[rd] & 0x8000)
				{
					r[rd] |= 0xffff0000;
				}
				r[rd] = r[rd] + constant + sr & 0x1;
			}
			else
			{
				sign2 = (r[rs] & 0x8000) >> 15;
				int temp = r[rs];
				//sign extend both operands to perform operation
				if (sign2)
				{
					temp |= 0xffff0000;
				}
				if (r[rd] & 0x8000)
				{
					r[rd] |= 0xffff0000;
				}
				r[rd] = r[rd] + temp + sr & 0x1;
			}

			//set CARRY
			if (r[rd] & 0x10000)
			{
				sr |= 0x1;
			}
			else
			{
				sr &= 0xfffe;
			}

			//set OVERFLOW
			if (sign1 == sign2 and sign1 != (r[rd] & 0x8000) >> 15)
			{
				sr |= 0x10;
			}
			else
			{
				sr &= 0xffef;
			}

			//keep it at 16 bits
			r[rd] &= 0xffff;

		}
		//*****CODE ADDED 1.12.19*****
		else if (opcode == 4)   //sub subi
		{
			int sign1 = (r[rd] & 0x8000) >> 15;
			int sign2;
			if (i)
			{
				sign2 = (constant & 0x80) >> 7;
				//sign extend both operands to perform operation
				if (sign2)
				{
					constant |= 0xffffff00;
				}
				if (sign1)
				{
					r[rd] |= 0xffffff00;
				}
				r[rd] = r[rd] - constant;
			}
			else
			{
				sign2 = (r[rs] & 0x8000) >> 15;
				int temp = r[rs];
				//sign extend both operands to perform operation
				if (sign2)
				{
					temp |= 0xffff0000;
				}
				if (sign1)
				{
					r[rs] |= 0xffff0000;
				}
				r[rd] = r[rd] - temp;
			}

			//set CARRY bit
			if (r[rd] & 0x10000)
			{
				sr |= 0x1;
			}
			else
			{
				sr &= 0xfffe;
			}

			//set OVERFLOW bit
			if (sign1 == sign2 and sign1 != (r[rd] & 0x8000) >> 15)
			{
				sr |= 0x10;
			}
			else
			{
				sr &= 0xffef;
			}

			//keep at 16 bits
			r[rd] &= 0xffff;
		}
		else if (opcode == 5)   //subc subci
		{
			int sign1 = (r[rd] & 0x8000) >> 15;
			int sign2;
			if (i)
			{
				sign2 = (constant & 0x80) >> 7;
				//sign extend both operands to perform
				if (sign2)
				{
					constant |= 0xffffff00;
				}
				if (sign1)
				{
					r[rd] |= 0xffff0000;
				}
				r[rd] = r[rd] - constant - sr & 0x1;
			}
			else
			{
				int temp;
				sign2 = (r[rs] & 0x8000) >> 15;
				temp = r[rs];
				//sign extend both
				if (sign2)
				{
					temp |= 0xffff0000;
				}
				if (sign1)
				{
					r[rd] |= 0xffff0000;
				}
				r[rd] = r[rd] - temp - sr & 0x1;
			}

			//set CARRY bit
			if (r[rd] & 0x10000)
			{
				sr |= 0x1;
			}
			else
			{
				sr &= 0xfffe;
			}

			//set OVERFLOW bit
			if (sign1 == sign2 and sign1 != (r[rd] & 0x8000) >> 15)
			{
				sr |= 0x10;
			}
			else
			{
				sr &= 0xffef;
			}

			//keep at 16 bits
			r[rd] &= 0xffff;
		}
		else if (opcode == 6)   //and andi
		{
			if (i)
			{
				int temp = r[rd];       //I don't know if we can bitwise and
				r[rd] = temp & constant;//inline, so created a temp veriable
			}                           //to store
			else
			{
				int temp = r[rd];       //Maybe we can do rd = rd & rs??
				r[rd] = temp & r[rs];
			}
			r[rd] &= 0xffff;
		}
		else if (opcode == 7)   //xor xori
		{
			if (i)                      //Can we do bitwise and/or this way?
			{                           //Need to ask about this one...
				int temp = r[rd];
				r[rd] = temp ^ constant;
			}
			else
			{
				int temp = r[rd];
				r[rd] = temp ^ r[rs];
			}
			r[rd] &= 0xffff;
		}
		else if (opcode == 8)   //compl
		{
			int temp = r[rd];           //Can we do complement like this?
			r[rd] = ~temp;
			r[rd] &= 0xffff;
		}
		else if (opcode == 9)   //shl
		{
			r[rd] = r[rd] << 1;   //shift r[rd] left one bit

								  //set CARRY
			if (r[rd] & 0x10000)
			{
				sr |= 0x1;
			}
			else
			{
				sr &= 0xfffe;
			}
			r[rd] &= 0xffff;
		}
		else if (opcode == 10)  //shla
		{
			int sign = (r[rd] & 0x8000);    //isolates sign bit

			r[rd] = r[rd] << 1;   //shifts one bit left
			if (sign)
			{
				r[rd] &= 0x7f;  //if sign bit was 1, sets sign bit on new
			}                   //r[rd] to 1 as well

								//set CARRY
			if (r[rd] & 0x10000)
			{
				sr |= 0x1;
			}
			else
			{
				sr &= 0xfffe;
			}

			//sign extend to 32-bits;
			if (r[rd] & 0x8000)
			{
				r[rd] |= 0xffff8000;
			}
			r[rd] &= 0xffff;
		}
		else if (opcode == 11)  //shr
		{
			int carry = (r[rd] & 0x1);

			r[rd] = r[rd] >> 1;   //shift r[rd] right one bit

								  //set CARRY
			if (carry)
			{
				sr |= 0x1;
			}
			else
			{
				sr &= 0xfffe;
			}
		}
		else if (opcode == 12)  //shra
		{
			int carry = (r[rd] & 0x1);

			int sign = (r[rd] & 0x8000);     //isolates sign bit
			r[rd] = r[rd] >> 1;
			if (sign)
			{
				r[rd] &= 0x7f;
			}

			//set CARRY
			if (carry)
			{
				sr |= 0x1;
			}
			else
			{
				sr &= 0xfffe;
			}

			//sign extend to 32-bits
			if (r[rd] & 0x8000)
			{
				r[rd] |= 0xffff8000;
			}
			r[rd] &= 0xffff;
		}
		else if (opcode == 13)  //compr compri
		{
			if (i)
			{
				if (r[rd] < constant)
				{
					sr |= 0x8;  //sets less bit to 1
					sr &= 0x19;  //sets greater and equal bits to 0
				}
				else if (r[rd] == constant)
				{
					sr |= 0x4;  //sets equal bit to 1
					sr &= 0x15;  //sets greater and less bits to 1
				}
				else if (r[rd] > constant)
				{
					sr |= 0x2;  //sets greater bit to 1
					sr &= 0x13;  //sets less and equal bits to 0
				}
			}
			else
			{
				if (r[rd] < r[rs])
				{
					sr |= 0x8;  //sets less bit to 1
					sr &= 0x19;  //sets greater and equal bits to 0
				}
				else if (r[rd] == r[rs])
				{
					sr |= 0x4;  //sets equal bit to 1
					sr &= 0x15;  //sets greater and less bits to 1
				}
				else if (r[rd] > r[rs])
				{
					sr |= 0x2;  //sets greater bit to 1
					sr &= 0x13;  //sets less and equal bits to 0
				}
			}
		}
		else if (opcode == 14)  //getstat
		{
			r[rd] = sr;
		}
		else if (opcode == 15)  //putstat
		{
			sr = r[rd];
		}
		else if (opcode == 16) //jump
		{
			pc = base + addr;
            if (addr > limit)    //Out of bounds
            {
                sr &= 0xffffff1f;
                sr |= 0x40;
                return;
            }
			
		}
		else if (opcode == 17) //jumpl
		{
			if (sr & 0x8)         //if less bit = 1
			{
				pc = base + addr;
			}
            if (addr > limit)    //Out of bounds
            {
                sr &= 0xffffff1f;
                sr |= 0x40;
                return;
            }
			
		}
		else if (opcode == 18)  //jumpe
		{
			if (sr & 0x4)         //if equal bit = 1
			{
				pc = base + addr;
			}
			if (addr >= limit)
            //if (addr < base or base + addr > limit)    //Out of bounds
            {
                sr &= 0xffffff1f;
                sr |= 0x40;
                return;
            }
			
		}
		else if (opcode == 19)  //jumpg
		{
            if (addr < base or base + addr > limit)    //Out of bounds
            {
                sr &= 0xffffff1f;
                sr |= 0x40;
                return;
            }
			if (sr & 0x2)         //if greater bit = 1
			{
				pc = base + addr;
			}
		}
		else if (opcode == 20)  //call *DON'T REALLY KNOW IF THIS ONE IS RIGHT*
		{
			if (base + addr > msize)
			{
				out << "Out of bound error.\n"; //Out of bound error
                sr &= 0xffffff1f;
                sr |= 0x40;
				return;
			}
			//Since variable 'limit' is the end of program memory, and we need
			//a block of 6 in the stack for a call, if sp < limit + 6 no space
			//in stack
			if (sp < base + limit + 6)
			{
				out << "Stack overflow.\n";     //Stack overflow??
                sr &= 0xffffff1f;
                sr |= 0x60;
                return;
			}
			mem[--sp] = pc;
			for (int i = 0; i < 4; ++i)
			{
				mem[--sp] = r[i];
			}
			mem[--sp] = sr;

			pc = base + addr;
			
			clock += 3;
		}
		else if (opcode == 21)  //return
		{
			if (sp > 250)   //Stack underflow
            {
                sr &= 0xffffff1f; //initializes vm-status bits to 0
                sr |= 0x80;       //sets vm-status to 100
                return;
            }        
            sr = mem[sp++];
			for (int i = 3; i >= 0; --i)
			{
				r[i] = mem[sp++];
			}
			pc = mem[sp++];
            
            clock += 3;
		}
		else if (opcode == 22)  //read
		{
            /*int incoming;
            in >> incoming;
            r[rd] = incoming;*/

            

            //rd = rd<<8;
			in >> r[rd];
            sr &= 0xfffff01f;
            sr |= 0xc0;
            sr |= rd<<8;   //Sets I/O register bits
            isSwitch = true;
			
            return;
		}
		else if (opcode == 23)  //write
		{
			/*int outgoing;
			outgoing = r[rd];*/

			//If bit 16 is 1, sign extend through the rest since word is 32 bits
			if (r[rd] & 0x8000)
			{
				r[rd] |= 0xffff0000;
			}
            
           

            //rd = rd<<8;
			out << r[rd] << endl;
            sr &= 0xfffff01f;// was ffffff1f
            sr |= 0xd0;
            sr += rd;   //Sets I/O register bits

            //out << outgoing << endl;
			
            isSwitch = true;
            return;
		}
		else if (opcode == 24)  //halt
		{
            sr &= 0xffffff1f;
            sr |= 0x20;
			return;
		}
		else if (opcode == 25)  //noop
		{
			sr = sr;
		}
		else
		{
			cout << "Bad opcode = " << opcode << endl;
            sr &= 0xffffff1f;
            sr |= 0xa0;
			//exit(3);
            return;
		}

		if (debug)
		{
			printf("ir=%d op=%d rd=%d i=%d rs=%d const=%d addr=%d\n", ir,
				opcode, rd, i, rs, constant, addr);
			printf("r[0]=%d r[1]=%d r[2]=%d r[3]=%d pc=%d sr=%d sp=%d clock=%d\n\n", r[0], r[1], r[2], r[3], pc, sr, sp, clock);
			//char c;
			//cin >> c;
		}

        elapsedClock = clock - clockStart;  //Calculates elapsed clock so we
                                            //can update accounting information.

        if (clock >= sliceTime) //Time for a slice!
        {
            isSwitch = true;
            sr &= 0xffffff1f;
            return;
        }
	}

	if (debug)
	{
		for (j = 0; j < limit; ++j)
		{
			printf("%8d", mem[j]);
			if (j % 8 == 7)
			{
				printf("\n");
			}
		}
		cout << endl;
	}
} //main
