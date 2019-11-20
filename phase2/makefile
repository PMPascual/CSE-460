
os: os_main.o os.o VirtualMachine.o Assembler.o
	g++ -o os os_main.o os.o VirtualMachine.o Assembler.o

main.o: os_main.cpp
	g++ -c main.cpp

os.o: os.cpp
	g++ -c os.cpp

VirtualMachine.o: VirtualMachine.cpp VirtualMachine.h
	g++ -c VirtualMachine.cpp

Assembler.o: Assembler.cpp Assembler.h
	g++ -c Assembler.cpp

clean:
	rm os *.o *.out
