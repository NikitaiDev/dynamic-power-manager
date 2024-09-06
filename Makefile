CC = gcc
CFLAGS = -Wall -Wextra -pedantic
TARGET = dynamic-power-manager

$(TARGET): src/main.c
	$(CC) $(CFLAGS) -o $(TARGET) src/main.c

clean:
	rm -f $(TARGET)
