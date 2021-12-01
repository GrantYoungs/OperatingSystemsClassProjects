# Name : Clifton Howard
# Project : Project 4
#
OBJECTS = Project5.o Reference.o ReferenceGenerator.o VirtualMemoryManager.o TLB.o Memory.o

TARGET = myMemory

$(TARGET): $(OBJECTS)
	g++ -Wall -o  $@ $^ 

clean:
	rm $(TARGET) $(OBJECTS)
