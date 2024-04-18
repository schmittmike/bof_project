TARGET=timecard
CFLAGS=-Wall -g

all: $(TARGET)

$(TARGET):
	gcc $(CFLAGS) -o $(TARGET) $(TARGET).c

