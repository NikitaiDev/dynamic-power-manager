CC = gcc
CFLAGS = -Wall -Wextra -pedantic
LDFLAGS = -lusb-1.0
TARGET = dynamic-power-manager
PREFIX = /usr/local

# Default rule to build the project
$(TARGET): src/main.c
	$(CC) $(CFLAGS) -o $(TARGET) src/main.c $(LDFLAGS) 

# Install the binary
install: $(TARGET)
	install -m 0755 $(TARGET) $(PREFIX)/bin/$(TARGET)

# Uninstall the binary
uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)

# Clean rule to remove compiled files
clean:
	rm -f $(TARGET)
