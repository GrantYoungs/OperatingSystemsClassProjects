OBJECTS = DotProduct.o Project4.o

TARGET = myDotProduct

$(TARGET): $(OBJECTS)
	g++ -Wall -lpthread -pthread -o $@ $^

clean:
	rm $(TARGET) $(OBJECTS)
