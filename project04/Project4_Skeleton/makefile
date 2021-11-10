OBJECTS = DotProduct.o Project4.o

TARGET = myDotProduct

$(TARGET): $(OBJECTS)
	g++ -Wall -lpthread -o $@ $^

clean:
	rm $(TARGET) $(OBJECTS)
