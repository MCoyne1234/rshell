SRCDIR = src
BINDIR = bin
EXECUTABLE = rshell

CC = g++
CFLAGS = -c -Wall -Werror -ansi -pedantic
LDFLAGS = 

TARGET = $(BINDIR)/$(EXECUTABLE)
SOURCES = $(wildcard $(addsuffix /*.cpp,$(SRCDIR)))
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

$(OBJECTS): %.o : %.cpp
	$(CC) $(CFLAGS) -o $@ $<

$(BINDIR):
	mkdir -p $@

clean:
	rm -f $(SRCDIR)/*.o
	rm -rf $(BINDIR)
