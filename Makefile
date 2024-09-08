CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Iinclude
LDFLAGS = -lusb-1.0 `pkg-config --cflags --libs gtk+-3.0 appindicator3-0.1` 
TARGET = dynamic-power-manager
PREFIX = /usr/local

SRCS = src/*
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Установка программы
install: $(TARGET)
	install -m 0755 $(TARGET) $(PREFIX)/bin/$(TARGET)

uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)
