
os: main.o OS.o VirtualMachine.o Assembler.o
	g++ -o os main.o OS.o VirtualMachine.o Assembler.o

main.o: main.cpp
	g++ -c main.cpp
	
os.o: OS.cpp
	g++ -c OS.cpp

VirtualMachine.o: VirtualMachine.cpp VirtualMachine.h
	g++ -c VirtualMachine.cpp

Assembler.o: Assembler.cpp Assembler.h
	g++ -c Assembler.cpp

clean:
	rm os *.o *.out *.st
