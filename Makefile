TARGET=timecard
OBJS=timecard.o

#compile as 32 bit binary with executable stack, and debug info for gdb
CFLAGS= -g -m32 -z execstack -fno-stack-protector

all: $(TARGET)

$(TARGET): $(OBJS)
	gcc $(CFLAGS) -o $(TARGET) $(TARGET).c
	sudo chown root $(TARGET)
	sudo chmod 4755 $(TARGET)

