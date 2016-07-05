CC = g++
CFLAGS = -g -v

# target name - basically in case it becomes main.cpp in the future,
# to accomodate extra files being included
TARGET = dictate

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	rm $(TARGET)
	rm /usr/local/bin/$(TARGET)

install:
	cp $(TARGET) /usr/local/bin
