OBJECTS = MyShell.o MyShellProcess.o Parser.o CShellParser.o MyShellParser.o RegularExpression.o Project2.o

TARGET = myShell

$(TARGET): $(OBJECTS)
	g++ -Wall -o $@ $^

clean:
	rm $(TARGET) $(OBJECTS)
